#include "sender.h"


Sender::Sender()
{
    udp = new QUdpSocket;
}

void Sender::sendCommand(MessageType msgType,QString msg)
{
    QByteArray data;    //字节数组
    //QDataStream类是将序列化的二进制数据送到io设备，因为其属性为只写
    QDataStream out(&data, QIODevice::WriteOnly);//out为待发送

    out << msgType;

    //一下都只是把数据存在out里
    //但是out又在data中
    //所以最后发送的还是data
    switch(msgType)
    {
        case CM_tExamStart:
        {
            out << msg;
            break;
        }
    }
    QByteArray com_qba = qCompress(data);
    udp->writeDatagram(com_qba,com_qba.length(),QHostAddress::Broadcast, PORT);
//    udp->waitForBytesWritten();
}

void Sender::sendExamInfo(MessageType msgType, ExamInfo *ei)
{
    QByteArray data;    //字节数组
    //QDataStream类是将序列化的二进制数据送到io设备，因为其属性为只写
    QDataStream out(&data, QIODevice::WriteOnly);//out为待发送

    out << msgType;

    //一下都只是把数据存在out里
    //但是out又在data中
    //所以最后发送的还是data
    switch(msgType)
    {
        case CM_tExamPaperInfo:
        {
            out << ei;
            break;
        }
    }
    QByteArray com_qba = qCompress(data);
    udp->writeDatagram(com_qba,com_qba.length(),QHostAddress::Broadcast, PORT);
//    udp->waitForBytesWritten();
}

void Sender::sendConfirm(MessageType msgType, QMap<QString, bool> confirmStuMap)
{
    QByteArray data;    //字节数组
    //QDataStream类是将序列化的二进制数据送到io设备，因为其属性为只写
    QDataStream out(&data, QIODevice::WriteOnly);//out为待发送

    out << msgType;

    //一下都只是把数据存在out里
    //但是out又在data中
    //所以最后发送的还是data
    switch(msgType)
    {
        case CM_tExamConfirm:
        {
            out << confirmStuMap;
            break;
        }
    }
    QByteArray com_qba = qCompress(data);
    udp->writeDatagram(com_qba,com_qba.length(),QHostAddress::Broadcast, PORT);
//    udp->waitForBytesWritten();
}
