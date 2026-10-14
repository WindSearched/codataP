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
int main(int argc, char *argv[]) {
    static Tube<std::string> tube;
    static Branch& branch = Core::branch;


    Core::branch
        .AddBranch(
        Branch{"test"}
            .AddExecution([](CommandArgument args)-> std::string{
                return "d";
            })
        )
        .AddBranch(
            Branch{"print"}
                .AddArgument(BranchArgument{"output"})
                .AddExecution([](CommandArgument args)-> std::string {
                    std::string o;
                    if (args.TryGet("output", o)) {
                        return o;
                    }
                    return "the argument 'output' is not valid";
                })
            )
    ;


    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    Core::OnStart.Invoke();

    auto exec = app.exec();

    Core::OnStop.Invoke();

    return exec;


}
