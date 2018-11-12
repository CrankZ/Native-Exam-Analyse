#include "ViewPaper.h"
#include "ui_ViewPaper.h"

ViewPaper::ViewPaper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewPaper)
{
    ui->setupUi(this);
    vbox = new QVBoxLayout();
    wgNothing = new QWidget();
    wgNothing->setLayout(vbox);
    scrollArea = new QScrollArea();
    scrollArea->setWidget(wgNothing);
    scrollArea->setWidgetResizable(false);

    this->setLayout(ui->verticalLayout);
    ui->verticalLayout->addWidget(scrollArea);
    vbox->setAlignment(Qt::AlignTop);

}

ViewPaper::~ViewPaper()
{
    delete ui;
}


void ViewPaper::reformStr(QString split, QString &paperEnd)
{
    if(split == "A)") {
        paperEnd.replace("A)","[A]");
        paperEnd.replace("B)","[B]");
        paperEnd.replace("C)","[C]");
        paperEnd.replace("D)","[D]");
        paperEnd.replace("E)","[E]");
        paperEnd.replace("F)","[F]");
        paperEnd.replace("G)","[G]");
        paperEnd.replace("H)","[H]");
        paperEnd.replace("I)","[I]");
        paperEnd.replace("J)","[J]");
        paperEnd.replace("K)","[K]");
        paperEnd.replace("L)","[L]");
        paperEnd.replace("M)","[M]");
        paperEnd.replace("N)","[N]");
        paperEnd.replace("O)","[O]");
    } else if(split == "A.") {
        paperEnd.replace("A.","[A]");
        paperEnd.replace("B.","[B]");
        paperEnd.replace("C.","[C]");
        paperEnd.replace("D.","[D]");
        paperEnd.replace("E.","[E]");
        paperEnd.replace("F.","[F]");
        paperEnd.replace("G.","[G]");
        paperEnd.replace("H.","[H]");
        paperEnd.replace("I.","[I]");
        paperEnd.replace("J.","[J]");
        paperEnd.replace("K.","[K]");
        paperEnd.replace("L.","[L]");
        paperEnd.replace("M.","[M]");
        paperEnd.replace("N.","[N]");
        paperEnd.replace("O.","[O]");
    }

}

// 检查所有答案
void ViewPaper::checkAnsIsRightAll()
{
    checkAnsIsRightChoiceAll();
    checkAnsIsRightCompletionAll();
}
/**
 * 检查所有题型
 */
void ViewPaper::checkAnsIsRightChoiceAll()
{
    for(int i = 0; i < rightAnsChoiceMap.count(); i++)
    {
        QString type = rightAnsChoiceMap.keys().at(i);
        checkAnsIsRightChoice(type);
    }
}

void ViewPaper::checkAnsIsRightCompletionAll()
{
    for(int i = 0;i < rightAnsComMap.count(); i++)
    {
        QString type = rightAnsComMap.keys().at(i);
        checkAnsIsRightCompletion(type);
    }
}

//检查答案是否正确，并且标记起来
void ViewPaper::checkAnsIsRightChoice(QString type)
{
    QList<QSplitter*> lSp = lSpMap.value(type);
    QMap<int, QString> rightAns = rightAnsChoiceMap.value(type);
//    QMap<int, QString> eachTypeRealAns;

//    // 当前题型的每题的分数
//    QMap<int, QString> typeInfo = examInfo->paper.value(type);
//    int perTypeScore = typeInfo.keys().at(0);

    //可以把学生正确与否的结果也保存为map，然后序列化之后发送
    QMapIterator<int, QString> it(rightAns);//迭代器
    int titleNum = 0;
//    int typeScore = 0;
//    examInfo->stuEachTypeScore.insert(type, 0);
    while (it.hasNext()) {

        it.next();
        //qDebug() << tr("题目序号：%1,答案：%2" ).arg(it.key()).arg(it.value());

        // 从1开始是因为0是题目
        for(int choiceNum = 1; choiceNum < lSp.at(titleNum)->count();choiceNum++) {
            //choice为当前遍历的radio序号，1234.
            QRadioButton *radioB = (QRadioButton*)lSp.at(titleNum)->widget(choiceNum);

            if(it.key() == titleNum) {//如果当前的题目和保存的rightAns一样，就是题目的序号
                //默认为单选，因为map里的QString暂时只存了一个字符
                //自动选择正确答案

                //                        radioB->setChecked(true);

                //正确答案
                QStringList c =
                {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O"};

                int py;//正确的结果
                for(int i = 0;i < c.count();i++) {
                    if(it.value() == c.at(i)) {
                        py = i;
                    }
                }

                // 先把正确的题目弄成绿色的
                if(choiceNum - 1 == py) {
                    //                    radioB->setChecked(true);
                    // 如果是对的，就统一设置成绿色
                    radioB->setStyleSheet("background-color: rgb(0, 255, 0);");
                }

                //                ////qDebug() << choiceNum;

                // 如果按钮被选择了
                if(radioB->isChecked()) {
                    for(int i = 0;i <= c.count();i++) {
                        // 把学生选择的结果保存起来
                        if(choiceNum == i) {
//                            eachTypeRealAns.insert(titleNum, c.at(i-1));
//                            examInfo->stuRealAnsChoice.insert(type, eachTypeRealAns);
                        }
                    }
                    // 保存学生的对错
                    // 如果对了
                    if(choiceNum - 1 == py) {
//                        si->score = si->score + examInfo->perScoreChoice;
//                        si->setRealAnsIsRight(titleNum,true);//这里面只存题号与是否正确

                        // 后期学生端也可能需要单独题型的分数
//                        typeScore += perTypeScore;
//                        examInfo->stuEachTypeScore.insert(type, typeScore);
                    } else {
//                        si->setRealAnsIsRight(titleNum,false);
                        // 如果是错的，就弄成红色
                        radioB->setStyleSheet("background-color: rgb(255, 0, 0);");
                    }
                }
            }
        }
        titleNum++;
    }
}

void ViewPaper::checkAnsIsRightCompletion(QString type)
{
    QMap<int, QMap<int, QString>> rightAnsCom;
    QMap<int, QMap<int, QString>> stuRealAnsCom;
    rightAnsCom = rightAnsComMap.value(type);
    QList<QSplitter*> lSpCompletion;
    lSpCompletion = lSpCompletionMap.value(type);
//    QMap<int, QString> typeInfo = examInfo->paper.value(type);
//    int perTypeScore = typeInfo.keys().at(0);
//    int typeScore = 0;

    for (int questionNum = 0; questionNum < rightAnsCom.count(); questionNum++) {
        QMap<int, QString> rightAnsComOneBlank = rightAnsCom.value(questionNum);
        QMap<int, QString> realAnsComOneBlank;
        int lineEditNum = 1;
        for(int blankNum = 0;blankNum < rightAnsComOneBlank.count(); blankNum++) {
            QLineEdit *le = (QLineEdit*)lSpCompletion.at(questionNum)->widget(lineEditNum);

            if(le->text() == rightAnsComOneBlank.value(blankNum)) {
//                typeScore += perTypeScore;
//                examInfo->stuEachTypeScore.insert(type, typeScore);
                le->setStyleSheet("background-color: rgb(0, 255, 0);");
            } else {
                le->setStyleSheet("background-color: rgb(255, 0, 0);");
            }

            lineEditNum = lineEditNum + 3;

            realAnsComOneBlank.insert(blankNum, le->text());
        }
        stuRealAnsCom.insert(questionNum, realAnsComOneBlank);
    }
//    examInfo->stuRealAnsCom.insert(type, stuRealAnsCom);
}

/**
 * 把正确答案存入的在rightAns这个map中，仅仅用于单独存放的答案
 */
void ViewPaper::addChoiceRightAns(QString type, QString str)
{
    QString strAll = str;
    if(strAll.contains("-答案-")) {
        QStringList strAllSplit = strAll.split("答案");
        int last = strAllSplit.count() - 1;//因为这里可能在题目里也有"答案"这个字样
        strAll = strAllSplit[last];
    }

    QMap<int, QString> rightAnsChoice;
    QString strLine = strAll.trimmed();
    strLine = strLine.remove(QRegExp("\\s"));//去掉所有空格
    strLine = strLine.remove(QRegExp("[0-9]"));//去掉所有数字
    strLine = strLine.remove("(");//去掉所有(符号;

    // 答案按照)分隔
    if(!strLine.contains(")")) {
        QMessageBox::warning(this, "错误", "未检测到答案文件！");
        return;
    }
    // 根据)来判断有多少答案
    QStringList strList = strLine.split(")");

    if(strList.count() - 1 != titleNumMap.value(type)) {
        QMessageBox::warning(this, "错误", "题目数量与答案数量不匹配！");
        return;
    }

    int ansCount = 0;//这个是题目序号
    for(int i = 0;i < strList.count(); i++) {
        QString strRightAns = strList[i];
        // 排除非选项，比如中文题目等等
        // 也许可能通过奇偶数来判断，选项应该只是偶数
        if(!strRightAns.isEmpty() &&
                regExIsContainsEnglish(strRightAns) &&
                strRightAns.toLocal8Bit().length() == 1) {//答案必须为字母，而且必须是单个字符，当然不能为空
            int rightAnsIdx = 0;

            if(compTitleNum > 0) {
                rightAnsIdx = ansCount + choiceTitleNum;
            } else {
                rightAnsIdx = ansCount;
            }
            rightAnsChoice.insert(rightAnsChoice.count(),strRightAns);//把正确结果写入realAns
//            qDebug() << "rightAnsIdx:" << rightAnsIdx << ":" << rightAnsChoice;
            ansCount++;
        }
    }
//    qDebug() << "type:" << type << "rightAnsChoice.count():" << rightAnsChoice.count();
    rightAnsChoiceMap.insert(type, rightAnsChoice);
}


// 学生的真实答案
void ViewPaper::selectStuRealAnsChoice(QMap<QString, QMap<int, QString> > stuRealAnsChoiceMap)
{
    for(int i = 0; i < stuRealAnsChoiceMap.count(); i++) {
        QString type = stuRealAnsChoiceMap.keys().at(i);
        QMap<int, QString> stuRealAnsChoice;
        stuRealAnsChoice = stuRealAnsChoiceMap.values().at(i);
//        qDebug() << "进来了：" << stuRealAnsChoice.count();
        selectAnsChoice(type, stuRealAnsChoice);
    }
}


void ViewPaper::selectStuRealAnsCom(QMap<QString, QMap<int, QMap<int, QString>>> stuRealAnsComMap)
{
    for (int i = 0; i < stuRealAnsComMap.count(); i++) {
        QString type = stuRealAnsComMap.keys().at(i);
        QMap<int, QMap<int, QString>> stuRealAns =
                stuRealAnsComMap.values().at(i);
        selectAnsCom(type, stuRealAns);
    }
}

void ViewPaper::selectRightAnsAll()
{
    selectRigthAnsChoiceAll();
    selectRightAnsComAll();
}

void ViewPaper::selectRigthAnsChoiceAll()
{
    for (int i = 0;i < rightAnsChoiceMap.count(); i++) {
        QString type = rightAnsChoiceMap.keys().at(i);
        QMap<int, QString> choiceRightAns = rightAnsChoiceMap.values().at(i);
        selectAnsChoice(type, choiceRightAns);
    }
}

/**
 * 选中正确选项，仅供测试使用
 */
void ViewPaper::selectAnsChoice(QString type, QMap<int, QString> choiceAns)
{
    QList<QSplitter*> lSp;
    lSp = lSpMap.value(type);

    //可以把学生正确与否的结果也保存为map，然后序列化之后发送
    QMapIterator<int, QString> it(choiceAns);//迭代器
    int titleNum = 0;
    while (it.hasNext()) {
        it.next();
        // 从1开始是因为0是题目Label
        for(int choiceNum = 1; choiceNum < lSp.at(titleNum)->count();choiceNum++) {
            // choice为当前遍历的radio序号，1234.
            QRadioButton *radioB = (QRadioButton*)lSp.at(titleNum)->widget(choiceNum);
            // 如果当前的题目和保存的rightAns一样，就是题目的序号
            if(it.key() == titleNum) {
                QStringList c =
                {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O"};

                // 正确答案
                int py;
                for(int i = 0;i < c.count();i++) {
                    if(it.value() == c.at(i)){
                        py = i;
                    }
                }

                if(choiceNum - 1 == py) {
                    radioB->setChecked(true);
                }
            }
        }
        titleNum++;
    }
}


void ViewPaper::selectRightAnsComAll()
{
    for (int i = 0; i < rightAnsComMap.count(); i++) {
        QString type = rightAnsComMap.keys().at(i);
        QMap<int, QMap<int, QString>> rightAnsCom =
                rightAnsComMap.values().at(i);
        selectAnsCom(type, rightAnsCom);
    }
}

void ViewPaper::selectAnsCom(QString type, QMap<int, QMap<int, QString>> comAns)
{
    QList<QSplitter*> lSpCompletion;
    lSpCompletion = lSpCompletionMap.value(type);
//    qDebug() << "type:" << type << rightAnsCom;
    for (int questionNum = 0; questionNum < comAns.count(); questionNum++) {
        QMap<int, QString> rightAnsComOneBlank = comAns.value(questionNum);
        int lineEditNum = 1;
        for(int blankNum = 0;blankNum < rightAnsComOneBlank.count(); blankNum++) {
//            qDebug() << "lineEditNum:" << lineEditNum;
            QLineEdit *lineEdit = (QLineEdit*)lSpCompletion.at(questionNum)->widget(lineEditNum);
            lineEdit->setText(rightAnsComOneBlank.value(blankNum));
            lineEditNum = lineEditNum + 3;
        }
    }
}

/**
 * 填空题
 * 相关控件直接在这里渲染了
 */
void ViewPaper::loadCompletion(QString type, QString str)
{
    QLabel *labelType = new QLabel;
    QVBoxLayout *vboxTemp = new QVBoxLayout;
    labelType->setText(type);
    labelType->setStyleSheet("font: 16pt;");
//    vbox->addWidget(labelType);
    vboxTemp->addWidget(labelType);
//    qDebug() << str;
    QStringList lineSplit = str.split("\n");
    // 一道填空题里面可能又多个填空的问题
    QMap<int, QString> rightAnsComOneBlank;

    QList<QSplitter*> lSpCompletion;
    // 填空的数量，为真实的空格数量
    int lineEditNum = 0;
    int realTitleNum = 0;
    for(int titleNumCom = 0;titleNumCom < lineSplit.count();titleNumCom++) {

        QString strLine = lineSplit[titleNumCom];//每次从文件中读取一行
        if(!strLine.contains("<completion>")) {
            continue;
        }
        QStringList splitCount = strLine.split("<completion>");
        QStringList splitA = strLine.split("<completion>");
        for(int i = 0; i < splitCount.count() - 1; i++) {
            lineEditNum++;

            lSpCompletion.append(new QSplitter);
            QStringList splitB = splitA[i+1].split("</completion>");
            QString completionRightAns  = splitB[0];
            rightAnsComOneBlank.insert(i,completionRightAns);
            QLabel *lb = new QLabel(this);
            QLabel *lb2 = new QLabel(this);
            QLineEdit *lineEdit = new QLineEdit(this);
            QString headText = splitA[0];// 答案前面的内容
            splitA[0] = ""; //head只有一次使用，其他的都放在end里面了
            // i==0是把标题序号加到第一个label里面，防止重复添加
            if(i == 0) {
                headText = tr("%1.%2").arg(++globalTitleNum).arg(headText);
            }
            lb->setText(headText);
//            lb->setText(tr("%1.%2").arg(++globalTitleNum).arg(headText));
//            lb->setStyleSheet("background-color: rgb(255, 85, 0);");
//            lb->adjustSize();
            // 模式sizePolicy是prefer，会自动伸展。
            // 这里有多长就显示多长就行了
            lb2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            lb2->sizePolicy();
            lSpCompletion.at(realTitleNum)->addWidget(lb);
//            qDebug() << "i:" << i;

            lineEdit->setText("");
            lineEdit->adjustSize();
            lSpCompletion.at(realTitleNum)->addWidget(lineEdit);
            QString endText = splitB[1];    //答案后面的内容
            lb2->setText(endText);
//            lb2->setStyleSheet("background-color: rgb(0, 255, 127);");
//            lb2->adjustSize();
            lb2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            lSpCompletion.at(realTitleNum)->addWidget(lb2);
            lSpCompletion.at(realTitleNum)->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        }
        vboxTemp->addWidget(lSpCompletion.at(realTitleNum));
        rightAnsCom.insert(realTitleNum, rightAnsComOneBlank);
        rightAnsComOneBlank.clear();
        realTitleNum++;
    }
    QMap<int, int> blankNumMap;
    // 题目序号，填空数量
    blankNumMap.insert(realTitleNum, lineEditNum);
//    qDebug() << "realTitleNum:" << realTitleNum
//             << "lineEditNum:" << lineEditNum;
    titleNumMapCom.insert(type, blankNumMap);

    lSpCompletionMap.insert(type, lSpCompletion);
    vbox->addLayout(vboxTemp);
    wgNothing->adjustSize();
    rightAnsComMap.insert(type, rightAnsCom);
    rightAnsCom.clear();
}


void ViewPaper::makeJustRadio(QString strAll)
{
    QStringList xx = strAll.split("<content>");

    QStringList contentSplit = xx[1].split("</content>");//这个是content

    JustRadio = contentSplit[1];
//    qDebug() << "JustRadio:" << JustRadio;

    QStringList JustNextContentSplit = strAll.split(JustRadio);
//    qDebug() << "JustNextContentSplit.count():" << JustNextContentSplit.count();
    justNextContent = "";

    if(JustNextContentSplit.count() > 2)
    {
        justNextContent += JustNextContentSplit[JustNextContentSplit.count() - 2];
        justNextContent += JustRadio;
        for(int i = 0;i < JustNextContentSplit.count() - 2;i++)
        {
            justNextContent += JustNextContentSplit[i+2];
        }
    }
    else
    {
        for(int i = 0;i < JustNextContentSplit.count() - 1;i++)
        {
            justNextContent += JustNextContentSplit[i+1];
        }
    }


    wgNothing->adjustSize();
}

void ViewPaper::makeContent(QString type, int i, QString strAll)
{

    QStringList xx = strAll.split("<content>");
    QStringList contentSplit = xx[1].split("</content>");//这个是content
    QString content = contentSplit[0];
//    qDebug() << "i:" << i <<"content:" << content;

    lLbContent.append(new QLabel);

//    lLbContent.at(i)->setAlignment(Qt::AlignTop);
    lLbContent.at(i)->setWordWrap(true);
    lLbContent.at(i)->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    lLbContent.at(i)->setText(content);
    lLbContent.at(i)->adjustSize();
    lLbContentMap.insert(type, lLbContent);
//    qDebug() << "lLbContent.count():" << lLbContent.count();
//    qDebug() << "lLbContentMap.count():" << lLbContentMap.count();
    wgNothing->adjustSize();
}

void ViewPaper::loadAuto(QString type, QString str)
{
    if(str.contains("</completion>")) {
        loadCompletion(type, str);
    } else {
        loadChoice(type, str);
    }
}



/**
 * 选择题题型，基本通杀所有选择题题型
 * 单选，阅读理解，选词填空等
 * TODO: 不支持多选
 */
void ViewPaper::loadChoice(QString type,QString str)
{
    QString strAll = str;
    // 没有<content>就加一个，方便统一的处理
    // 后期进行判断，如果<content>标签里面的内容为空，就不添加
    if(!strAll.contains("<content>")) {
        strAll = "<content></content>" + strAll;
    }
    if(strAll.contains("<content>")) {
        QStringList strAllSplit = strAll.split("</content>");
        for(int i = 0;i < strAllSplit.count() - 1;i++)
        {
            makeJustRadio(strAll);
            makeContent(type, i, strAll);

            strAll = justNextContent;
            QString split;
            if(JustRadio.contains("A)") &&
                    JustRadio.contains("B)") &&
                    JustRadio.contains("C)")) {
                split = "A)";
            }
            else if (JustRadio.contains("A.") &&
                     JustRadio.contains("B.") &&
                     JustRadio.contains("C.")) {
                split = "A.";
            }

            if(JustRadio.contains(split)) {
                // 暂时还没法有多重换行符\n
                QStringList strAllSplit = JustRadio.split(split);
                QStringList temp = strAllSplit[0].split("\n");
                QString firstQuestion = temp[temp.count() - 2];

                QString paperStart = firstQuestion + "\r\n";
                for(int i = 1; i < strAllSplit.count(); i++) {
                    paperStart += split + strAllSplit[i];//因为以"A)"为分界线的话，要切割很多块，所以要组合起来
                }

                QString paperEnd;
                if(paperStart.contains("-答案-")) {
                    QStringList paperStartSplit = paperStart.split("-答案-");
                    int index = paperStartSplit.count() - 2;//确保是最后一个含有"答案"字符串
                    paperEnd = paperStartSplit[index];
                } else {
                    qDebug() << "没有答案文件";
                    QMessageBox::warning(this, "错误", "答案设置异常，请检查");
                    return;
//                    paperEnd = paperStart;
                }

                reformStr(split, paperEnd);
                paperEnd.remove("-");
                paperEnd.replace("\n\r","");
                paperEnd.replace("\r","");
                choiceNum2(paperEnd);
                makeWidget(type, paperEnd);
            }
        }
    }
    wgNothing->adjustSize();
    addChoiceRightAns(type, str);

    lLbContent.clear();
    lSp.clear();
    titleNum = 0;

}

/**
 * 负责题目的展示，包括题目和选项
 */
void ViewPaper::makeWidget(QString type, QString str)
{
    str = str.replace("\n\n", "\n");
    QStringList splitByLine = str.split("\n");
    for(int i = 0;i < splitByLine.count();i++) {
        QString strLine = splitByLine[i];
        //                strLine = strLine.remove(QRegExp("\\s"));//去掉所有空格，去掉空格的话，英文题目会有问题
        strLine = strLine.remove("\n");//去掉题目中的回车
//                strLine = strLine.remove(QRegExp("[0-9]"));//去掉所有数字，可能题目中有数字
        strLine = strLine.remove(".");//去掉所有.
        strLine = strLine.remove("-");//去掉所有-
        //                isAnsMode();//这里由addChoiceRightAns()这个函数添加标准答案

        // 防止有空白
        if(strLine.isEmpty()){
            continue;
        }
        // 正常的题型，每题四个选项
        if(offset == 0) {
            //题目
            if(i % (5+offset) == 0) {
                lSp.append(new QSplitter(Qt::Vertical,0));
                QLabel *lbTitle = new QLabel(this);
                QString temp = strLine.mid(0,4);
                // 如果本身有序号，就不自动填写了
//                if(regExIsContainsNumber(temp)) {
//                    lbTitle->setText(strLine);
//                } else {
//                    // 字符串前三个字符是否有数字，如果有则删除
//                    QString isContainsNumber = strLine.mid(0,3);
//                    QStringList oringTextSplit = strLine.split(isContainsNumber);
//                    if(regExIsContainsNumber(isContainsNumber)) {
//                        oringTextSplit[0] = oringTextSplit[0].remove(QRegExp("[0-9]"));//去掉所有数字，可能题目中有数字
//                        oringTextSplit[0] = oringTextSplit[0].remove(".");//去除标点符号
//                    }
//                    QString oringText2 = oringTextSplit[0] + oringTextSplit[1];

//                    lbTitle->setText(tr("%1.%2").arg(++globalTitleNum).arg(oringText2));
//                }
                // 暂时只支持统一编号
                // 字符串前三个字符是否有数字，如果有则删除
                QString isContainsNumber = strLine.mid(0,3);
                QStringList oringTextSplit = strLine.split(isContainsNumber);
                if(regExIsContainsNumber(isContainsNumber)) {
                    oringTextSplit[0] = oringTextSplit[0].remove(QRegExp("[0-9]"));//去掉所有数字，可能题目中有数字
                    oringTextSplit[0] = oringTextSplit[0].remove(".");//去除标点符号
                }
                QString oringText2 = oringTextSplit[0] + oringTextSplit[1];
                lbTitle->setText(tr("%1.%2").arg(++globalTitleNum).arg(oringText2));

                lbTitle->adjustSize();
                lSp.at(titleNum)->addWidget(lbTitle);
                titleNum++;
            } else {
                //以下是选项
                QRadioButton *radioB = new QRadioButton(this);
                QString choiceText;
                if(strLine.trimmed().isEmpty()) {
                    choiceText = "";
                } else {
                    choiceText = strLine.trimmed();
                }
                radioB->setText(choiceText);
                radioB->adjustSize();
                int spNum = titleNum - 1;//sp的数量和title数量一致
                lSp.at(spNum)->addWidget(radioB);


                //一个title四个choice
                if(lSp.at(spNum)->count() == (5+offset)) {
                    //禁止splitter调节大小
                    for(int q = 0;q < (4+offset); q++) {
                        //使splitter大小不可调节
                        QSplitterHandle *splitterHandle = lSp.at(spNum)->handle(q);
                        if(splitterHandle) {
                            splitterHandle->setDisabled(true);
                        }
                    }
                    lSp.at(spNum)->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                    lSp.at(spNum)->adjustSize();
                }
            }
//            qDebug() << "type:" << type << "lSp.count():" << lSp.count();

        } else if(offset > 0) {
            // 如果offset>0，也就是说总题目数>4
            // 那么就是英语中的选词填空
            // 题目格式如下，每两行一个选项

            //1.
            //A)B)C)D)E)F)G)H)I)J)K)L)M)N)O)
            //2.
            //A)B)C)D)E)F)G)H)I)J)K)L)M)N)O)
            //3.
            //A)B)C)D)E)F)G)H)I)J)K)L)M)N)O)

            if(i % 2 == 0)//题目
            {
                lSp.append(new QSplitter(Qt::Horizontal,0));
                QLabel *lbTitle = new QLabel(this);
                QString temp = strLine.mid(0,4);
                // 是否统一编号
                // 如果本身有标题，就不自动填写了
//                if(regExIsContainsNumber(temp)) {
////                    lbTitle->setText(strLine);
//                    lbTitle->setText(tr("%1.%2").arg(++globalTitleNum).arg(strLine));
//                } else {
//                    lbTitle->setText(tr("%1.").arg(++globalTitleNum));
//                }
                // 暂时只支持全局编号
                lbTitle->setText(tr("%1.").arg(++globalTitleNum));

                lbTitle->adjustSize();
                //                    //qDebug() << "titleNumtitleNumtitleNum:" << titleNum;
                lSp.at(titleNum)->addWidget(lbTitle);
                titleNum++;
            } else {
                // 选项都被改成了[]，这里把选项提取出来
                QStringList strLineSplit = strLine.split("][");
//                //qDebug() << "strLineSplit.count():" << strLineSplit.count();
                //qDebug() << offset + 5;
                QString choice = "";
                int spNum = lSp.count() - 1;//sp的数量和title数量一致
//                qDebug() << "compTitleNum:" << compTitleNum << ",lSp.count():" << lSp.count();
                for(int i = 0;i < strLineSplit.count();i++) {
                    QRadioButton *radioB = new QRadioButton(this);
                    choice = strLineSplit[i];
                    if(choice.contains("[")) {
                        QStringList strLinesplitB = choice.split("[");
                        choice = strLinesplitB[1];
                    }
                    if(choice.contains("]")) {
                        QStringList strLinesplitB = choice.split("]");
                        choice = strLinesplitB[0];
                    }
//                    //qDebug() << "choice:" << choice;

                    QString choiceText = tr("[%1]").arg(choice);
                    radioB->setText(choiceText);
                    radioB->adjustSize();
                    lSp.at(spNum)->addWidget(radioB);
                    lSp.at(spNum)->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                }
            }
        }
    }
    titleNumMap.insert(type, titleNum);
//    qDebug() << "inside,titleNum:" << titleNum;
    lSpMap.insert(type, lSp);

    if(isRandom) {
        randomAddWidget(type);
    } else {
        normalAddWidget(type);
    }
}

// 都是选择题，抽取出来
void ViewPaper::normalAddWidget(QString type)
{
//    qDebug() << "add,type:" << type;
    int titleNum = 0;

    titleNum = titleNumMap.value(type);

    QList<QLabel*> lLbContent =
            lLbContentMap.value(type);

//    qDebug() << "lLbContent.count():" << lLbContent.count();

    QVBoxLayout *vboxTemp = new QVBoxLayout;
    QLabel *labelType = new QLabel;
    labelType->setText(type);
    labelType->setStyleSheet("font: 16pt;");

    if(lLbContent.count() <= 1) {
        vboxTemp->addWidget(labelType);
    }

    QList<QSplitter*> lSp = lSpMap.value(type);
//    qDebug() << "lSpMap.count():" << lSpMap.count();
//    qDebug() << "lSp.count():" << lSp.count();
    // 根据题目数量来判断该插哪里
    for(int i = 0;i < lSp.count();i++) {
        // 如果有题目的话
        if(lLbContent.count() > 0) {
            int temp = (5+offset);
            temp = 5;   // 选项数量
            // 所有选项个数 除以 题目个数
            temp = titleNum / lLbContent.count();
//            qDebug() << "titleNum:" << titleNum << "lLbContent.count():" << lLbContent.count();

            if(i % (5) == 0) {
                int contentIdx = i / (temp);
//                qDebug() << "i:" << i << ",temp + 1:" << temp + 1;
//                qDebug() << "contentIdx:" << contentIdx;
                if(contentIdx < lLbContent.count() &&
                        !lLbContent.at(contentIdx)->text().isEmpty()) {
//                    vbox->addWidget(lLbContent.at(contentIdx));
//                    lLbContent.at(contentIdx)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
                    vboxTemp->addWidget(lLbContent.at(contentIdx));
//                    wgNothing->adjustSize();
                }
            }
        }
//        vbox->addWidget(lSp.at(i));
        vboxTemp->addWidget(lSp.at(i));
    }
    vbox->addLayout(vboxTemp);
}

void ViewPaper::randomAddWidget(QString type)
{
    int titleNum = 0;

    titleNum = titleNumMap.value(type);

    QList<QLabel*> lLbContent =
            lLbContentMap.value(type);

    for(int i = 0;i < lLbContent.count(); i++) {
        QString content = lLbContent.at(i)->text();
        if(!content.isEmpty()) {
            QMessageBox::warning(this, "警告", "不是单选题，禁止随机！\n已切换到非随机模式");
            normalAddWidget(type);
            isRandom = false;
            return;
        }
    }


//    qDebug() << "lLbContent.count():" << lLbContent.count();

    QVBoxLayout *vboxTemp = new QVBoxLayout;
    QLabel *labelType = new QLabel;
    labelType->setText(type);
    labelType->setStyleSheet("font: 16pt;");

    if(lLbContent.count() <= 1) {
        vboxTemp->addWidget(labelType);
    }

    QList<QSplitter*> lSp = lSpMap.value(type);
    // 根据题目数量来判断该插哪里
    for(int i = 0;i < lSp.count();i++) {
        // 如果有题目的话
        if(lLbContent.count() > 0) {
            int temp = (5+offset);
            temp = 5;   // 选项数量
            // 所有选项个数 除以 题目个数
            temp = titleNum / lLbContent.count();
            if(i % (5) == 0) {
                int contentIdx = i / (temp);
                if(contentIdx < lLbContent.count() &&
                        !lLbContent.at(contentIdx)->text().isEmpty()) {
                    vboxTemp->addWidget(lLbContent.at(contentIdx));
                }
            }
        }
    }
    QList<int> randomList;
    randomList = randomNumList(lSp.count());
    for(int i = 0;i < randomList.count();i++) {
        int randomIdx = randomList.at(i);

        QLabel *lbTitle = new QLabel;
        lbTitle = (QLabel*)lSp.at(randomIdx)->widget(0);
        QString oringText = lbTitle->text();

        // 如果是随机模式，题目的序号还要重写
        // 是否含有题目序号
        QString isContainsNumber = oringText.mid(0,3);
        QStringList oringTextSplit = oringText.split(isContainsNumber);
        if(regExIsContainsNumber(isContainsNumber))
        {
            oringTextSplit[0] = oringTextSplit[0].remove(QRegExp("[0-9]"));//去掉所有数字，可能题目中有数字
            oringTextSplit[0] = oringTextSplit[0].remove(".");//去除标点符号
        }
        QString oringText2 = oringTextSplit[0] + oringTextSplit[1];
        lbTitle->setText(tr("%1.%2").arg(i+1).arg(oringText2));


        vboxTemp->addWidget(lSp.at(randomIdx));
    }
    vbox->addLayout(vboxTemp);
}

QList<int> ViewPaper::randomNumList(int maxNum)
{
    //一定范围，且不重复的随机数
    //从0开始，所以到不了maxNum
    int i,j;
    QList<int> numbersList;

    QString MAC = getMACAdress();
    MAC = MAC.remove(QRegExp("[A-Z]"));
    MAC = MAC.remove(QRegExp(":"));
    MAC = MAC.mid(MAC.length()-4,MAC.length());
    int randomTime = QTime(0,0,0).secsTo(QTime::currentTime());
    int randomSeed = MAC.toInt() * (randomTime % 1000);
//    qDebug() << "randomSeed:" << randomSeed;
    qsrand(randomSeed);
    for(i=0;i<maxNum;i++) {
        numbersList.append(qrand()%maxNum);
        bool flag=true;
        while(flag) {
            for(j=0;j<i;j++) {
                if(numbersList[i]==numbersList[j]){
                    break;
                }
            }
            if(j<i) {
                numbersList[i]=rand()%maxNum;
            }
            if(j==i) {
                flag=!flag;
            }
        }
    }
//    for(i=0;i<maxNum;i++)
//    {
//        qDebug()<<numbersList[i];
//    }
    return numbersList;
}

//获得MAC地址
QString ViewPaper::getMACAdress()
{
    QList<QNetworkInterface> NetList;//网卡链表
    int NetCount = 0;//网卡个数
    int Neti=0;
    QNetworkInterface thisNet;//所要使用的网卡

    NetList = QNetworkInterface::allInterfaces();//获取所有网卡信息
    NetCount = NetList.count();//统计网卡个数

    for(Neti = 0; Neti < NetCount; Neti++){//遍历所有网卡
        if( NetList[Neti].isValid() ){//判断该网卡是否是合法
            thisNet = NetList[Neti];//将该网卡置为当前网卡
            break;
        }
    }
    return(thisNet.hardwareAddress());//获取该网卡的MAC
}




int ViewPaper::choiceNum2(QString strAll)
{
    QStringList c =
    {"[A]","[B]","[C]","[D]","[E]","[F]","[G]","[H]","[I]","[J]","[K]","[L]","[M]","[N]","[O]"};

    for(int i = c.count() - 1;i >= 0 ;i--)
    {
        if(strAll.contains(c.at(i)))
        {
            offset = i - 3;
            //qDebug() << tr("i:%1,offset:%2,c:%3").arg(i).arg(offset).arg(c.at(i));
            return offset;
        }
    }
    return 0;
}



bool ViewPaper::regExIsContainsChinese(QString str)
{
    //如果包含汉字，则说明是题目的选项
    QRegExp rx("[\u4e00-\u9fa5]");
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1)
    {
        list << rx.cap(0);
        if(!rx.cap(0).isEmpty())
        {
            return true;
        }
        //        qDebug() << list;
        pos += rx.matchedLength();
    }
    return false;
}

bool ViewPaper::regExIsContainsEnglish(QString str)
{
    //如果包含汉字，则说明是题目的选项
    QRegExp rx("[A-Z]");
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1)
    {
        list << rx.cap(0);
        if(!rx.cap(0).isEmpty())
        {
            return true;
        }
        //        qDebug() << list;
        pos += rx.matchedLength();
    }
    return false;
}

bool ViewPaper::regExIsContainsNumber(QString str)
{
    //如果包含汉字，则说明是题目的选项
    QRegExp rx("[0-9]");
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1)
    {
        list << rx.cap(0);
        if(!rx.cap(0).isEmpty())
        {
            return true;
        }
        //        qDebug() << list;
        pos += rx.matchedLength();
    }
    return false;
}

void ViewPaper::setStuInfo(QString stuNo, QString stuName, QString stuClass, int stuRealScore)
{
    ui->lineEdit_stuNo->setText(stuNo);
    ui->lineEdit_stuName->setText(stuName);
    ui->classComboBox_stuClass->addItem(stuClass);
    ui->classComboBox_stuClass->setCurrentText(stuClass);
    ui->label_score->setText(tr("%1").arg(stuRealScore));
}


QByteArray ViewPaper::encodeStr(QString strSource)
{
    QByteArray baEncode = strSource.toUtf8();
    baEncode = baEncode.toBase64();
    qDebug() << "baEncode:" << baEncode;
    return baEncode;

}

QString ViewPaper::decodeBa(QByteArray baEncode)
{
    QString strDecode = QString::fromUtf8(QByteArray::fromBase64(baEncode));
    qDebug() << "strDecode:" << strDecode;
    return strDecode;
}

QString ViewPaper::decodeStr(QString strEncode)
{
    QByteArray ba = strEncode.toUtf8();
    QString strDecode = QString::fromUtf8(QByteArray::fromBase64(ba));
    qDebug() << "strDecode:" << strDecode;
    return strDecode;
}
