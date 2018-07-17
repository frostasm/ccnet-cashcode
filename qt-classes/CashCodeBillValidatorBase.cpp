#include "CashCodeBillValidatorBase.h"

#include "cashcodeprotocol.h"

#include <QScopedPointer>
#include <QDebug>


CashCodeBillValidatorBase::CashCodeBillValidatorBase(QObject *parent) : QObject(parent)
{
    initConnections();
}

CashCodeBillValidatorBase::~CashCodeBillValidatorBase()
{
}

int CashCodeBillValidatorBase::receivedCash() const
{
    return m_receivedCash;
}

bool CashCodeBillValidatorBase::isReceptionRunning() const
{
    return m_receptionRunning;
}

void CashCodeBillValidatorBase::startReception()
{
    Q_ASSERT_X(false, "startReception", "You need to override this method in child classes");
}

void CashCodeBillValidatorBase::stopReception()
{
    Q_ASSERT_X(false, "stopReception", "You need to override this method in child classes");
}

void CashCodeBillValidatorBase::setReceptionRunning(bool receptionRunning)
{
    if (m_receptionRunning == receptionRunning)
        return;

    m_receptionRunning = receptionRunning;
    emit receptionRunningChanged(m_receptionRunning);
}

void CashCodeBillValidatorBase::setReceivedCash(int cashReceived)
{
    if (m_receivedCash == cashReceived)
        return;

    m_receivedCash = cashReceived;
    emit receivedCashChanged(cashReceived);
}

void CashCodeBillValidatorBase::setError(const QString &errorMessage)
{
    emit errorOccured(errorMessage);
}

void CashCodeBillValidatorBase::clearReceivedCash()
{
    setReceivedCash(0);
}

void CashCodeBillValidatorBase::setReceptionRunningToTrue()
{
    setReceptionRunning(true);
}

void CashCodeBillValidatorBase::setReceptionRunningToFalse()
{
    setReceptionRunning(false);
}

void CashCodeBillValidatorBase::updateReceivedCash(int denomination)
{
    setReceivedCash(m_receivedCash + denomination);
}

void CashCodeBillValidatorBase::initConnections()
{
    connect(this, &CashCodeBillValidatorBase::receptionStarted,
            this, &CashCodeBillValidatorBase::setReceptionRunningToTrue);
    connect(this, &CashCodeBillValidatorBase::receptionStopped,
            this, &CashCodeBillValidatorBase::setReceptionRunningToFalse);
    connect(this, &CashCodeBillValidatorBase::errorOccured,
            this, &CashCodeBillValidatorBase::setReceptionRunningToFalse);
    connect(this, &CashCodeBillValidatorBase::billAccepted,
            this, &CashCodeBillValidatorBase::updateReceivedCash);
}

void CashCodeBillValidatorBase::resetConnections()
{
    disconnect(this, &CashCodeBillValidatorBase::receptionStarted,
               this, &CashCodeBillValidatorBase::setReceptionRunningToTrue);
    disconnect(this, &CashCodeBillValidatorBase::receptionStopped,
               this, &CashCodeBillValidatorBase::setReceptionRunningToFalse);
    disconnect(this, &CashCodeBillValidatorBase::errorOccured,
               this, &CashCodeBillValidatorBase::setReceptionRunningToFalse);
    disconnect(this, &CashCodeBillValidatorBase::billAccepted,
               this, &CashCodeBillValidatorBase::updateReceivedCash);
}
