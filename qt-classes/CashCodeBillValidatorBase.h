#ifndef CASHCODEBILLVALIDATORBASE_H
#define CASHCODEBILLVALIDATORBASE_H

#include <QObject>
#include <QSharedPointer>

#include <atomic>


class CashCodeBillValidatorBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int receivedCash READ receivedCash WRITE setReceivedCash NOTIFY receivedCashChanged)
    Q_PROPERTY(bool receptionRunning READ isReceptionRunning WRITE setReceptionRunning NOTIFY receptionRunningChanged)

public:
    explicit CashCodeBillValidatorBase(QObject *parent = nullptr);
    virtual ~CashCodeBillValidatorBase();

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
    virtual void startReception();
    virtual void stopReception();

protected slots:
    virtual void setReceivedCash(int receivedCash);
    virtual void setError(const QString &errorMessage);
    virtual void setReceptionRunning(bool receptionRunning);
    virtual void clearReceivedCash();

private:
    std::atomic<int> m_cashReceived{0};
    std::atomic<bool> m_receptionRunning{false};
};

#endif // CASHCODEBILLVALIDATORBASE_H
