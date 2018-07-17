#include "CashCodeBillValidator.h"

#include "cashcodeprotocol.h"

#include <QScopedPointer>

CashCodeBillValidator::CashCodeBillValidator(QObject *parent)
    : CashCodeBillValidatorBase(parent)
{
}

CashCodeBillValidator::~CashCodeBillValidator()
{
    stopReception();
}

void CashCodeBillValidator::startReception()
{
    clearReceivedCash();

    if (ensureCashCodeProtocolInitialized()) {
        Q_ASSERT(!m_protocol.isNull());

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
