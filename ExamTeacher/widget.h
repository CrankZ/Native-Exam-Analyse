#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QTableWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QAxObject>
#include <QtXlsx>
#include <QMenu>
#include <QLineEdit>
#include <QCloseEvent>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>


#include "setpaper.h"
#include "JlCompress.h"
#include "quazip.h"
#include "quazipfile.h"
#include "sender.h"
#include "receiver.h"
#include "analyzer.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

private:
    QMap<QString, ExamInfo*> examInfoMap;
    QMap<QString ,int> eachClass;
    QMap<QString ,int> eachClass2;
    Sender *sender;
    SetPaper *setPaper;

    QTimer *showTimeTimer;
    QTimer *confirmTimer;
    int timeStartNum;
    int hour = -1;
    int min = -1;
    int sec = -1;
    QString ExamTime = "";
    Receiver *receiver;
    Analyzer *analyze;

    QThread *thread;
//    ViewPaper *vp = new ViewPaper();

    // 这两个弄成全局变量是为了方便生成EXCEL，不用在多次计算了
    QMap<QString, double> classAverageScoreMap;
    QMap<QString, double> classPassRateMap;
    // 确认收到学生考试信息
    // 学生no，有没有收到，如果为true就是收到了
    QMap<QString, bool> confirmStuMap;

private:

    void saveTabelData();
    void setEachClassViaTabel();
    void tabelWidget2Xlsx(QTableWidget *table,QString saveName);
    bool regExIsFullNumber(QString str);
    bool regExIsContainsChinese(QString str);
    void readData();
    bool isDirExist(QString fullPath);
    bool DelDir(QString path);
    void saveAllExamInfoToZipFile();
    void readExamInfoFromZipFile(QString filePath = "stuRes.zip");
    void compressDir(QString dir, QString saveName = "");
    void savaAllDataToXlsx(QString saveName = "stuRes.xlsx");
    bool saveStuPaper(QString saveName,QString paper);

    void setTableWidget(QString stuNo,QString stuName,QString stuClass,QString stuScore);
    void makeTableWidgetOneExamInfo(ExamInfo *examInfo);
    void getCurrentTimeFormWeb();
    void expireDate(QString endDateStr);
    void saveToDeskTop();
    void examInfoReceivedConfirm();
    void getCurrentTimeFromWeb();


private slots:
    void on_toolButton_openData_clicked();
    void on_toolButton_setPaper_clicked();
    void on_toolButton_saveRes_clicked();
    void on_toolButton_startExam_clicked();

public slots:
    void deleteRow();
    void viewStuPaper();
    void showTime();
    void updateTimeSlot(int time);
    void updateStuExamInfoSlot(ExamInfo *examInfo);
    void makeWidgetClassInfoSlot(QMap<QString, double> classAverageScoreMap, QMap<QString, double> classPassRateMap);
    void prepareNewExamSlot();


protected:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

signals:
    void analyzeSignal(ExamInfo *examInfo);
};

#endif // WIDGET_H
