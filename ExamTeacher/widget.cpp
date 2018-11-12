#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout_main);

    setPaper = new SetPaper;
    receiver = new Receiver();
    QThread *th = new QThread();
    receiver->moveToThread(th);
    th->start();
    analyze = new Analyzer();
    thread = new QThread();
    sender = new Sender();
    analyze->moveToThread(thread);


    showTimeTimer = new QTimer;
    confirmTimer = new QTimer;



    //添加tabel的右键
    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* actionView = new QAction("查看学生试卷");
    QAction* actionDelete = new QAction("删除一行");
    ui->tableWidget->addAction(actionView);
//    ui->tableWidget->addAction(actionDelete);

    connect(actionView, SIGNAL(triggered()),
            this, SLOT(viewStuPaper()));
    connect(actionDelete, SIGNAL(triggered()),
            this, SLOT(deleteRow()));

    connect(setPaper,&SetPaper::updateTimeSignal,
            this,&Widget::updateTimeSlot);

    connect(setPaper,&SetPaper::prepareNewExamSignal,
            this,&Widget::prepareNewExamSlot);

    connect(receiver,&Receiver::updateStuExamInfoSignal,
            this,&Widget::updateStuExamInfoSlot);

    // 另起一个线程，专门分析数据
    connect(this, &Widget::analyzeSignal,
            analyze, &Analyzer::analyzeSlot);

    qRegisterMetaType<QMap<QString, double>>("QMap<QString, double>");
    connect(analyze, &Analyzer::makeWidgetClassInfoSignal,
            this, &Widget::makeWidgetClassInfoSlot);

    connect(showTimeTimer,&QTimer::timeout,
            this,&Widget::showTime);

    connect(confirmTimer,&QTimer::timeout,
            this,&Widget::examInfoReceivedConfirm);



    qDebug() << "main:" << QThread::currentThreadId();

//    expireDate("2019-05-05");//软件过期
    ui->tableWidget->viewport()->setAcceptDrops(true);
    setAcceptDrops(true);

    ui->toolButton_openData->hide();


    timeStartNum = 0;    //倒计时的时间
    updateTimeSlot(timeStartNum);

    confirmTimer->start(1000);
}


Widget::~Widget()
{
    delete ui;
}

void Widget::showTime()
{
    // 如果考试时间结束
    if (timeStartNum <= 1) {
        // 开始确保学生试卷有交上来
        confirmTimer->start(1000);
        showTimeTimer->stop();
    }
    updateTimeSlot(--timeStartNum);
}

/**
 * 主界面时间的初始化
 */
void Widget::updateTimeSlot(int time)
{
    timeStartNum = time;

    hour = timeStartNum / 3600;
    min = (timeStartNum - (hour * 3600)) / 60;
    sec = timeStartNum - (hour * 3600) - (min * 60);

    QString hourStr = tr("%1").arg(hour);
    QString minStr = tr(":%1").arg(min);
    QString secStr = tr(":%1").arg(sec);

    if(hour < 10) {
        hourStr = tr("0%1").arg(hour);
    }
    if(min < 10) {
        minStr = tr(":0%1").arg(min);
    }
    if(sec < 10) {
        secStr = tr(":0%1").arg(sec);
    }
    QString timeStr = (hourStr + minStr + secStr);
    ui->lcdNumber->display(timeStr);
}

void Widget::updateStuExamInfoSlot(ExamInfo *examInfo)
{
    examInfoMap.insert(examInfo->stuNo, examInfo);
    // 学生列表
    makeTableWidgetOneExamInfo(examInfo);
    // 单独一个线程分析数据
    // 分析结束后，返回数据
    thread->start();
    qDebug() << "这里有没有：" << examInfo->stuName;
    confirmStuMap.insert(examInfo->stuNo, true);
    confirmTimer->start(1000);
    emit analyzeSignal(examInfo);
//    perQuestionPercentageOneSi(si);
}

/**
 * 收到学生的成绩后给予确认
 * 有两种方案
 * 1、接收一个回复一个，但是只发送一次，怕接收不到。所以采取第二种
 * 2、所有信息加到map里，发送map
 *
 * 考试结束后，应该用定时器发送
 */
void Widget::examInfoReceivedConfirm()
{
//    if(confirmStuMap.count() > 0) {
//        sender->sendConfirm(CM_tExamConfirm, confirmStuMap);
//    }

    sender->sendConfirm(CM_tExamConfirm, confirmStuMap);
}

/**
 * 准备新的考试
 * 清空相关信息
 */
void Widget::prepareNewExamSlot()
{
    confirmStuMap.clear();
    confirmTimer->stop();
}

void Widget::viewStuPaper()
{
    ViewPaper *vp = new ViewPaper();
    int stuNum = examInfoMap.count();
    for(int i = 0;i < stuNum;i++) {
        ExamInfo *examInfo = examInfoMap.values().at(i);
        int currentRow = ui->tableWidget->currentRow();
        QString stuNoByMap = examInfo->stuNo;
        QString stuNoByTable = ui->tableWidget->item(currentRow,0)->text();
        // 通过学号来查找

        if (stuNoByMap == stuNoByTable) {
            qDebug() << ui->tableWidget->item(currentRow,0)->text();
            QString MAC = examInfo->stuMAC;
            QString stuNo = examInfo->stuNo;
            QString stuName = examInfo->stuName;
            QString stuClass = examInfo->stuClass;
            int score = examInfo->stuRealScore;

            QMap<QString, QMap<int, QString>> stuRealAnsChoice;
            QMap<QString, QMap<int, QMap<int, QString>>> stuRealAnsCom;
            stuRealAnsChoice = examInfo->stuRealAnsChoice;
            stuRealAnsCom = examInfo->stuRealAnsCom;

            // 题型名称,每题分值，试卷内容
            QHash<QString, QMap<int, QString>> paper;
            paper = examInfo->paper;


            for(int i = 0; i < paper.count(); i++) {
                QString typeName = paper.keys().at(i);
                QMap<int, QString> typeInfo = paper.values().at(i);
                for(int j = 0;j < typeInfo.count();j++) {
                    // 每题分值
                    int perTypeScore = typeInfo.keys().at(j);
                    QString typePaper = typeInfo.values().at(j);
                    vp->loadAuto(typeName, typePaper);
                }
            }

            vp->setStuInfo(stuNo,stuName,stuClass,score);
            // 选择学生真实答案
            vp->selectStuRealAnsChoice(stuRealAnsChoice);
            vp->selectStuRealAnsCom(stuRealAnsCom);
            vp->checkAnsIsRightAll();

        }
    }
    vp->show();
//    vp->clear();
}

void Widget::deleteRow()
{
    //TODO:增加删除确认
    int rowIndex = ui->tableWidget->currentRow();
    if (rowIndex != -1)
        ui->tableWidget->removeRow(rowIndex);
}




bool Widget::saveStuPaper(QString saveName,QString paper)
{
    qDebug() << tr("%1\n%2").arg(saveName).arg(paper);
    QString strSaveName = saveName;

    QFile fileOut(strSaveName);
    if(! fileOut.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {

        return false;
    }
    QByteArray baTemp = "";
    if(paper.isEmpty())
    {
        qDebug() << "试卷内容为空，无法保存";
    }
    else
    {
        baTemp += paper.toLocal8Bit();
        fileOut.write(baTemp);
        return true;
    }
    fileOut.close();
    return false;
}

/**
 * 保存当前考试内容
 * 把每个学生考试内容单独保存成文件
 * 注意：自定义map无法保存成文件，比如examInfoMap
 */
void Widget::saveAllExamInfoToZipFile()
{
    //程序目录下新建data文件夹，把数据都存进去
    QString currentPath =  QDir::currentPath();
    QString fullPath = currentPath + "/data";
    qDebug() << "fullPath:" << fullPath;

    DelDir(fullPath);//删除目录
    isDirExist(fullPath);//创建当前文件夹

    for(int i = 0;i < examInfoMap.count();i++) {
        ExamInfo *examInfo = examInfoMap.values().at(i);
        QString filename = tr("data//%1.dat").arg(i);
        qDebug() << filename;
        QFile myFile(filename);
        if (myFile.open(QIODevice::WriteOnly)) {
            QDataStream out(&myFile);
            out << examInfo;
        }
        myFile.flush();
        myFile.close();
        qDebug() << "myFile.size():" << myFile.size();
    }

    // 所有文件保存到data文件夹后，统一打包成zip文件
    QDateTime currentDateTime =QDateTime::currentDateTime();
    QString currentDateTimeStr = currentDateTime.toString("yyyyMMddhhmmss");

    QString saveName = tr("考试记录//%1考试记录.zip").arg(currentDateTimeStr);

    compressDir(fullPath, saveName);
    DelDir(fullPath);//删除目录
}

void Widget::readExamInfoFromZipFile(QString filePath)
{
    QString fullPath = filePath;
    qDebug() << fullPath;
    QuaZip zipR(fullPath);  //设置读取的zip文件
    zipR.open(QuaZip::mdUnzip); //以读取的方式打开zip文件
    for(int i = 0;i < zipR.getEntriesCount();i++) {
        zipR.setCurrentFile(tr("%1.dat").arg(i)); //设置要读取的zip内文件
        QuaZipFile fileR(&zipR);
        ExamInfo * examInfo = new ExamInfo();
        QDataStream in(&fileR);
        fileR.open(QIODevice::ReadOnly);
        in >> examInfo;

        examInfoMap.insert(examInfo->stuNo, examInfo);
        makeTableWidgetOneExamInfo(examInfo);
        // 单独一个线程分析数据
        // 分析结束后，返回数据
        thread->start();
        qDebug() << "这里有没有：" << examInfo->stuName;
        emit analyzeSignal(examInfo);

        fileR.close();  //使用后需要关闭
    }
    zipR.close();
}

/**
 * 压缩指定文件夹里面的所有内容
 */
void Widget::compressDir(QString dir,QString saveName)
{
    //选择压缩的文件夹，然后在这个文件夹上一层保存压缩的文件
    QStringList dirSplit = dir.split("/");
    QString lastDir = dirSplit[dirSplit.length() - 1];//得到最后一个目录

    QStringList dirSplit2 = dir.split(lastDir);//然后按照最后一个目录来分割
    QString savePath = dirSplit2[0];
    QString saveFile;
    if(saveName.isEmpty()) {
        saveFile = tr("%1/stuRes.dat").arg(savePath);
    } else {
        saveFile = saveName;
    }

    if (false==JlCompress::compressDir(saveFile,dir)) {
        qDebug()<<"compress failed";
    } else {
        qDebug()<<"compress successed";
    }
}

void Widget::saveToDeskTop()
{
    QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    qDebug() << "desktop:" << desktop;

    QString AllClassName = "";
    for(int i = 0;i < setPaper->examInfo->className.count();i++)
    {
        AllClassName += setPaper->examInfo->className.at(i);
        AllClassName += "-";
    }
    qDebug() << tr("%1%2考试结果").arg(AllClassName).arg(ExamTime);

    savaAllDataToXlsx(tr("%1/%2%3考试结果.xlsx").arg(desktop).arg(AllClassName).arg(ExamTime));
    QString fullPath = QDir::currentPath() + "/data";
    compressDir(fullPath,tr("%1/%2%3考试结果.dat").arg(desktop).arg(AllClassName).arg(ExamTime));
}


bool Widget::DelDir(QString path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList){ //遍历文件信息
        if (file.isFile()){ // 是文件，删除
            file.dir().remove(file.fileName());
        } else { // 递归删除
            DelDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

bool Widget::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists()){
        return true;
    } else {
        bool ok = dir.mkdir(fullPath);//只创建一级子目录，即必须保证上级目录存在
        return ok;
    }
}



//这个是根据tabel来添加的
//主要作用就是修改eachClass2这个map
void Widget::setEachClassViaTabel()
{
    int eachClassNum = 0;

    QString tabelDate[ui->tableWidget->rowCount()][ui->tableWidget->columnCount()];

    for(int row=0; row < ui->tableWidget->rowCount(); row++)
    {
        for(int col=0; col<ui->tableWidget->columnCount(); col++)
        {
            tabelDate[row][col] = ui->tableWidget->item(row, col)->text();
            //            qDebug() << tr("tabelDate[%1][%2]:%3").arg(row).arg(col).arg(tabelDate[row][col]);
            if(col == ui->tableWidget->columnCount()-2)
            {
                if(tabelDate[row][col] == "软件1508")
                {
                    eachClassNum++;
                    //                    qDebug() << "eachClassNum:" << eachClassNum;
                    eachClass2.insert("软件1508",eachClassNum);
                }
            }
        }
    }
}


// 根据接受过来的数据，添加tabel数据，还有计算平均分和及格率
void Widget::makeWidgetClassInfoSlot(QMap<QString, double> classAverageScoreMap, QMap<QString, double> classPassRateMap)
{
    // 搞个全局变量，生成表格的时候直接调用就行了
    this->classAverageScoreMap = classAverageScoreMap;
    this->classPassRateMap = classPassRateMap;
    int classNum = classPassRateMap.count();
    qDebug() << "classNum:" << classNum;
    for (int i = 0; i < classNum; i++) {
        QString className = classAverageScoreMap.keys().at(i);
        double classAverageScore = classAverageScoreMap.values().at(i);
        double classPassRate = classPassRateMap.values().at(i);

        qDebug() << "className:" << className
                 << "classAverageScore:" << classAverageScore
                 << "classPassRate:" << classPassRate;
    }

    // 直接clear会把标题也弄掉
    ui->tableWidget_classInfo->clear();
//    ui->tableWidget_classInfo->clear();
    for (int i = 0; i < classNum; i++) {
        ui->tableWidget_classInfo->setSortingEnabled(false);//这里开头和结尾都要加上，不然排序会出现空白，不知道为什么
        QString className = classAverageScoreMap.keys().at(i);
        double classAverageScore = classAverageScoreMap.values().at(i);
        double classPassRate = classPassRateMap.values().at(i);

        QTableWidgetItem *itemclassName = new QTableWidgetItem(className);
        QTableWidgetItem *itemclassAverageScore = new QTableWidgetItem();
        QTableWidgetItem *itemclassPassRate = new QTableWidgetItem();

        itemclassAverageScore->setData(Qt::EditRole, classAverageScore);
        itemclassPassRate->setData(Qt::EditRole, classPassRate);

        ui->tableWidget_classInfo->insertRow(0);
        ui->tableWidget_classInfo->setItem(0,0,itemclassName);
        ui->tableWidget_classInfo->setItem(0,1,itemclassAverageScore);
        ui->tableWidget_classInfo->setItem(0,2,itemclassPassRate);
        ui->tableWidget->setSortingEnabled(true);
    }
}


// 学生列表
void Widget::makeTableWidgetOneExamInfo(ExamInfo *examInfo)
{
    //这个效率更高，不用每次都循环全部，而是来一个增加一个
    //但是有时如果提交两次会出现重复，但是影响也不大，因为生成表格还是循环的
    //表格循环没有问题因为只循环一次，但是如果接收每次都要循环，就是n的n次方了，处理就会很慢
    ui->tableWidget->setSortingEnabled(false);//这里开头和结尾都要加上，不然排序会出现空白，不知道为什么
//    ui->tableWidget->clearContents();
//    ui->tableWidget->setRowCount(0);
//    ui->tableWidget->setRowCount(mapSi.count());

    QString MAC = examInfo->stuMAC;
    QString stuNo = examInfo->stuNo;
    QString stuName = examInfo->stuName;
    QString stuClass = examInfo->stuClass;
    int score = examInfo->stuRealScore;

    QTableWidgetItem *itemStuNo = new QTableWidgetItem(stuNo);
    QTableWidgetItem *itemStuName = new QTableWidgetItem(stuName);
    QTableWidgetItem *itemStuClass = new QTableWidgetItem(stuClass);
    QTableWidgetItem *itemStuScore = new QTableWidgetItem();

    int myNumber = score;
    itemStuScore->setData(Qt::EditRole, myNumber);

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0,itemStuNo);
    ui->tableWidget->setItem(0,1,itemStuName);
    ui->tableWidget->setItem(0,2,itemStuClass);
    ui->tableWidget->setItem(0,3,itemStuScore);


    ui->tableWidget->setSortingEnabled(true);
}

void Widget::setTableWidget(QString stuNo,QString stuName,QString stuClass,QString stuScore)
{
    qDebug() << stuNo << stuName << stuClass << stuScore;
    QTableWidgetItem *item_studNo = new QTableWidgetItem(stuNo);
    QTableWidgetItem *item_studName = new QTableWidgetItem(stuName);
    QTableWidgetItem *item_studClass = new QTableWidgetItem(stuClass);
    QTableWidgetItem *item_studScore = new QTableWidgetItem(stuScore);
    //    int myNumber = stuNo.toInt();
    //    item_studNo->setData(Qt::EditRole, myNumber);

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0,0,item_studNo);
    ui->tableWidget->setItem(0,1,item_studName);
    ui->tableWidget->setItem(0,2,item_studClass);
    ui->tableWidget->setItem(0,3,item_studScore);
}


void Widget::saveTabelData()
{
    tabelWidget2Xlsx(ui->tableWidget,"fuck.xlsx");
}

void Widget::savaAllDataToXlsx(QString saveName)
{
    //    QString saveName = "stuRes.xlsx";
    //利用QtXlsx把tabelWidget中的内容保存到xlsx表格中
    if(examInfoMap.count() <= 0)
    {
        qDebug() << "没有分数，啥也不干";
        return;
    }
    QXlsx::Document xlsx;
    xlsx.addSheet("成绩总览");
    xlsx.write(1,1,"学号");
    xlsx.write(1,2,"姓名");
    xlsx.write(1,3,"班级");
    xlsx.write(1,4,"分数");


    for(int row=0; row < examInfoMap.count(); row++) {
        ExamInfo *examInfo = examInfoMap.values().at(row);
        for(int col=0; col < 4; col++) {
            QString stuNo = examInfo->stuNo;
            QString stuName = examInfo->stuName;
            QString stuClass = examInfo->stuClass;
            int score = examInfo->stuRealScore;

            xlsx.write(row+1+1,1,stuNo);
            xlsx.write(row+1+1,2,stuName);
            xlsx.write(row+1+1,3,stuClass);
            xlsx.write(row+1+1,4,score);
        }
    }

    //每题正确率

    xlsx.write(1,7,"班级");
    xlsx.write(1,8,"及格率");
    xlsx.write(1,9,"平均分");

    //班级平均分，及格率
    for(int i = 0;i < classPassRateMap.count();i++) {
        QString className = classPassRateMap.keys().at(i);
        double classPassRate = classPassRateMap.value(className);
        double classAverageScore = classAverageScoreMap.value(className);

        xlsx.write(i+2,7,className);
        QXlsx::Format format;
        format.setNumberFormatIndex(10);//10就是处理%的方法，自动*100，并且加上%
        xlsx.write(i+2,8,(double)classPassRate,format);
        xlsx.write(i+2,9,(double)classAverageScore);
    }

    xlsx.saveAs(saveName);
}

void Widget::tabelWidget2Xlsx(QTableWidget *table,QString saveName)
{
    //利用QtXlsx把tabelWidget中的内容保存到xlsx表格中
    QXlsx::Document xlsx;
    xlsx.write(1,1,"学号");
    xlsx.write(1,2,"姓名");
    xlsx.write(1,3,"班级");
    xlsx.write(1,4,"分数");
    QString tabelDate[table->rowCount()][table->columnCount()];
    for(int row=0; row < table->rowCount(); row++)
    {
        for(int col=0; col < table->columnCount(); col++)
        {
            tabelDate[row][col] = table->item(row, col)->text();
            //            qDebug() << tr("tabelDate[%1][%2]:%3").arg(row).arg(col).arg(tabelDate[row][col]);
            if(regExIsFullNumber(tabelDate[row][col]) &&
                    tabelDate[row][col].toLocal8Bit().length() <= 10)//全部都是数字，而且位数小于等于10
            {
                //保存到XLSX为数字格式，方便排序
                xlsx.write(row+1+1,col+1,tabelDate[row][col].toInt());//从1开始，不是从0开始
                //这里多加1是因为，第一行要有相关标题
            }
            else
            {
                //全部为字符串形式
                xlsx.write(row+1+1,col+1,tabelDate[row][col]);//从1开始，不是从0开始
                //这里多加1是因为，第一行要有相关标题
            }
        }
    }
    xlsx.saveAs(saveName);
}


void Widget::on_toolButton_startExam_clicked()
{
    if(setPaper->examInfo->className.count() == 0){
        QMessageBox::warning(this,"试卷为空","没有试卷！无法开始考试！");
    } else {
        QDateTime currentDate = QDateTime::currentDateTime();
        ExamTime = currentDate.toString("yyyyMMddhhmm");//savetoDesktop()需要用到
        QMapIterator<QString, int> it(eachClass);
        while (it.hasNext()) {
            it.next();
            //        qDebug() << it.key() << ":"<< it.value();
        }
        sender->sendCommand(CM_tExamStart);
        //    analyseTabelData();
        int oneSec = 1000;    //一秒，这个一般不修改1000
        showTimeTimer->start(oneSec);
        QMessageBox::about(this,"考试开始","考试已经开始！");
        this->showMaximized();
    }
}


void Widget::on_toolButton_saveRes_clicked()
{
    //获取保存文件名
    QString saveName = QFileDialog::getSaveFileName(
                this,
                tr("保存信息"),
                tr("."),
                tr("EXCEL 2010 files(*.xlsx);;All files(*)")
                );
    //判断文件名长度
    if(saveName.length() < 1) {
        return;
    }
//    QString saveName = "xxx.xlsx";
    savaAllDataToXlsx(saveName);
    saveAllExamInfoToZipFile();
}
bool Widget::regExIsFullNumber(QString str)
{
    //如果包含汉字，则说明是题目的选项
    //str内必须全部都是数字，否则为false
    QRegExp rx("[0-9]");
    for(int i = 0;i < str.size();i++) {
        if(rx.indexIn(str,i) != i) {
            return false;
        }
    }
    return true;
}

bool Widget::regExIsContainsChinese(QString str)
{
    //如果包含汉字，则说明是题目的选项
    QRegExp rx("[\u4e00-\u9fa5]");
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        list << rx.cap(0);
        if(!rx.cap(0).isEmpty()) {
            return true;
        }
        //        qDebug() << list;
        pos += rx.matchedLength();
    }
    return false;
}


void Widget::on_toolButton_setPaper_clicked()
{
//    setPaper->setAttribute(Qt::WA_DeleteOnClose);
//    setPaper->show();//用这个话，上次的记录会保存
    setPaper = new SetPaper();

    connect(setPaper,&SetPaper::updateTimeSignal,
            this,&Widget::updateTimeSlot);

    connect(setPaper,&SetPaper::prepareNewExamSignal,
            this,&Widget::prepareNewExamSlot);

    setPaper->show();
}

void Widget::on_toolButton_openData_clicked()
{
    //先不要清空，不然如果正在接收数据，数据有丢失的风险
    //    for(int row = 0;row < ui->tableWidget->rowCount();row++)
    //    {
    //        ui->tableWidget->removeRow(row);
    //    }
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("打开配置文件"),
                tr("."),
                tr("Text files(*.dat);;All files(*)")
                );
    if(filePath.isEmpty()) {
        //空字符串不处理，返回
        return;
    } else {
        readExamInfoFromZipFile(filePath);
    }
}

void Widget::getCurrentTimeFormWeb()
{
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost("time.nist.gov", 13);
    if (socket->waitForConnected()) {
        if (socket->waitForReadyRead()) {
            QString str(socket->readAll());
            str = str.trimmed();
            qDebug() << str;
            str = str.section(" ", 1, 2);
            qDebug() << str;
        }
    }
    socket->close();
    delete socket;
}


void Widget::getCurrentTimeFromWeb()
{
    QTcpSocket *socket = new QTcpSocket();
    socket->connectToHost("time.nist.gov", 13);
    if (socket->waitForConnected()) {
        if (socket->waitForReadyRead()) {
            QString str(socket->readAll());
            str = str.trimmed();
            qDebug() << str;
            //"58424 18-11-02 06:04:48 03 0 0 885.9 UTC(NIST) *"
            str = str.section(" ", 1, 1);
            str = "20" + str;
            qDebug() << str;
            //"18-11-02 06:04:48"
        }
    }
    socket->close();
    delete socket;
}


void Widget::expireDate(QString endDateStr)
{
    //获取系统当前时间
    QDateTime currentDate = QDateTime::currentDateTime();
    QDateTime endDate = QDateTime::fromString(endDateStr, "yyyy-MM-dd");

    qint64 nSecs = endDate.secsTo(currentDate);
    if (nSecs > 0) {
        QMessageBox::about(this,"软件过期","软件过期，请购买正版\nQQ：331714375\nEmail:331714375@qq.com");
        this->deleteLater();
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("退出程序"),
                                   QString(tr("退出后将无法接受信息，是否退出？")),
                                   QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No) {
        event->ignore();  //忽略退出信号，程序继续运行
    }
    else if (button == QMessageBox::Yes) {
        event->accept();  //接受退出信号，程序退出
        savaAllDataToXlsx();
    }
}


void Widget::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void Widget::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls()) {
        QString fileName = url.toLocalFile();
        if(fileName.isEmpty()) {
            //空字符串不处理，返回
            return;
        } else {
            readExamInfoFromZipFile(fileName);
        }
        qDebug() << "Dropped file:" << fileName;
    }
}
