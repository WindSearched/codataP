#include <fstream>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QProcessEnvironment>
#include <QDir>
#include <QLocalSocket>

#include <iostream>
#include <string>

#include "codes/Tube.h"
#include "codes/Branch.h"
#include "codes/Core.h"
#include "codes/Data.h"
#include "codes/MainWindow.h"
#include "codes/branches/math.h"
#include "utils/SingleInstance.h"

// === 在这里添加 Windows 特定的头文件引入 ===
#ifdef Q_OS_WIN
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 // 确保启用 GetConsoleWindow (Windows XP 及以上)
#endif
#include <windows.h>
#else
#include <unistd.h>
#endif

static bool launchedFromTerminal()
{
#ifdef Q_OS_WIN
    return GetConsoleWindow() != nullptr;   // 需 #include <windows.h>
#else
    return isatty(STDIN_FILENO);            // 需 #include <unistd.h>
#endif
}
// 纯本地执行：把 method + args 交给 Core::branch，返回结果字符串
// 注意：不依赖 MainWindow、不碰 SingleInstance
QString runLocalCommand(const QString& method, const QStringList& args)
{
    try {
        std::vector<std::string> split;
        split.push_back(Core::ToString(method));
        for (const auto& a : args) split.push_back(Core::ToString(a));
        return Core::ToQString(Core::branch.Command(split));
    } catch (const std::exception& e) {
        return QString("error: %1").arg(e.what());
    }
}

void registerBranches() {
    Core::branch
    .AddBranch(
        Branch{"test"}
            .AddExecution(
                [](CommandArgument args) -> std::string {
                    return "This is the test branch of Codata program";
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
    )
    .AddBranch(math::math_)
    .AddBranch(Data::branch)
    ;
}
int start(QApplication& app, MainWindow& window) {

    //try load tube cache
    Core::OnStart.Add([&window] {
        auto save = Data::sets["saveTube"];
        if (!save) return;

        auto cache = Data::sets["tubeCache"];
        if (!cache) return;

        auto& t = window.tubeCache;
        const auto list = cache.as<std::vector<std::string>>();

        t.Put(list);
    });
    Core::OnStop.Add([&window] {
        auto save = Data::sets["saveTube"];
        if (!save) return;

        Data::sets["tubeCache"] = window.tubeCache.tube.tube;
    });

    //close yaml
    Core::OnStop.Add([] {
        std::ofstream fout(Data::setsPath);
        fout << Data::sets;
        fout.close();
    });

    qInfo() << "Before Core::OnStart";
    std::cerr << "Before Core::OnStart" << std::endl;
    Core::OnStart.Invoke();

    std::cerr << "Before app.exec()" << std::endl;
    const int result = app.exec();

    std::cerr << "After app.exec()" << std::endl;
    Core::OnStop.Invoke();
}
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QApplication::setApplicationName("Codata");
    QApplication::setOrganizationName("WS");

    // 关闭窗口后继续在后台运行（注意：如需真正后台常驻，
    // 应结合 MainWindow 的关闭策略，这里保持原状）
    app.setQuitOnLastWindowClosed(true);

    // 待执行的 exec（本进程成为主实例时使用）
    bool hasPendingExec = false;
    QString pendingExecMethod;
    QStringList pendingExecArgs;

    // ---- 命令行参数 ----
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

    parser.addPositionalArgument(
        "command",
        "Command: close | exec <method> [args...]",
        "[close|exec]"
    );

    parser.process(app);

    bool startHidden = parser.isSet(hiddenOption);
    const QStringList arguments = parser.positionalArguments();

    const QString serverName = "Codata_WS_instance";

    SingleInstance instance(serverName);

    // =========================================================
    // ★ debug：不创建服务器，不碰 IPC
    //    - 无参数  → 前台打开 MainWindow，app.exec() 常驻
    //    - 带参数  → 纯本地执行命令，不弹窗，立刻返回
    // =========================================================
    if (!arguments.isEmpty() && arguments.first() == "debug") {

        // ---- 情况 1：无参数 → 打开窗口 ----
        if (arguments.size() == 1) {
            registerBranches();          // 无 GUI 依赖的分支

            MainWindow mainWindow;

            mainWindow.show();
            std::cerr << "[CodataP debug] Window shown. Ctrl+C to quit." << std::endl;

            return start(app, mainWindow);               // 常驻，调试完 Ctrl+C
        }

        // ---- 情况 2：带参数 → 纯本地执行，不开窗 ----
        const QString method = arguments.at(1);
        const QStringList args = arguments.mid(2);

        registerBranches();

        const QString r = runLocalCommand(method, args);
        std::cout << r.toStdString() << std::endl;
        return 0;                            // 立刻返回，不卡终端
    }

    // =========================================================
    // ① close：请求已有实例退出
    // =========================================================
    if (!arguments.isEmpty() && arguments.first() == "close") {
        if (instance.sendCommand("QUIT")) {
            qInfo() << "Quit request sent.";
        } else {
            qInfo() << "No running instance found.";
        }
        return 0;
    }

    // =========================================================
    // ② exec <method> [args...]
    // =========================================================
    if (!arguments.isEmpty() && arguments.first() == "exec") {
        if (arguments.size() < 2) {
            qWarning() << "用法: codata exec <method> [args...]";
            return 1;
        }

        const QString method = arguments.at(1);

        // 其余参数原样透传
        QJsonArray arr;
        for (int i = 2; i < arguments.size(); ++i) arr.append(arguments.at(i));
        const QByteArray argsJson =
            QJsonDocument(arr).toJson(QJsonDocument::Compact);

        // 协议：EXEC:<method>\x1f<json>
        const QString cmd = "EXEC:" + method
                          + QChar(0x1f)
                          + QString::fromUtf8(argsJson);

        // 后台执行，不弹窗
        startHidden = true;

        // ① 已有实例？发过去并等结果
        QByteArray response;
        if (instance.sendCommandWithResponse(cmd, response)) {
            std::cout << response.constData() << std::endl;
            return 0;
        }

        // ② 无实例（或超时）→ 本地创建并执行
        qInfo() << "No instance running; will create one and exec locally.";
        hasPendingExec = true;
        pendingExecMethod = method;
        pendingExecArgs = arguments.mid(2);
        // 不 return，继续到 detach 判定 / instance.start()
    }

    // =========================================================
    // ③ detach 判定：必须在 instance.start() 之前！
    //    此时本进程还没有 listen，探测到的才是"别的实例"
    // =========================================================
    if (!qEnvironmentVariableIsSet("COD.._CHILD") && launchedFromTerminal())  {
        QLocalSocket probe;
        probe.connectToServer(serverName);
        const bool hasInstance = probe.waitForConnected(300);
        probe.abort();

        if (!hasInstance) {
            // 无实例 → detached 启动自己，父进程立刻返回，终端不卡
            QProcess p;
            auto env = QProcessEnvironment::systemEnvironment();
            env.insert("COD.._CHILD", "1");
            p.setProcessEnvironment(env);
            p.setProgram(QCoreApplication::applicationFilePath());

            QStringList childArgs;
            if (startHidden) childArgs << "--hidden";

            // exec 竞态落到本地：把 method + args 传给子进程
            // 注意：这里必须在 hasPendingExec 被清空之前读取！
            if (hasPendingExec) {
                childArgs << "exec" << pendingExecMethod << pendingExecArgs;
            }

            p.setArguments(childArgs);
            p.setWorkingDirectory(QDir::currentPath());

            if (p.startDetached()) {
                qInfo() << "Background instance started.";
                return 0;          // ★ 终端立刻返回
            }
            qWarning() << "startDetached failed; running in foreground.";
        }
        // hasInstance == true → 继续到 ④
        // instance.start() 会发 SHOW，返回 Existing，本进程随即退出
    }

    // =========================================================
    // ④ instance.start()
    // =========================================================
    const auto instanceResult = instance.start(!startHidden);

    if (instanceResult == SingleInstance::Result::Existing) {
        // 竞态补发（exec 分支没抢到实例，这里补一次）
        if (hasPendingExec) {
            QJsonArray arr;
            for (const auto& a : pendingExecArgs) arr.append(a);
            const QByteArray argsJson =
                QJsonDocument(arr).toJson(QJsonDocument::Compact);
            const QString cmd = "EXEC:" + pendingExecMethod
                              + QChar(0x1f)
                              + QString::fromUtf8(argsJson);
            QByteArray response;
            if (instance.sendCommandWithResponse(cmd, response)) {
                std::cout << response.constData() << std::endl;
            }
        }
        return 0;
    }

    if (instanceResult == SingleInstance::Result::Error) {
        qCritical() << "Failed to start Codata.";
        return 1;
    }

    // =========================================================
    // ⑤ 以下只由主实例执行
    //    （detached 的子进程，或 COD.._CHILD=1 调试时前台进程）
    // =========================================================

    // ---- 初始化命令系统 ----
    static Tube<std::string> tube;

    registerBranches();

    MainWindow mainWindow;

    // ---- exec 分派：命令树即白名单 ----
    auto dispatchExec = [](const QString& method,
                                      const QStringList& args) -> QString {
        std::vector<std::string> split;
        split.push_back(Core::ToString(method));
        for (const auto& arg : args) {
            split.push_back(Core::ToString(arg));
        }
        auto put = Core::branch.Command(split);
        return Core::ToQString(put);
    };

    instance.setExecHandler(dispatchExec);

    // ---- 信号连接 ----
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

    QObject::connect(
        &instance,
        &SingleInstance::quitRequested,
        &app,
        &QCoreApplication::quit
    );

    // ---- 窗口显示 ----
    if (!startHidden) {
        std::cerr << "[Codata] Showing main window" << std::endl;
        mainWindow.show();
    } else {
        std::cerr << "[Codata] Starting hidden" << std::endl;
    }

    // ---- 本地 pending exec（子进程真正执行的那次） ----
    if (hasPendingExec) {
        const QString method = pendingExecMethod;
        const QStringList args = pendingExecArgs;
        hasPendingExec = false;

        qInfo() << "Executing pending method locally:" << method;

        QTimer::singleShot(0, &mainWindow,
            [dispatchExec, method, args]() {
                const QString r = dispatchExec(method, args);
                std::cout << r.constData() << std::endl;
            });
    }

    int result = start(app, mainWindow);
    return result;
}
