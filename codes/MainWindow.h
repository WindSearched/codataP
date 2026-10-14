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

class QPlainTextEdit;
class QLineEdit;
class QPushButton;
class QListWidget;

class SuggestLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    using QLineEdit::QLineEdit;

    void setSuggestion(const QString& text)
    {
        suggestion = text;
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        QLineEdit::paintEvent(event);

        if (suggestion.isEmpty() || !hasFocus())
            return;

        QPainter painter(this);
        painter.setPen(QColor(160, 160, 160));

        const int x = fontMetrics().horizontalAdvance(
            text().left(cursorPosition())
        );

        painter.drawText(
            x + 8,
            (height() + fontMetrics().ascent()
                 - fontMetrics().descent()) / 2,
            suggestion
        );
    }

private:
    QString suggestion;
};

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
    SuggestLineEdit *inputSuggest;

    SEvent<std::string> onTextChanged;
    SEvent<> onEnterDown, onTabDown,
        onRightDown, onLeftDown, onUpDown, onDownDown;

    ///if the shift key is pressed
    bool shift_ = false;

    void ToCommand();
    void LogLine(std::string log);
    void SetInput(std::string input);
    void AppendInput(std::string text);
    std::string GetInput();
    void SetList(std::vector<std::string> elements);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void onButtonClicked();
};


#endif //CODATAP_MAINWINDOW_H
