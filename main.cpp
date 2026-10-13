#include <iostream>
#include <QApplication>

#include "codes/Branch.h"
#include "codes/Core.h"
#include "codes/Event.h"
#include "codes/SEvent.h"
#include "codes/Tube.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QPushButton>

#include <string>

#include "codes/MainWindow.h"

void Windowing(QWidget& window)
{
    window.resize(800, 400);
    window.setWindowTitle("codataP");

    auto* layout = new QVBoxLayout(&window);
    auto* up = new QHBoxLayout();
    auto* down = new QHBoxLayout();

    auto* logBox = new QPlainTextEdit(&window);
    up->addWidget(logBox, 3);
    logBox-> setReadOnly(true);
    logBox->resize(510,267);
    logBox->move(40,40);

    auto* listBox = new QListWidget(&window);
    up->addWidget(listBox, 1);
    listBox->addItem("Branch 1");
    listBox->addItem("Branch 2");
    listBox->addItem("Branch 3");

    layout->addLayout(up);

    auto* inputBox = new QLineEdit(&window);
    down->addWidget(inputBox, 9);

    auto* button = new QPushButton(&window);
    down -> addWidget(button, 1);

    layout->addLayout(down);


}

int main(int argc, char *argv[]) {
    static Tube<std::string> tube;
    static SEvent<> OnStart, OnStop;
    static Branch& branch = Core::branch;


    Core::branch.AddBranch(
        Branch{"test"}
            .AddExecution([](CommandArgument args)-> std::string{
                return "d";
            })
        );

    OnStart.Add([] {
        std::cout << "OnStart" << std::endl;
    });



    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    OnStart.Invoke();

    auto exec = app.exec();

    OnStop.Invoke();

    return exec;


}
