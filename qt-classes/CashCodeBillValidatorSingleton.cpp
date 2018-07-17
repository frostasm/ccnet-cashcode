#include "CashCodeBillValidatorSingleton.h"

#include "CashCodeBillValidator.h"

#include <QCoreApplication>
#include <QDebug>


CashCodeBillValidatorSingleton* CashCodeBillValidatorSingleton::s_instance{nullptr};

CashCodeBillValidatorSingleton* CashCodeBillValidatorSingleton::instance()
{
    static bool static_init = []()->bool {
        s_instance = new CashCodeBillValidatorSingleton();
        return true;
    }();
    Q_UNUSED(static_init);

    return s_instance;
}

CashCodeBillValidatorSingleton::CashCodeBillValidatorSingleton()
    : CashCodeBillValidatorBase(qApp)
    , m_validator(new CashCodeBillValidator())
{
    // move validator to workerThread
    m_validator->moveToThread(&m_workerThread);
    connect(qApp, &QCoreApplication::aboutToQuit, m_validator, &QObject::deleteLater);
    connect(m_validator, &CashCodeBillValidator::destroyed, &m_workerThread, &QThread::quit);
    m_workerThread.start();

    // m_validator -> this
    connect(m_validator, &CashCodeBillValidator::receptionStarted, this, &CashCodeBillValidatorBase::receptionStarted);
    connect(m_validator, &CashCodeBillValidator::receptionStopped, this, &CashCodeBillValidatorBase::receptionStopped);

    connect(m_validator, &CashCodeBillValidator::billAccepted, this, &CashCodeBillValidatorBase::billAccepted);
    connect(m_validator, &CashCodeBillValidator::billRejected, this, &CashCodeBillValidatorBase::billRejected);

    connect(m_validator, &CashCodeBillValidator::errorOccured, this, &CashCodeBillValidatorSingleton::setError);
    connect(m_validator, &CashCodeBillValidator::receivedCashChanged,
            this, &CashCodeBillValidatorSingleton::setReceivedCash);
    connect(m_validator, &CashCodeBillValidator::receptionRunningChanged,
            this, &CashCodeBillValidatorSingleton::setReceptionRunning);

    // this -> m_validator
    connect(this, &CashCodeBillValidatorSingleton::startReceptionRequested,
            m_validator, &CashCodeBillValidator::startReception);
    connect(this, &CashCodeBillValidatorSingleton::stopReceptionRequested,
            m_validator, &CashCodeBillValidator::stopReception);
}

CashCodeBillValidatorSingleton::~CashCodeBillValidatorSingleton()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void CashCodeBillValidatorSingleton::startReception()
{
    emit startReceptionRequested(QPrivateSignal());
}

void CashCodeBillValidatorSingleton::stopReception()
{
    emit stopReceptionRequested(QPrivateSignal());
}
