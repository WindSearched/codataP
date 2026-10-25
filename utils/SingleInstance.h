#pragma once

#include <QObject>
#include <QLocalServer>
#include <QString>

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

    // 向正在运行的实例发送指令
    bool sendCommand(const QString& command);
    bool sendCommandWithResponse(const QString& command, QByteArray& response, int timeoutMs = 3000);

    signals:
        void showRequested();
        void quitRequested();
    void execRequested(const QString& method, const QStringList& args);
    using ExecHandler = std::function<QString(const QString& method, const QStringList& args)>;
    void setExecHandler(ExecHandler handler) { m_execHandler = std::move(handler); }
private:
    ExecHandler m_execHandler;
    void handleNewConnection();

    QString m_serverName;
    QLocalServer m_server;
};