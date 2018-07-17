#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CashCodeBillValidatorTester.h"
#include "CashCodeBillValidatorSingleton.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    {
        CashCodeBillValidatorTester* validatorTester = new CashCodeBillValidatorTester();
        auto testingFactory = [validatorTester]() -> CashCodeBillValidatorTester* { return validatorTester; };
        CashCodeBillValidatorSingleton::setCashCodeBillValidatorFactory(testingFactory);

        connect(validatorTester, &CashCodeBillValidatorTester::startReceptionRequested, this, [this]() {
            ui->plainTextEditLogs->appendPlainText("startReceptionRequested\n");
        });
        connect(validatorTester, &CashCodeBillValidatorTester::stopReceptionRequested, this, [this]() {
            ui->plainTextEditLogs->appendPlainText("stopReceptionRequested\n");
        });

        connect(ui->pbtnStartReception, &QPushButton::clicked,
                validatorTester, &CashCodeBillValidatorTester::receptionStarted);
        connect(ui->pbtnStopReception, &QPushButton::clicked,
                validatorTester, &CashCodeBillValidatorTester::receptionStopped);

        connect(ui->pbtnBillAccepted, &QPushButton::clicked, validatorTester, [validatorTester, this]() {
             validatorTester->billAccepted(ui->spinBoxBillAccepted->value());
        });

        connect(ui->pbtnBillRejected, &QPushButton::clicked, validatorTester, [validatorTester, this]() {
             validatorTester->billRejected(ui->spinBoxBillAccepted->value());
        });

    }
    const CashCodeBillValidatorBase* billValidator = ccBillValidator;
    connect(billValidator, &CashCodeBillValidatorBase::receptionRunningChanged, this, [this](bool running){
        const QString newText = running ? "Stop" : "Start";
        ui->pushButtonRunStop->setEnabled(true);
        ui->pushButtonRunStop->setText(newText);
    });

    connect(billValidator, &CashCodeBillValidatorBase::receivedCashChanged, this, [this](int amount){
        ui->labelCashAmount->setText(QString::number(amount));
    });

    connect(billValidator, &CashCodeBillValidatorBase::errorOccured, this, [this](QString error){
        ui->pushButtonRunStop->setEnabled(true);
        ui->pushButtonRunStop->setText("Start");
        QMessageBox::critical(this, "Error", error);
    });

    connect(billValidator, &CashCodeBillValidatorBase::billAccepted, this, [this, billValidator](int denomination){
        ui->plainTextEditLogs->appendPlainText(QString("Accepted: %1").arg(denomination));
        ui->plainTextEditLogs->appendPlainText(QString("Sum: %1").arg(billValidator->receivedCash()));
    }, Qt::QueuedConnection);

    connect(billValidator, &CashCodeBillValidatorBase::billRejected, this, [this, billValidator](int denomination){
        ui->plainTextEditLogs->appendPlainText(QString("Rejected: %1").arg(denomination));
    }, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonRunStop_clicked()
{
    CashCodeBillValidatorBase * const billValidator = ccBillValidator;
    const bool isRunning = billValidator->isReceptionRunning();
    ui->pushButtonRunStop->setEnabled(false);
    ui->pushButtonRunStop->setText(isRunning ? "Stopping..." : "Starting...");
    if (isRunning) {
        billValidator->stopReception();
    } else {
        billValidator->startReception();
    }
}
