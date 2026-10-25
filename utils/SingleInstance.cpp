#include "SingleInstance.h"

#include <QLocalSocket>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>

SingleInstance::SingleInstance(
    const QString& serverName,
    QObject* parent
)
    : QObject(parent),
      m_serverName(serverName)
{
    connect(
        &m_server,
        &QLocalServer::newConnection,
        this,
        &SingleInstance::handleNewConnection
    );
}

bool SingleInstance::sendCommand(const QString& command)
{
    QLocalSocket socket;
    socket.connectToServer(m_serverName);

    if (!socket.waitForConnected(500)) {
        return false;
    }

    const QByteArray message = command.toUtf8() + '\n';

    if (socket.write(message) == -1) {
        return false;
    }

    if (!socket.waitForBytesWritten(1000)) {
        return false;
    }

    socket.disconnectFromServer();
    return true;
}

bool SingleInstance::sendCommandWithResponse(const QString& command,
                                             QByteArray& response,
                                             int timeoutMs)
{
    QLocalSocket socket;
    socket.connectToServer(m_serverName);
    if (!socket.waitForConnected(500)) return false;

    if (socket.write(command.toUtf8() + '\n') == -1) return false;
    if (!socket.waitForBytesWritten(1000)) return false;

    if (!socket.waitForReadyRead(timeoutMs)) {
        socket.disconnectFromServer();
        return false;   // 超时
    }
    response = socket.readAll().trimmed();
    socket.disconnectFromServer();
    return true;
}

SingleInstance::Result SingleInstance::start(bool showExisting)
{
    const QString command = showExisting ? "SHOW" : "PING";

    // 已有实例正在运行，直接通知它。
    if (sendCommand(command)) {
        return Result::Existing;
    }

#ifndef Q_OS_WIN
    QLocalServer::removeServer(m_serverName);   // 清理崩溃残留
#endif

    // 尝试成为主实例。
    if (m_server.listen(m_serverName)) {
        return Result::Primary;
    }

    // listen 失败后再次尝试连接，避免启动竞争时误判。
    if (sendCommand(command)) {
        return Result::Existing;
    }

    qCritical() << "Cannot create local server:"
                << m_server.errorString()
                << "Server name:" << m_serverName;

    return Result::Error;
}

void SingleInstance::handleNewConnection()
{
    while (m_server.hasPendingConnections()) {
        QLocalSocket* client = m_server.nextPendingConnection();
        if (!client) continue;

        connect(client, &QLocalSocket::readyRead, this, [this, client]() {
            while (client->canReadLine()) {
                const QByteArray line = client->readLine().trimmed();
                if (line.isEmpty()) continue;

                if (line == "SHOW") {
                    emit showRequested();
                    client->disconnectFromServer();
                } else if (line == "QUIT") {
                    emit quitRequested();
                    client->disconnectFromServer();
                } else if (line.startsWith("EXEC:")) {
                    // 格式：EXEC:<method>\x1f<json-array>
                    const QByteArray body = line.mid(5);
                    const int sep = body.indexOf('\x1f');
                    const QString method = QString::fromUtf8(body.left(sep));

                    QStringList args;
                    if (sep >= 0) {
                        const auto arr =
                            QJsonDocument::fromJson(body.mid(sep + 1)).array();
                        for (const auto& v : arr) args << v.toString();
                    }

                    // 同步分派并回写结果
                    const QString result =
                        m_execHandler
                            ? m_execHandler(method, args)
                            : QStringLiteral("error: no exec handler");

                    client->write(result.toUtf8() + '\n');
                    client->waitForBytesWritten(1000);
                    client->disconnectFromServer();
                } else {
                    // 未知命令，直接断开避免悬挂
                    client->disconnectFromServer();
                }
            }
        });

        connect(
            client,
            &QLocalSocket::disconnected,
            client,
            &QObject::deleteLater
        );
    }
}