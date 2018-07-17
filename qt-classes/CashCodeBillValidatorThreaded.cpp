#include "CashCodeBillValidatorThreaded.h"

#include "CashCodeBillValidatorWorker.h"

#include <QDebug>

CashCodeBillValidatorThreaded::CashCodeBillValidatorThreaded(QObject *parent)
    : CashCodeBillValidatorBase(parent)
    , m_validator(new CashCodeBillValidatorWorker())
{
    // move validator to workerThread
    m_validator->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_validator, &QObject::deleteLater);
    m_workerThread.start();

    // m_validator -> this
    connect(m_validator, &CashCodeBillValidatorWorker::receptionStarted, this, &CashCodeBillValidatorBase::receptionStarted);
    connect(m_validator, &CashCodeBillValidatorWorker::receptionStopped, this, &CashCodeBillValidatorBase::receptionStopped);

    connect(m_validator, &CashCodeBillValidatorWorker::billAccepted, this, &CashCodeBillValidatorBase::billAccepted);
    connect(m_validator, &CashCodeBillValidatorWorker::billRejected, this, &CashCodeBillValidatorBase::billRejected);

    connect(m_validator, &CashCodeBillValidatorWorker::errorOccured, this, &CashCodeBillValidatorThreaded::setError);
    connect(m_validator, &CashCodeBillValidatorWorker::receivedCashChanged,
            this, &CashCodeBillValidatorThreaded::setReceivedCash);
    connect(m_validator, &CashCodeBillValidatorWorker::receptionRunningChanged,
            this, &CashCodeBillValidatorThreaded::setReceptionRunning);

    // this -> m_validator
    connect(this, &CashCodeBillValidatorThreaded::startReceptionRequested,
            m_validator, &CashCodeBillValidatorWorker::startReception);
    connect(this, &CashCodeBillValidatorThreaded::stopReceptionRequested,
            m_validator, &CashCodeBillValidatorWorker::stopReception);
}

CashCodeBillValidatorThreaded::~CashCodeBillValidatorThreaded()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void CashCodeBillValidatorThreaded::startReception()
{
    emit startReceptionRequested(QPrivateSignal());
}

void CashCodeBillValidatorThreaded::stopReception()
{
    emit stopReceptionRequested(QPrivateSignal());
}

void CashCodeBillValidatorThreaded::stopWorkerThread()
{
    m_workerThread.quit();
    m_workerThread.wait();
}
