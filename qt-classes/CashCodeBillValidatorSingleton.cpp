#include "CashCodeBillValidatorSingleton.h"

#include "CashCodeBillValidatorWorker.h"

#include <QCoreApplication>
#include <QDebug>


CashCodeBillValidatorSingleton* CashCodeBillValidatorSingleton::s_instance{nullptr};

CashCodeBillValidatorSingleton* CashCodeBillValidatorSingleton::instance()
{
    static bool static_init = []()->bool {
        s_instance = new CashCodeBillValidatorSingleton();
        return true;
    }();
    Q_UNUSED(static_init);

    return s_instance;
}

CashCodeBillValidatorSingleton::CashCodeBillValidatorSingleton()
    : CashCodeBillValidatorThreaded(qApp)
{
    connect(qApp, &QCoreApplication::aboutToQuit, this, &CashCodeBillValidatorSingleton::stopWorkerThread);
}

CashCodeBillValidatorSingleton::~CashCodeBillValidatorSingleton()
{
}
