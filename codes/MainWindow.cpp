//
// Created by ws on 2026/10/13.
//

#include "MainWindow.h"
#include "MainWindow.h"

#include <iostream>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QVBoxLayout>

#include "Core.h"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    initUI();

    input->installEventFilter(this);

    onEnterDown.Add([] {
        std::cout << "entered" << std::endl;
    });
    onEnterDown.Add([this] {
        auto text = this->GetInput();
        auto split = Core::Split(text, ' ');

        CommandArgument args;
        auto* branch = Core::branch.Parse(split, args);
        auto result = branch->Run(args);
        this->LogLine(result);

        input->clear();
    });

    connect(input, &QLineEdit::textEdited, this,
    [this](const QString& text) {
            onTextChanged.Invoke(Core::ToString(text));
        }
    );

    connect(button, &QPushButton::clicked,
            this, &MainWindow::onButtonClicked);
}

void MainWindow::initUI()
{
    this->resize(800, 400);

    auto* mainLayout = new QVBoxLayout(this);
    auto* out = new QHBoxLayout();
    auto* in = new QHBoxLayout();

    list = new QListWidget(this);
    input = new QLineEdit(this);
    button = new QPushButton("执行", this);
    log = new QPlainTextEdit(this);

    out->addWidget(log, 3);
    out->addWidget(list, 1);

    in->addWidget(input, 9);
    in->addWidget(button, 1);

    mainLayout->addLayout(out);
    mainLayout->addLayout(in);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == input) { //if is from input box
        auto* key = static_cast<QKeyEvent*>(event);
        if (event->type() == QEvent::KeyPress) { //key pressed
            switch (key->key()) {
                case Qt::Key_Shift:
                    shift_ = true;
                    return true;
                case Qt::Key_Tab:
                    onTabDown.Invoke();
                    return true;
                case Qt::Key_Return:
                case Qt::Key_Enter:
                    onEnterDown.Invoke();
                    return true;
            }
        }
        else if (event->type() == QEvent::KeyRelease) { // key released
            switch (key->key()) {
                case Qt::Key_Shift:
                    shift_ = false;
                    return true;
            }
        }

        return QWidget::eventFilter(obj, event);
    }
}
void MainWindow::LogLine(std::string logged) {
    log->appendPlainText(Core::ToQString(logged));
}

void MainWindow::AppendInput(std::string text) {
    auto v = Core::ToQString(text);
    input->setText(input->text() + v);
}

void MainWindow::SetInput(std::string text) {
    auto v = Core::ToQString(text);
    input->setText(v);
}
std::string MainWindow::GetInput() {
    return Core::ToString(input->text());
}

void MainWindow::onButtonClicked()
{
    // 获取输入框内容
    QString command = input->text();

    // 显示执行结果
    log->appendPlainText(command);

    // 把命令添加到列表
    list->addItem(command);

    // 清空输入框
    input->clear();
}