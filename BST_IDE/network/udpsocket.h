#ifndef NODEUDP_H
#define NODEUDP_H

#include "nodebase.h"
#include <QUdpSocket>

class NodeUdp : public PhysicalDevice
{
    Q_OBJECT
public:
    NodeUdp(QObject *parent = 0);
    ~NodeUdp();

    bool OpenUdp(quint16 pPort);
    void Close();
    bool isOpen() const {return true;}
    bool IsLocalIP(QHostAddress pIP, quint16 pPort)
    {
        QStringList localIPList = GetLocalIPList();
        for(int i=0;i<localIPList.count();i++)
        {
            QHostAddress localIP = QHostAddress(localIPList.at(i));
            if(localIP == pIP)
                return true;
        }
        return false;
    }

    qint64 write(QByteArray data, QHostAddress host);
    qint64 write(const QByteArray &data);   //>@�㲥����
    qint64 reply(const QByteArray &data);
    qint64 echo();
    QByteArray readAll();
public:
    quint16             mPort;
    QUdpSocket*         mUdpSocket;

    QByteArray          m_RevByteArray;
    QHostAddress        m_SenderHost; //>@��ǰ���ݵķ��ͷ���IP
    quint16             m_SenderPort; //>@��ǰ���ݵķ��ͷ���PORT
};

#endif // NODEUDP_H
