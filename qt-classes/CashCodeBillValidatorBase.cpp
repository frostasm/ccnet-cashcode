#include "CashCodeBillValidatorBase.h"

#include "cashcodeprotocol.h"

#include <QScopedPointer>

CashCodeBillValidatorBase::CashCodeBillValidatorBase(QObject *parent) : QObject(parent)
{
    connect(this, &CashCodeBillValidatorBase::receptionStarted, this, [this](){ setReceptionRunning(true); });
    connect(this, &CashCodeBillValidatorBase::receptionStopped, this, [this](){ setReceptionRunning(false); });
    connect(this, &CashCodeBillValidatorBase::errorOccured, this, [this](){ setReceptionRunning(false); });
    connect(this, &CashCodeBillValidatorBase::billAccepted, this, [this](int denomination){
        setReceivedCash(m_cashReceived + denomination);
    });
}

CashCodeBillValidatorBase::~CashCodeBillValidatorBase()
{
}

int CashCodeBillValidatorBase::receivedCash() const
{
    return m_cashReceived;
}

bool CashCodeBillValidatorBase::isReceptionRunning() const
{
    return m_receptionRunning;
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
    if (m_cashReceived == cashReceived)
        return;

    m_cashReceived = cashReceived;
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
