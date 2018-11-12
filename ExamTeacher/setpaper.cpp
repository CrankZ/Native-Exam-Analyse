#include "setpaper.h"
#include "ui_setpaper.h"

SetPaper::SetPaper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetPaper)
{
    ui->setupUi(this);

    int oneSec = 1000;    //一秒，这个一般不修改1000
    qtime = new QTimer;
    qtime->start(oneSec); //start为QTIME自己的函数

    connect(qtime,&QTimer::timeout,
            this,&SetPaper::on_timeLeft);

    typeAdd = new TypeAddWidget;
    connect(typeAdd,&TypeAddWidget::typeAddSignal,
            this,&SetPaper::typeAddSlot);

    this->setLayout(ui->verticalLayout_main);
    timeComboInit();

    examInfo = new ExamInfo;
    sender = new Sender;
    viewPaper = new ViewPaper(ui->paperATextEdit);
//    viewPaper->hide();

    totalSec = 0;

    ui->paperATextEdit->setAcceptDrops(false);
    this->setAcceptDrops(true);
//    setAttribute(Qt::WA_DeleteOnClose);
}

void SetPaper::typeAddSlot(QString type, bool isRandom, int perScore, int typeQuestionNum, int blankNum, QString typePaper)
{
    qDebug() << "typeQuestionNum:" << typeQuestionNum << "blankNum:" << blankNum;
    int typeScore = 0;
    typeScore = perScore * typeQuestionNum;
    if (blankNum != 0) {
        typeScore = perScore * blankNum;
    }
    totalScore += typeScore;
    totalQuestionNum += typeQuestionNum;
    // 赋值当前试卷总分
    examInfo->paperTotalScore = totalScore;
    ui->label_totalScore->setText(tr("%1分").arg(totalScore));
    ui->label_totalQuestionNum->setText(tr("%1题").arg(totalQuestionNum));
    viewPaper->isRandom = isRandom;
    viewPaper->loadAuto(type, typePaper);
    // 预览模式，直接选择正确答案
    viewPaper->selectRightAnsAll();
    viewPaper->resize(ui->paperATextEdit->width(), ui->paperATextEdit->height());

    // 添加ExamInfo，等待发送
    // 每题分数和题型内容
    QMap<int, QString> typeInfo;
    typeInfo.insert(perScore, typePaper);
    examInfo->paper.insert(type, typeInfo);
    examInfo->isRandomMap.insert(type, isRandom);
    qDebug() << "examInfo->paper.count():" << examInfo->paper.count();
}

SetPaper::~SetPaper()
{
    delete ui;
}

void SetPaper::on_timeLeft()
{
    viewPaper->resize(ui->paperATextEdit->width(), ui->paperATextEdit->height());
    // 这个adjust不知道为啥，只能在后期才有效，放到按钮函数里没用。。
    viewPaper->wgNothing->adjustSize();
}

void SetPaper::timeComboInit()
{
    for (int i = 0; i <= 10;i++) {
        ui->hourComboBox->addItem(tr("%1").arg(i));
    }
    for (int i = 0; i <= 59;i++) {
        ui->minComboBox->addItem(tr("%1").arg(i));
    }
    for (int i = 0; i <= 59;i++) {
        ui->secComboBox->addItem(tr("%1").arg(i));
    }
}


void SetPaper::on_addClassNameButton_clicked()
{
    if(ui->classNameLineEdit->text().isEmpty()) {
        QMessageBox::warning(this,"班级错误","班级不能为空！");
    } else {
        //这里有等于号是因为要包括0的情况，不然第一次没法添加
        for(int i = 0;i <= ui->classNameComboBox->count();i++) {
            if(ui->classNameComboBox->itemText(i) ==  ui->classNameLineEdit->text()) {
                QMessageBox::about(this,"重复添加","班级已存在，不可重复添加！");
                break;
            } else {
                QMessageBox::about(this,"添加成功","添加成功！");
                ui->classNameComboBox->addItem(ui->classNameLineEdit->text());
                break;
            }
        }
    }
    ui->classNameLineEdit->clear();
}



bool SetPaper::timeCheck()
{
    if(ui->hourComboBox->currentText() == "0" &&
            ui->minComboBox->currentText() == "0"&&
            ui->secComboBox->currentText() == "0") {
        QMessageBox::warning(this,"时间错误","时间不可为空，请重新设置时间");
        return false;
    } else {
        return true;
    }
    return false;
}

void SetPaper::timeAdd()
{
    if (timeCheck()) {
        int hourInt = ui->hourComboBox->currentText().toInt() *60*60;
        int minInt = ui->minComboBox->currentText().toInt() * 60;
        int secInt = ui->secComboBox->currentText().toInt();
        //    //qDebug() << tr("int:%1").arg(hourTime);
        totalSec = hourInt + minInt + secInt;
        //        //qDebug() << tr("totalSec:%1").arg(totalSec);
        examInfo->time = totalSec;
    }
}

bool SetPaper::classCheck()
{
    if(ui->classNameComboBox->count() <= 0) {
        QMessageBox::warning(this,"班级错误","班级不可为空，请添加班级");
        return false;
    } else {
        return true;
    }
    return false;
}

void SetPaper::classAdd()
{
    if(classCheck()) {
        examInfo->className.clear();
        for(int i = 0; i < ui->classNameComboBox->count() ;i++) {
            QString className = ui->classNameComboBox->itemText(i);
            examInfo->className.append(className);
        }
        //qDebug() << "Xcount:" << examInfo->className.count();
    }
}

// 检查是否有试卷内容
bool SetPaper::paperCheck()
{
    if(examInfo->paper.count() > 0) {
        return true;
    }
    QMessageBox::warning(this,"试卷错误","试卷不能为空，请添加试卷");
    return false;
}


void SetPaper::on_toolButton_sendExamInfo_clicked()
{
    if(timeCheck() && classCheck() && paperCheck()) {
        timeAdd();
        classAdd();

        sender->sendExamInfo(CM_tExamPaperInfo,examInfo);

        QMessageBox::about(this,"试卷发送成功","试卷发送成功，请学生填写个人信息\n学生填写完毕后，可以开始考试！");
        emit updateTimeSignal(totalSec);//发送时间到主界面
        this->hide();
    }
    // 清空接收成功学生的信息
    emit prepareNewExamSignal();
}

void SetPaper::on_toolButton_openPaper_clicked()
{
//    typeAdd->deleteLater();
//    typeAdd = new TypeAddWidget;
//    TypeAddWidget *typeAdd = new TypeAddWidget();
    typeAdd->show();
    typeAdd->clear();


//    // 这个不知道为啥，放到别的地方就出错。。
//    if(!viewPaper->isVisible()) {
//        viewPaper = new ViewPaper(ui->paperATextEdit);
//    }
}


void SetPaper::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void SetPaper::dropEvent(QDropEvent *e)
{
//    foreach (const QUrl &url, e->mimeData()->urls()) {
//        QString fileName = url.toLocalFile();
//        if(fileName.isEmpty())
//        {
//            //空字符串不处理，返回
//            return;
//        }
//        else
//        {
//            viewPaper->deleteLater();
//            vp = new ViewPaper(ui->paperATextEdit);

//            viewPaper->LoadWhich(fileName);
//            viewPaper->isSelectRightAns(true);
//            viewPaper->resize(ui->paperATextEdit->width(),ui->paperATextEdit->height());
//            viewPaper->show();

//            //定义文件对象
//            QFile fileIn(fileName);
//            if(!fileIn.open(QIODevice::ReadOnly))
//            {
//                return; //不处理文件
//            }
//            //读取并解析文件
//            while(!fileIn.atEnd())
//            {
//                QByteArray baAll =fileIn.readAll();
//                QString strAll = QString::fromLocal8Bit(baAll);
//                if(strAll.contains("<content>"))
//                {
//                    ui->plainTextEdit_comprehension->setPlainText(strAll);
//                }
//                if(strAll.contains("<completion>"))
//                {
//                    ui->plainTextEdit_completion->setPlainText(strAll);
//                }
//                else
//                {
//                    //最后是选择题
//                    ui->plainTextEdit_choice->setPlainText(strAll);
//                }
//            }
//        }
//        //qDebug() << "Dropped file:" << fileName;
//    }
}

void SetPaper::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("退出程序"),
                                   QString(tr("试卷未发送，是否退出？")),
                                   QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No) {
        event->ignore();  //忽略退出信号，程序继续运行
    }
    else if (button == QMessageBox::Yes) {
        event->accept();  //接受退出信号，程序退出
    }
}

