#include "dialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QJSEngine>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), storedValue(0), waitingForOperand(true)
{
    // 創建佈局並初始化按鈕
    createButtons();

    // 主佈局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);

    setLayout(mainLayout);
    setWindowTitle("Calculator");

    setFixedSize(300, 300);  // 固定視窗大小
}

Dialog::~Dialog() {}

void Dialog::createButtons()
{
    // 初始化網格佈局
    layout = new QGridLayout;

    // 顯示區域
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setText("0");
    display->setMinimumHeight(30);  // 設定顯示框的高度

    // 將顯示框加入到佈局中第一行
    layout->addWidget(display, 0, 0, 1, 4);

    // 定義按鈕的名稱和位置
    QStringList buttons = {
        "DEL", "/", "*", "-",
        "7", "8", "9", "+",
        "4", "5", "6", "=",
        "1", "2", "3", "",
        "0", "", ".", ""
    };

    int pos = 0;
    for (int row = 1; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (pos < buttons.size()) {
                QString buttonText = buttons[pos];
                if (!buttonText.isEmpty()) {  // 跳過空按鈕位置
                    QPushButton *button = new QPushButton(buttonText, this);
                    button->setMinimumSize(50, 40);  // 設定按鈕大小
                    layout->addWidget(button, row, col);
                    connect(button, &QPushButton::clicked, this, &Dialog::onButtonClicked);
                }
                pos++;
            }
        }
    }
}

void Dialog::onButtonClicked()
{
    // 處理按鈕點擊事件
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (!button) return;

    QString buttonText = button->text();

    if (buttonText == "C") {
        display->clear();
        display->setText("0");
        storedValue = 0;
        currentOperator.clear();
        waitingForOperand = true;
    } else if (buttonText == "DEL") {
        display->backspace();
        if (display->text().isEmpty()) {
            display->setText("0");
        }
    } else if (buttonText == "=") {
        calculate();
        currentOperator.clear();
        waitingForOperand = true;
    } else if (buttonText == "+" || buttonText == "-" || buttonText == "*" || buttonText == "/") {
        // 儲存當前顯示的數字，清除顯示框，並等待下一個操作數
        storedValue = display->text().toDouble();
        currentOperator = buttonText;
        display->clear();  // 清除顯示框
        waitingForOperand = true;
    } else {  // 處理數字按鈕
        if (waitingForOperand) {
            display->clear();
            waitingForOperand = false;
        }
        if (display->text() == "0") {
            display->clear();
        }
        display->setText(display->text() + buttonText);
    }
}

void Dialog::calculate()
{
    double operand = display->text().toDouble();
    double result = storedValue;

    if (currentOperator == "+") {
        result += operand;
    } else if (currentOperator == "-") {
        result -= operand;
    } else if (currentOperator == "*") {
        result *= operand;
    } else if (currentOperator == "/") {
        if (operand != 0.0) {
            result /= operand;
        } else {
            display->setText("Error");
            return;
        }
    }

    display->setText(QString::number(result));
    storedValue = result;
}
