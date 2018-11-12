#ifndef SETPAPER_H
#define SETPAPER_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "viewpaper.h"
#include "examinfo.h"
#include "justenum.h"
#include "sender.h"
#include "typeaddwidget.h"

namespace Ui {
class SetPaper;
}

class SetPaper : public QWidget
{
    Q_OBJECT

public:
    explicit SetPaper(QWidget *parent = 0);
    ~SetPaper();

    ExamInfo *examInfo;

private:
    Ui::SetPaper *ui;

private:
    Sender *sender;
    ViewPaper *viewPaper;
    QTimer *qtime;
    int totalSec;
    QVBoxLayout *layout = new QVBoxLayout;
    TypeAddWidget *typeAdd;
    int totalScore = 0;
    int totalQuestionNum = 0;


private slots:
    void on_timeLeft();
    void typeAddSlot(QString type, bool isRandom, int perScore, int typeQuestionNum, int blankNum, QString typePaper);

private slots:
    void on_addClassNameButton_clicked();
    void on_toolButton_openPaper_clicked();
    void on_toolButton_sendExamInfo_clicked();

private:
    void timeComboInit();
    bool timeCheck();
    void timeAdd();
    bool classCheck();
    void classAdd();
    bool paperCheck();
    void getPerScore();
signals:
    void updateTimeSignal(int);
    void prepareNewExamSignal();
protected:
    void closeEvent(QCloseEvent *event);
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
};

#endif // SETPAPER_H
