#include "udp_socket.h"

/**
 * @file        udp_socket.cpp
 * @brief       负责通过UDP协议进行数据通信
 * @authors     YangWeiBin
 * @copyright   Gmagii
 * @version     v0.0.1
 * @date        2020-11-06 15:30:58
 */

/**
 * @brief 构造函数:      设置网络通信的属性信息
 * @param sendIP:       需要发送到的计算机的IP地址(目的地址)
 * @param sendPort:     需要发送到的计算机的Port号(目的端口号)
 * @param receiveIP:    需要监听的本地计算机的IP地址(本机IP)
 * @param receivePort:  需要监听的本地计算机的IP地址(本机端口号)
 * @param parent:       基类指针
 */
UDPSocket::UDPSocket(QString sendIP, QString sendPort,
                     QString receiveIP,  QString receivePort, QObject *parent) :
                     QObject(parent),m_str_QDIP(sendIP), m_str_QDPort(sendPort),
                     m_str_MMJKIP(receiveIP),m_str_MMJKPort(receivePort) {
    m_pudpsocket_send = new QUdpSocket;
    m_pudpsocket_receive = new QUdpSocket;
    m_pudpsocket_receive->bind(QHostAddress(m_str_MMJKIP), m_str_MMJKPort.toInt());
    connect(m_pudpsocket_receive, SIGNAL(readyRead()), this, SLOT(UDPreceive()));
}


/**
 * @brief  监听本机IP和端口
 * @return 返回接收到的字节流
 */
QByteArray UDPSocket::UDPreceive()
{
    QByteArray ByteArr_receive;
    while(m_pudpsocket_receive->hasPendingDatagrams())  {
        ByteArr_receive.resize(m_pudpsocket_receive->pendingDatagramSize());
        m_pudpsocket_receive->readDatagram(ByteArr_receive.data(), ByteArr_receive.size());
        // qDebug() <<  ByteArr_receive.data();
    }
    return ByteArr_receive;
}

/**
 * @brief 发送字节流byteArr
 * @param byteArr: 要发送的字节流
 */
void UDPSocket::UDPsend(QByteArray byteArr) {
    m_pudpsocket_send->writeDatagram(byteArr, (QHostAddress)(m_str_QDIP),  m_str_QDPort.toInt());
}
