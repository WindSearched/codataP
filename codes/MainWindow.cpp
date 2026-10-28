//
// Created by ws on 2026/10/13.
//

#include "MainWindow.h"

#include <iostream>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <ranges>

#include "Core.h"
#include "Sets.h"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    tubeCache.New(10);

    initUI();
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);
    //setCentralWidget(central);

    input->installEventFilter(this);

    //add put in tube function
    onEnterDown.Add([this] {
        auto t = this->GetInput();
        if (t.empty())return;

        tubeCache.Put(t);
        tubeCache.Revert();
    });
    //add enter function
    onEnterDown.Add([this] {
        auto text = this->GetInput();
        auto split = Core::Split(text, ' ');

        std::string put = text + "\n"
            + Core::branch.Command(split);
        this -> LogLine(put);

        input->clear();

        LogLineHead();
    });
    // command cache
    onUpDown.Add([this] {
        if (!shift_) return;
        if (tubeCache.pointer == 0)
            tubeCache.PutFront(this->GetInput());
        auto t = tubeCache.MoveBack();
        if (!t.empty())
            SetInput(t);
    });
    // command cache
    onDownDown.Add([this] {
        if (!shift_) return;
        if (tubeCache.pointer != 0) {
            SetInput(tubeCache.MoveFront());
        }
    });
    //add the suggestion
    onTextChanged.Add([this](std::string text) {
        auto s = Core::Split(text, ' ');
        auto last = s.empty() ? "" : s.back();
        s.pop_back();
        CommandArgument args;
        auto* branch = Core::branch.Parse(s, args);

        // 提取字段
        auto l1 = branch->branches
            | std::views::transform([](const Branch& b) {
                  return b.name;
              });



        // 合并到同一个 vector
        std::vector<std::string> result;
        std::ranges::copy(l1, std::back_inserter(result));
        if (!branch->args.empty() && args.Count() < branch->args.size()) {
            auto l2 = branch->args[args.Count()].GetSuggestion(last);
            std::ranges::copy(l2, std::back_inserter(result));
        }

        // 剔除满足条件的元素：示例为删除空字符串
        std::erase_if(result, [last](const std::string& s) {
            return !s.starts_with(last);
        });

        //set to listbox
        SetList(result);
        // auto a = s
        // | std::views::drop(s.size() - branch->args.size() + args.Count())
        // | std::views::take(s.size());
        // std::vector<std::string> l;
        // std::ranges::copy(a, std::back_inserter(l));

        SetInputSuggestion(branch, args.GetList(), result.size(), last);
    });
    //add setting of suggestion
    onTabDown.Add([this] {
        if (list->count() == 0) return;
        auto item = list->currentItem();
        if (item == nullptr) item = list->item(0);

        auto t = this->GetInput();

        auto pos = t.find_last_of(' ');
        if (pos != std::string::npos) {
            t.erase(pos);
        }
        else {
            t.clear();
        }

        t += ( t.empty() ? "" : " ") + Core::ToString(item->text()) + " ";

        this->SetInput(t);
        onTextChanged.Invoke(t);
    });
    //adding selection of suggestion
    onUpDown.Add([this] {
        if (shift_) return;
        int row = list->currentRow();
        if (row == -1) {
            row = 0;
        }
        else {
            row--;
            if (row <0)
                row = list->count() - 1;
        }
        list->setCurrentRow(row);
    });
    onDownDown.Add([this] {
        if (shift_) return;
        int row = list->currentRow();
        if (row == -1) {
            row = 0;
        }
        else {
            row++;
            if (row >= list->count())
                row = 0;
        }
        list->setCurrentRow(row);
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
    input = new SuggestLineEdit(this);
    button = new QPushButton("Enter", this);
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

                case Qt::Key_Right:
                    onRightDown.Invoke();
                    return shift_;
                case Qt::Key_Left:
                    onLeftDown.Invoke();
                    return shift_;
                case Qt::Key_Up:
                    onUpDown.Invoke();
                    return true;
                case Qt::Key_Down:
                    onDownDown.Invoke();
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
    return false;
}
void MainWindow::LogLine(std::string logged) {
    auto l = Core::ToQString(logged);

    if (HasLineHeadInLastLine()) {
        log->moveCursor(QTextCursor::End);
        log->insertPlainText(l);
        return;
    }

    log->appendPlainText(l);
}

void MainWindow::LogLineHead() {
    LogLine(Sets::GetLineHead());
}

bool MainWindow::HasLineHeadInLastLine() {
    auto s = log->toPlainText();
    if (!s.isEmpty()) {
        auto last = s.back();
        if (last == '>') {
            return true;
        }
    }
    return false;
}

void MainWindow::RemoveLastLine() {
    QTextCursor cursor = log->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.removeSelectedText();
}

void MainWindow::RemoveLineHeadWeatherExists() {
    if (HasLineHeadInLastLine())
        RemoveLastLine();
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

void MainWindow::SetList(std::vector<std::string> elements) {
    list->clear();
    for (auto element : elements) {
        list->addItem(Core::ToQString(element));
    }
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

void MainWindow::SetInputSuggestion(Branch* node, std::vector<std::string> args, int listCount, std::string last) {
    std::string sug = "";
    auto add =  [&sug](std::string t) {
        sug += sug == "" ? t : "/" + t;
    };

    bool branchnotfound = !last.empty() && listCount == 0;
    int count = args.size();
    bool brancharg = node->args.size() >0;

    if (branchnotfound) {//not found branches
        if (!brancharg)
            add("branch not found");
    }
    else if (node->branches.size() > 0) {
        add("branch");

    }
    if (!node->args.empty() && count  < (int) node->args.size())
        add(node->args[count].arg);

    sug = "<" + sug + ">";
    input->setSuggestion(Core::ToQString(sug));
}


void SuggestLineEdit::setSuggestion(const QString& text)
{
    suggestion = text;
    update();
}

void SuggestLineEdit::paintEvent(QPaintEvent* event)
{
    QLineEdit::paintEvent(event);

    if (suggestion.isEmpty() || !hasFocus())
        return;

    QPainter painter(this);
    painter.setPen(QColor(160, 160, 160));

    const QFontMetrics fm(font());
    const QString beforeCursor = text().left(cursorPosition());

    const int x = 8 + fm.horizontalAdvance(beforeCursor);
    const int y = (height() + fm.ascent() - fm.descent()) / 2;

    painter.drawText(x, y, suggestion);
}
void MainWindow::showAndActivate() {
    // 如果窗口最小化，则恢复正常状态
    if (isMinimized()) {
        showNormal();
    } else {
        show();
    }

    // 将窗口提升到前台并请求键盘焦点
    raise();
    activateWindow();
}