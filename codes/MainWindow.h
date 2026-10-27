//
// Created by ws on 2026/10/13.
//

#ifndef CODATAP_MAINWINDOW_H
#define CODATAP_MAINWINDOW_H


#pragma once

#include <QLineEdit>
#include <QWidget>
#include <QPainter>

#include "SEvent.h"
#include "Tube.h"
#include "classes/TubeCache.h"

class Branch;
class QPlainTextEdit;
class QLineEdit;
class QPushButton;
class QListWidget;
class QPaintEvent;

class SuggestLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    using QLineEdit::QLineEdit;

    void setSuggestion(const QString& text);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString suggestion;
};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    TubeCache<std::string> tubeCache;
    void LogLine(std::string log);
    void LogLineHead();
    bool HasLineHeadInLastLine();
    void RemoveLastLine();
    void RemoveLineHeadWeatherExists();
public slots:
    void showAndActivate();

private:
    void initUI();

    QPushButton* button;
    QListWidget* list;
    QPlainTextEdit* log;
    SuggestLineEdit *input;

    SEvent<std::string> onTextChanged;
    SEvent<> onEnterDown, onTabDown,
        onRightDown, onLeftDown, onUpDown, onDownDown;

    ///if the shift key is pressed
    bool shift_ = false;

    void ToCommand();
    void SetInput(std::string input);
    void AppendInput(std::string text);
    std::string GetInput();
    void SetList(std::vector<std::string> elements);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void onButtonClicked();
    void SetInputSuggestion(Branch* node, std::vector<std::string> args, int listCount, std::string last);
};


#endif //CODATAP_MAINWINDOW_H
