//
// Created by ws on 2026/10/13.
//

#ifndef CODATAP_MAINWINDOW_H
#define CODATAP_MAINWINDOW_H


#pragma once

#include <QWidget>

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

private slots:
    void onButtonClicked();
};


#endif //CODATAP_MAINWINDOW_H
