#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CashCodeBillValidatorSingleton.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    connect(billValidator, &CashCodeBillValidatorBase::billAccepted, this, [this](int denomination){
        ui->plainTextEditLogs->appendPlainText(QString("Accepted: %1").arg(denomination));
    });

    connect(billValidator, &CashCodeBillValidatorBase::billRejected, this, [this](int denomination){
        ui->plainTextEditLogs->appendPlainText(QString("Rejected: %1").arg(denomination));
    });
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
