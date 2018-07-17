#ifndef CASHCODEBILLVALIDATORSINGLETON_H
#define CASHCODEBILLVALIDATORSINGLETON_H

#include "CashCodeBillValidatorBase.h"

#include <QThread>

#define ccBillValidator (static_cast<CashCodeBillValidatorBase *>(CashCodeBillValidatorSingleton::instance()))


class CashCodeBillValidator;

class CashCodeBillValidatorSingleton : public CashCodeBillValidatorBase
{
    Q_OBJECT
public:

    static CashCodeBillValidatorSingleton* instance();

signals:
    void startReceptionRequested(QPrivateSignal);
    void stopReceptionRequested(QPrivateSignal);

public slots:
    void startReception() override;
    void stopReception() override;

private:
    Q_DISABLE_COPY(CashCodeBillValidatorSingleton)

    CashCodeBillValidatorSingleton();
    ~CashCodeBillValidatorSingleton();

    static CashCodeBillValidatorSingleton* s_instance;

    QThread m_workerThread;
    CashCodeBillValidator* m_validator{nullptr};
};

#endif // CASHCODEBILLVALIDATORSINGLETON_H
