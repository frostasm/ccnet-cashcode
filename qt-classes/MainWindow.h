#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class CashCodeBillValidator;

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

    CashCodeBillValidator* m_billValidator{nullptr};
};

#endif // MAINWINDOW_H
