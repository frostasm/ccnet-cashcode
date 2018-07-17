#include "CashCodeBillValidatorThreaded.h"

#include "CashCodeBillValidatorWorker.h"

#include <QDebug>

std::function<CashCodeBillValidatorBase*()> CashCodeBillValidatorThreaded::s_factory([]() {
    return new CashCodeBillValidatorWorker();
});

CashCodeBillValidatorThreaded::CashCodeBillValidatorThreaded(QObject *parent)
    : CashCodeBillValidatorBase(parent)
    , m_validator(s_factory())
{
    // move validator to workerThread
    m_validator->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_validator, &QObject::deleteLater);
    m_workerThread.start();

    resetConnections();

    // m_validator -> this
    connect(m_validator, &CashCodeBillValidatorBase::receptionStarted, this, &CashCodeBillValidatorBase::receptionStarted);
    connect(m_validator, &CashCodeBillValidatorBase::receptionStopped, this, &CashCodeBillValidatorBase::receptionStopped);

    connect(m_validator, &CashCodeBillValidatorBase::billAccepted, this, &CashCodeBillValidatorBase::billAccepted);
    connect(m_validator, &CashCodeBillValidatorBase::billRejected, this, &CashCodeBillValidatorBase::billRejected);

    connect(m_validator, &CashCodeBillValidatorBase::errorOccured, this, &CashCodeBillValidatorThreaded::setError);
    connect(m_validator, &CashCodeBillValidatorBase::receivedCashChanged,
            this, &CashCodeBillValidatorThreaded::setReceivedCash);
    connect(m_validator, &CashCodeBillValidatorBase::receptionRunningChanged,
            this, &CashCodeBillValidatorThreaded::setReceptionRunning);

    // this -> m_validator
    connect(this, &CashCodeBillValidatorThreaded::startReceptionRequested,
            m_validator, &CashCodeBillValidatorBase::startReception);
    connect(this, &CashCodeBillValidatorThreaded::stopReceptionRequested,
            m_validator, &CashCodeBillValidatorBase::stopReception);
}

CashCodeBillValidatorThreaded::~CashCodeBillValidatorThreaded()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void CashCodeBillValidatorThreaded::setCashCodeBillValidatorFactory(std::function<CashCodeBillValidatorBase*()> factory)
{
    s_factory = factory;
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
