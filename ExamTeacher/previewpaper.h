#ifndef PreviewPaper_H
#define PreviewPaper_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QFile>
#include <QMessageBox>
#include <QLabel>
#include <QRadioButton>
#include <QDebug>
#include <QGroupBox>
#include <QSplitter>
#include <QUdpSocket>
#include <QScrollArea>
#include <QtNetwork>
#include <QDesktopWidget>
#include <QCloseEvent>
#include <QPlainTextEdit>

#include "justenum.h"
#include "sendinfo.h"
#include "examinfo.h"
#include "sender.h"

namespace Ui {
class PreviewPaper;
}

class PreviewPaper : public QMainWindow
{
    Q_OBJECT

public:
    explicit PreviewPaper(QWidget *parent = 0);
    ~PreviewPaper();

public:
    void loadChoice(QString type, QString str);
    void loadCompletion(QString type, QString str);

    QWidget *wgNothing;
    QScrollArea *scrollArea;

private:
    Ui::PreviewPaper *ui;



    QMap<QString, QList<QSplitter*>> lSpCompletionMap;


    QMap<QString, QList<QSplitter*>> lSpMap;


    // 这三个目前来讲，还必须要全局变量
    // 因为有等多个content的话，还是要重复存取
    QList<QLabel*> lLbContent;
    QMap<QString, QList<QLabel*>> lLbContentMap;
    QList<QPlainTextEdit*> lPlaintTextEdit;
    QMap<QString, QList<QPlainTextEdit*>> lPlaintTextEditMap;
    QList<QSplitter*> lSp;
    int titleNum = 0;


    QMap<int, QString> rightAns;

    QMap<QString, QMap<int, QString>> rightAnsChoiceMap;

    QMap<int, QString> realAns;
    QMap<int, bool> realAnsIsRight;

//    QMap<int, QString> rightAnsCom;
    QMap<int, QMap<int, QString>> rightAnsCom; // 一个填空题里面可能有多个问题，所以外层还要一个Map来包裹

    QMap<QString, QMap<int, QMap<int, QString>>> rightAnsComMap;
    QMap<int, bool> realAnsIsRightCom;

    QUdpSocket *udp;
    SendInfo *si;
    QVBoxLayout *vbox;


    QTimer *qtime;
    int timeStartNum;
    bool isReady;
    int hour = -1;
    int min = -1;
    int sec = -1;

    ExamInfo *examInfo;


    int totalScoreChoice = 0;
    int totalScoreCompletion = 0;

    QList<int> randomNumList(int maxNum);

    int offset = 0; // 这个是选择题选项的数量，如果这个为0.那么就是4个选项，如果为1那么就是5个选择题，以此类推
    int choiceTitleNum = 0;
    int wxtkTitleNum = 0;   // 完形填空
    int compTitleNum = 0;

    QMap<QString, int> titleNumMap;

    QString justNextContent = "";
    QString JustRadio;

    QString justRadioAndAns;
    QString nextContextAndRadio;
    Sender *sender = new Sender;
private:
    bool regExIsContainsChinese(QString str);
    bool regExIsContainsEnglish(QString str);
    bool regExIsContainsNumber(QString str);


    void setRealAnsIsRightCom();

    bool savePaper(QString saveName, QString paper);
    void setSpDisable();

    void normalAddWidget(QString type);
    void randomAddWidget();

    QByteArray encodeStr(QString strSource);
    QString decodeBa(QByteArray baEncode);
    QString decodeStr(QString strEncode);

    ////////////////////
    int choiceNum(QString fileName);



    void addChoiceRightAns(QString type, QString str);

    void checkAnsIsRightChoice(QString type);
    void checkAnsIsRightChoiceAll();
    void checkAnsIsRightCompletion(QString type);
    void checkAnsIsRightCompletionAll();


    void selectRigthAnsChoice(QString type);
    void selectRightAnsCom(QString type);
    void selectRightAnsComAll();

    void makeJustRadio(QString strAll);
    void makeContent(QString type, int i, QString strAll);
    void randomSelect();

    int choiceNum2(QString strAll);
    void makeWidget(QString type, QString str);
    void reformStr(QString split, QString &str);

};

#endif // PreviewPaper_H
