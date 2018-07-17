#ifndef CASHCODEBILLVALIDATOR_H
#define CASHCODEBILLVALIDATOR_H

#include "IBillValidatorEventsListener.h"

#include <QObject>
#include <QSharedPointer>

#include <atomic>

class CashCodeProtocol;

class CashCodeBillValidator : public QObject, public IBillValidatorEventsListener
{
    Q_OBJECT
    Q_PROPERTY(int receivedCash READ receivedCash WRITE setReceivedCash NOTIFY receivedCashChanged)
    Q_PROPERTY(bool receptionRunning READ isReceptionRunning WRITE setReceptionRunning NOTIFY receptionRunningChanged)

public:
    explicit CashCodeBillValidator(QObject *parent = nullptr);
    virtual ~CashCodeBillValidator();

    int receivedCash() const;

    bool isReceptionRunning() const;

signals:
    void receptionStarted();
    void receptionStopped();
    void errorOccured(const QString& error);
    void billAccepted(int denomination);
    void billRejected(int denomination);

    void receivedCashChanged(int receivedCash);
    void receptionRunningChanged(bool receptionRunning);

public slots:
    void startReception();
    void stopReception();

protected slots:
    void setReceivedCash(int receivedCash);
    void setError(const QString &errorMessage);
    void setReceptionRunning(bool receptionRunning);

protected:
    virtual void billValidatorReceptionStarted();
    virtual void billValidatorReceptionStopped();
    virtual void billValidatorErrorOccured(const std::string& error);
    virtual void billValidatorBillAccepted(int denomination);
    virtual void billValidatorBillRejected(int denomination);

private:
    void clearReceivedCash();
    bool ensureCashCodeProtocolInitialized();
    void deleteCashCodeProtocol();

    std::atomic<int> m_cashReceived{0};
    QSharedPointer<CashCodeProtocol> m_protocol;
    bool m_receptionRunning{false};
};

#endif // CASHCODEBILLVALIDATOR_H
