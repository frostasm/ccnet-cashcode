#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class CashCodeBillValidatorWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonRunStop_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
