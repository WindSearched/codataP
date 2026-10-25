#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QTimer>

#include <string>

#include "codes/Tube.h"
#include "codes/Branch.h"
#include "codes/Core.h"
#include "codes/MainWindow.h"
#include "utils/SingleInstance.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("Codata");
    QApplication::setOrganizationName("WS");

    // 关闭窗口后继续在后台运行
    app.setQuitOnLastWindowClosed(true);

    QString pendingExecMethod;

    // 命令行参数
    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Codata single-instance application"
    );
    parser.addHelpOption();

    QCommandLineOption hiddenOption(
        QStringList() << "hidden",
        "Start without showing the main window"
    );

    parser.addOption(hiddenOption);

    // 支持：codataP close
    parser.addPositionalArgument(
        "command",
        "Command: close",
        "[close]"
    );

    parser.process(app);

    bool startHidden = parser.isSet(hiddenOption);
    const QStringList arguments = parser.positionalArguments();

    const QString serverName =
        "Codata_WS_instance";

    SingleInstance instance(serverName);

    // close：请求已有实例退出，不启动新实例
    if (!arguments.isEmpty() &&
        arguments.first() == "close") {

        if (instance.sendCommand("QUIT")) {
            qInfo() << "Quit request sent.";
        } else {
            qInfo() << "No running instance found.";
        }

        return 0;
    }

    // ---- 新增：exec 命令 ----
    if (!arguments.isEmpty() && arguments.first() == "exec") {
        if (arguments.size() < 2) {
            qWarning() << "exec 需要一个方法名，例如: codata exec refresh";
            return 1;
        }

        const QString method = arguments.at(1);

        // 后台执行，不弹窗
        startHidden = true;

        // ① 先尝试交给已有实例
        if (instance.sendCommand("EXEC:" + method)) {
            qInfo() << "Exec request sent:" << method;
            return 0;
        }

        // ② 没有实例 → 走到下方 start()，本进程成为主实例
        qInfo() << "No instance running; will create one and exec locally.";
        pendingExecMethod = method;

        // 注意：不 return，让流程继续到 instance.start()
    }
    // ------------------------

    // 检查是否已有实例
    const auto instanceResult = instance.start(!startHidden);

    if (instanceResult == SingleInstance::Result::Existing) {
        if (!pendingExecMethod.isEmpty()) {
            instance.sendCommand("EXEC:" + pendingExecMethod);   // 竞态补发
        }
        return 0;
    }

    if (instanceResult == SingleInstance::Result::Error) {
        qCritical() << "Failed to start Codata.";
        return 1;
    }

    // 以下只由主实例执行

    // 初始化命令系统
    static Tube<std::string> tube;
    static Branch& branch = Core::branch;

    Core::branch
        .AddBranch(
            Branch{"test"}
                .AddExecution(
                    [](CommandArgument args) -> std::string {
                        return "d";
                    }
                )
        )
        .AddBranch(
            Branch{"print"}
                .AddArgument(BranchArgument{"output"})
                .AddExecution(
                    [](CommandArgument args) -> std::string {
                        std::string output;

                        if (args.TryGet("output", output)) {
                            return output;
                        }

                        return "the argument 'output' is not valid";
                    }
                )
        );

    MainWindow mainWindow;

    // 接收到 SHOW 请求时显示窗口
    QObject::connect(
        &instance,
        &SingleInstance::showRequested,
        &mainWindow,
        [&mainWindow]() {
            qInfo() << "Received SHOW command";
            mainWindow.show();
            mainWindow.showNormal();
            mainWindow.raise();
            mainWindow.activateWindow();
            qInfo() << "Window visible:" << mainWindow.isVisible();
        }
    );

    // 接收到 QUIT 请求时正常退出事件循环
    QObject::connect(
        &instance,
        &SingleInstance::quitRequested,
        &app,
        &QCoreApplication::quit
    );

    QObject::connect(
    &instance,
    &SingleInstance::execRequested,
    &mainWindow,
    [&mainWindow](const QString& method) {
        qInfo() << "Received EXEC:" << method;

        // 白名单映射，避免任意方法调用
        const QHash<QString, std::function<void()>> kMethods = {

        };

        auto it = kMethods.find(method);
        if (it != kMethods.end()) {
            it.value()();
            }
        else {
            qWarning() << "Unknown exec method:" << method;
            }
        }
    );

    if (!startHidden) {
        std::cerr << "[Codata] Showing main window" << std::endl;
        mainWindow.show();
    } else {
        std::cerr << "[Codata] Starting hidden" << std::endl;
    }

    //Core::OnStart.Invoke();

    // const int result = app.exec();
    //
    // // 正常退出时执行
    // Core::OnStop.Invoke();
    //
    // return result;

    if (!pendingExecMethod.isEmpty()) {
        const QString method = pendingExecMethod;
        pendingExecMethod.clear();
        qInfo() << "Executing pending method locally:" << method;

        // 延迟到事件循环启动后执行，避免构造期未就绪
        QTimer::singleShot(0, &mainWindow, [&mainWindow, method]() {
            if (method == "refresh")       mainWindow.refresh();
            else if (method == "doSomething") { /* ... */ }
            else qWarning() << "Unknown exec method:" << method;
        });
    }

    qInfo() << "Before Core::OnStart";
    std::cerr << "Before Core::OnStart" << std::endl;
    Core::OnStart.Invoke();

    std::cerr << "Before app.exec()" << std::endl;
    const int result = app.exec();

    std::cerr << "After app.exec()" << std::endl;
    Core::OnStop.Invoke();

    return result;
}