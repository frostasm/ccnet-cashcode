#ifndef CASHCODEBILLVALIDATORTESTER_H
#define CASHCODEBILLVALIDATORTESTER_H

#include "CashCodeBillValidatorBase.h"

class CashCodeBillValidatorTester : public CashCodeBillValidatorBase
{
    Q_OBJECT
public:
    explicit CashCodeBillValidatorTester(QObject *parent = nullptr);

signals:
    void startReceptionRequested();
    void stopReceptionRequested();

public slots:
    void startReception() override;
    void stopReception() override;
};

#endif // CASHCODEBILLVALIDATORTESTER_H
