#include "CashCodeBillValidator.h"

#include "cashcodeprotocol.h"

#include <QScopedPointer>

CashCodeBillValidator::CashCodeBillValidator(QObject *parent) : QObject(parent)
{
    connect(this, &CashCodeBillValidator::receptionStarted, this, &CashCodeBillValidator::clearReceivedCash);
    connect(this, &CashCodeBillValidator::receptionStarted, this, [this](){ setReceptionRunning(true); });
    connect(this, &CashCodeBillValidator::receptionStopped, this, [this](){ setReceptionRunning(false); });
    connect(this, &CashCodeBillValidator::errorOccured, this, [this](){ setReceptionRunning(false); });
    connect(this, &CashCodeBillValidator::billAccepted, this, [this](int denomination){
        setReceivedCash(m_cashReceived + denomination);
    });
}

CashCodeBillValidator::~CashCodeBillValidator()
{
    stopReception();
}

int CashCodeBillValidator::receivedCash() const
{
    return m_cashReceived;
}

bool CashCodeBillValidator::isReceptionRunning() const
{
    return m_receptionRunning;
}

void CashCodeBillValidator::startReception()
{
    if (ensureCashCodeProtocolInitialized()) {
        Q_ASSERT(!m_protocol.isNull());

        m_cashReceived = 0;
        const bool enableOk = m_protocol->EnableSequence() == 0;
        if (!enableOk) {
            setError("Can't enable bill validator");
            return;
        }

        m_protocol->StartListening();
    }
}

void CashCodeBillValidator::stopReception()
{
    if (!m_protocol) {
        return;
    }

    if (m_protocol->isValidatorListening()) {
        m_protocol->StopListening();
        m_protocol->DisableSequence();
    }
}

void CashCodeBillValidator::setReceptionRunning(bool receptionRunning)
{
    if (m_receptionRunning == receptionRunning)
        return;

    m_receptionRunning = receptionRunning;
    emit receptionRunningChanged(m_receptionRunning);
}

void CashCodeBillValidator::setReceivedCash(int cashReceived)
{
    if (m_cashReceived == cashReceived)
        return;

    m_cashReceived = cashReceived;
    emit receivedCashChanged(cashReceived);
}

void CashCodeBillValidator::setError(const QString &errorMessage)
{
    deleteCashCodeProtocol();
    emit errorOccured(errorMessage);
}

void CashCodeBillValidator::billValidatorReceptionStarted()
{
    emit receptionStarted();
}

void CashCodeBillValidator::billValidatorReceptionStopped()
{
    emit receptionStopped();
}

void CashCodeBillValidator::billValidatorErrorOccured(const std::string &error)
{
    setError(QString::fromStdString(error));
}

void CashCodeBillValidator::billValidatorBillAccepted(int denomination)
{
    emit billAccepted(denomination);
}

void CashCodeBillValidator::billValidatorBillRejected(int denomination)
{
    emit billRejected(denomination);
}

void CashCodeBillValidator::clearReceivedCash()
{
    setReceivedCash(0);
}

bool CashCodeBillValidator::ensureCashCodeProtocolInitialized()
{
    if (m_protocol) {
        return true;
    }

    QScopedPointer<CashCodeProtocol> protocol(new CashCodeProtocol(this));
    protocol->SetCashCodeTable({
                                   {0, 1},
                                   {1, 2},
                                   {2, 5},
                                   {3, 10},
                                   {4, 20},
                                   {5, 50},
                                   {6, 100},
                                   {7, 200},
                                   {8, 500}
                               });

    const int connectResult = protocol->ConnectValidator();
    if (connectResult != 0) {
        setError("Can't connect to CashCode Bill Validator");
        return false;
    }


    const int powerUpResult = protocol->PowerUpValidator();
    if (powerUpResult != 0) {
        setError("Can't Power Up CashCode Bill Validator");
        return false;
    }

    m_protocol = QSharedPointer<CashCodeProtocol>(protocol.take());
    return true;
}

void CashCodeBillValidator::deleteCashCodeProtocol()
{
    if (m_protocol) {
        m_protocol.clear();
    }
}
