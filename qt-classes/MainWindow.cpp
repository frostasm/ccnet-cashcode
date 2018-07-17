#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CashCodeBillValidator.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_billValidator = new CashCodeBillValidator(this);
    connect(m_billValidator, &CashCodeBillValidator::receptionRunningChanged, this, [this](){
        const QString newText = m_billValidator->isReceptionRunning() ? "Stop" : "Start";
        ui->pushButtonRunStop->setEnabled(true);
        ui->pushButtonRunStop->setText(newText);
    });

    connect(m_billValidator, &CashCodeBillValidator::receivedCashChanged, this, [this](int amount){
        ui->labelCashAmount->setText(QString::number(amount));
    });

    connect(m_billValidator, &CashCodeBillValidator::errorOccured, this, [this](QString error){
        ui->pushButtonRunStop->setEnabled(true);
        ui->pushButtonRunStop->setText("Start");
        QMessageBox::critical(this, "Error", error);
    });

    connect(m_billValidator, &CashCodeBillValidator::billAccepted, this, [this](int denomination){
        ui->plainTextEditLogs->appendPlainText(QString("Accepted: %1").arg(denomination));
    });

    connect(m_billValidator, &CashCodeBillValidator::billRejected, this, [this](int denomination){
        ui->plainTextEditLogs->appendPlainText(QString("Rejected: %1").arg(denomination));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonRunStop_clicked()
{
    const bool isRunning = m_billValidator->isReceptionRunning();
    ui->pushButtonRunStop->setEnabled(false);
    ui->pushButtonRunStop->setText(isRunning ? "Stopping..." : "Starting...");
    if (isRunning) {
        m_billValidator->stopReception();
    } else {
        m_billValidator->startReception();
    }
}
