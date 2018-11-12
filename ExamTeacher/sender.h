#ifndef Sender_H
#define Sender_H

#include <QUdpSocket>
#include <QNetworkInterface>
#include <QDataStream>

#include "justenum.h"
#include "examinfo.h"

class Sender : public QObject
{
    Q_OBJECT

public:
    Sender();
    void sendCommand(MessageType msgType,QString msg = "");
    void sendExamInfo(MessageType msgType, ExamInfo *ei);
    void sendConfirm(MessageType msgType, QMap<QString, bool> confirmStuMap);
private:
    QUdpSocket *udp;

};

#endif // Sender_H
