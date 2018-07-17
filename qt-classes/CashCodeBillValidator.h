#ifndef CASHCODEBILLVALIDATOR_H
#define CASHCODEBILLVALIDATOR_H

#include "CashCodeBillValidatorBase.h"

#include "IBillValidatorEventsListener.h"

#include <QObject>
#include <QSharedPointer>

class CashCodeProtocol;

class CashCodeBillValidator : public CashCodeBillValidatorBase, public IBillValidatorEventsListener
{
    Q_OBJECT
public:
    explicit CashCodeBillValidator(QObject *parent = nullptr);
    virtual ~CashCodeBillValidator();

public: // IBillValidatorEventsListener
    virtual void billValidatorReceptionStarted();
    virtual void billValidatorReceptionStopped();
    virtual void billValidatorErrorOccured(const std::string& error);
    virtual void billValidatorBillAccepted(int denomination);
    virtual void billValidatorBillRejected(int denomination);

public slots:
    void startReception() override;
    void stopReception() override;

private:
    //! emit errorOccured at error
    bool ensureCashCodeProtocolInitialized();
    void deleteCashCodeProtocol();

    QSharedPointer<CashCodeProtocol> m_protocol;
};

#endif // CASHCODEBILLVALIDATOR_H
