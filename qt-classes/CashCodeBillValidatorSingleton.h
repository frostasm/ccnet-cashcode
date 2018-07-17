#ifndef CASHCODEBILLVALIDATORSINGLETON_H
#define CASHCODEBILLVALIDATORSINGLETON_H

#include "CashCodeBillValidatorThreaded.h"

#include <QThread>

#define ccBillValidator (static_cast<CashCodeBillValidatorBase *>(CashCodeBillValidatorSingleton::instance()))


class CashCodeBillValidatorSingleton : public CashCodeBillValidatorThreaded
{
    Q_OBJECT
public:

    static CashCodeBillValidatorSingleton* instance();

private:
    Q_DISABLE_COPY(CashCodeBillValidatorSingleton)

    CashCodeBillValidatorSingleton();
    ~CashCodeBillValidatorSingleton();

    static CashCodeBillValidatorSingleton* s_instance;
};

#endif // CASHCODEBILLVALIDATORSINGLETON_H
