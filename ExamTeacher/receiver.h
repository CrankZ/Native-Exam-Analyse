#ifndef Receiver_H
#define Receiver_H

#include <QUdpSocket>

#include "justenum.h"
#include "examinfo.h"

class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = nullptr);

public:
    // 所有学生考试数据
    // 目前用MAC作为key
    QMap<QString, ExamInfo*> examInfoMap;

private:
    QUdpSocket * udpSocketRecv;
    int i = 0;

private slots:
    void processPendingDatagram();
signals:
    void updateStuExamInfoSignal(ExamInfo*);
};

#endif // Receiver_H
