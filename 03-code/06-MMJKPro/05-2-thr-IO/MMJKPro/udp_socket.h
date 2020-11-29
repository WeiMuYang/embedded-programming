#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include <QUdpSocket>

class UDPSocket : public QObject
{
    Q_OBJECT
public:
    explicit UDPSocket(QString sendIP, QString sendPort,
                       QString receiveIP,  QString receivePort,
                       QObject *parent = 0);

    void setSendIPPort(QString IP, QString Port);
    void setReceiveIPPort(QString IP, QString Port);

signals:

public slots:
    QByteArray UDPreceive();
    void UDPsend(QByteArray byteArr);
private:
    QUdpSocket *m_pudpsocket_send;
    QUdpSocket *m_pudpsocket_receive;
    const QString m_str_QDIP;
    const QString m_str_QDPort;
    const QString m_str_MMJKIP;
    const QString m_str_MMJKPort;
};

#endif // UDPSOCKET_H
