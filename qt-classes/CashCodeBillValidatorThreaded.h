#ifndef CASHCODEBILLVALIDATORTHREADED_H
#define CASHCODEBILLVALIDATORTHREADED_H

#include "CashCodeBillValidatorBase.h"

#include <QThread>


class CashCodeBillValidatorWorker;

class CashCodeBillValidatorThreaded : public CashCodeBillValidatorBase
{
    Q_OBJECT
public:
    CashCodeBillValidatorThreaded(QObject* parent = nullptr);
    ~CashCodeBillValidatorThreaded();

signals:
    void startReceptionRequested(QPrivateSignal);
    void stopReceptionRequested(QPrivateSignal);

public slots:
    void startReception() override;
    void stopReception() override;

protected slots:
    void stopWorkerThread();

private:
    Q_DISABLE_COPY(CashCodeBillValidatorThreaded)

    QThread m_workerThread;
    CashCodeBillValidatorWorker* m_validator{nullptr};
};

#endif // CASHCODEBILLVALIDATORTHREADED_H
