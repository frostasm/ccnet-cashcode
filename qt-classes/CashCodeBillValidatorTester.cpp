#include "CashCodeBillValidatorTester.h"

#include <QDebug>

CashCodeBillValidatorTester::CashCodeBillValidatorTester(QObject *parent)
    : CashCodeBillValidatorBase(parent)
{

}

void CashCodeBillValidatorTester::startReception()
{
    emit startReceptionRequested();
}

void CashCodeBillValidatorTester::stopReception()
{
    emit stopReceptionRequested();
}
