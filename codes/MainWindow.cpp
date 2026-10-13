//
// Created by ws on 2026/10/13.
//

#include "MainWindow.h"
#include "MainWindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    initUI();

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