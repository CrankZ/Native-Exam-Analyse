#include "receiver.h"

Receiver::Receiver(QObject *parent) : QObject(parent)
{
    udpSocketRecv = new QUdpSocket(this);
    udpSocketRecv->bind(EXAMPORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocketRecv,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
}

void Receiver::processPendingDatagram()
{
    QByteArray datagram;
    while (udpSocketRecv->hasPendingDatagrams())
    {
        datagram.resize(udpSocketRecv->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocketRecv->readDatagram(
                    datagram.data(),
                    datagram.size(),
                    &sender,
                    &senderPort);

        QByteArray qUncom = qUncompress(datagram);

        QDataStream in(&qUncom, QIODevice::ReadOnly);//因为其属性为只读，所以是输入

        int msgType;
        in >> msgType;    //读取1个32位长度的整型数据到messageTyep中


        switch(msgType)
        {
        // 服务端最好还是确认一下发过来的消息，告诉他收到的。
        // 如果学生机没收到，则说明没收到，则继续发送
        case CM_sExam:
        {
//            SendInfo *sendInfo = new SendInfo();
//            in >> sendInfo;
//            mapSi.insert(sendInfo->getStuNo(),sendInfo);//为了防止重复，所以采用map容器，并且用MAC作为key
//            qDebug() << "mapSi.count():" << mapSi.count();
//            emit updateStuExamInfoSignal(sendInfo);


            ExamInfo *examInfo = new ExamInfo();
            in >> examInfo;
            // 为了防止重复，所以采用map容器，并且用MAC作为key
//            examInfoMap.insert(examInfo->stuMAC,examInfo);
//            qDebug() << "examInfoMap.count():" << examInfoMap.count();
            emit updateStuExamInfoSignal(examInfo);
            break;
        }
            break;
        }
    }
}
