#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void onButtonClicked();

private:
    QLineEdit *display;
    QGridLayout *layout;

    // 記錄運算過程的變數
    QString currentOperator;
    double storedValue;
    bool waitingForOperand;

    // 方法來初始化按鈕
    void createButtons();
    void calculate();
};

#endif // DIALOG_H
