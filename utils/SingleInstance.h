#pragma once

#include <QLocalServer>
#include <QString>
#include <QStringList>
#include <functional>

class SingleInstance : public QObject
{
    Q_OBJECT

public:
    enum class Result {
        Primary,
        Existing,
        Error
    };

    explicit SingleInstance(
        const QString& serverName,
        QObject* parent = nullptr
    );

    Result start(bool showExisting = true);

    // 向正在运行的实例发送指令（fire-and-forget）
    bool sendCommand(const QString& command);

    // 发送并等待一行响应（用于 EXEC）
    bool sendCommandWithResponse(const QString& command,
                                 QByteArray& response,
                                 int timeoutMs = 3000);

    // exec 处理回调：输入方法名 + 参数，返回结果字符串
    using ExecHandler =
        std::function<QString(const QString& method, const QStringList& args)>;

    void setExecHandler(ExecHandler handler) {
        m_execHandler = std::move(handler);
    }

signals:
    void showRequested();
    void quitRequested();

private:
    void handleNewConnection();

    QString m_serverName;
    QLocalServer m_server;
    ExecHandler m_execHandler;
};