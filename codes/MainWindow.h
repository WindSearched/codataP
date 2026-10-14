//
// Created by ws on 2026/10/13.
//

#ifndef CODATAP_MAINWINDOW_H
#define CODATAP_MAINWINDOW_H


#pragma once

#include <QWidget>

#include "SEvent.h"

class QPlainTextEdit;
class QLineEdit;
class QPushButton;
class QListWidget;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void initUI();

    QLineEdit* input;
    QPushButton* button;
    QListWidget* list;
    QPlainTextEdit* log;

    SEvent<std::string> onTextChanged;
    SEvent<>  onEnterDown, onTabDown;

    ///if the shift key is pressed
    bool shift_ = false;

    void ToCommand();
    void LogLine(std::string log);
    void SetInput(std::string input);
    void AppendInput(std::string text);
    std::string GetInput();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void onButtonClicked();
};


#endif //CODATAP_MAINWINDOW_H
