#include "PreviewPaper.h"
#include "ui_PreviewPaper.h"

PreviewPaper::PreviewPaper(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreviewPaper)
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

PreviewPaper::~PreviewPaper()
{
    delete ui;
}


void PreviewPaper::reformStr(QString split, QString &paperEnd)
{
    if(split == "A)")
    {
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
    }
    else if(split == "A.")
    {
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


void PreviewPaper::selectRightAnsComAll()
{
    for (int i = 0; i < rightAnsComMap.count(); i++)
    {
        QString type = rightAnsComMap.keys().at(i);
        selectRightAnsCom(type);
    }
}
void PreviewPaper::selectRightAnsCom(QString type)
{
    QMap<int, QMap<int, QString>> rightAnsCom;
    rightAnsCom = rightAnsComMap.value(type);
    QList<QSplitter*> lSpCompletion;
    lSpCompletion = lSpCompletionMap.value(type);
//    qDebug() << "type:" << type << rightAnsCom;
    for (int questionNum = 0; questionNum < rightAnsCom.count(); questionNum++)
    {
        QMap<int, QString> rightAnsComOneBlank = rightAnsCom.value(questionNum);
        int lineEditNum = 1;
        for(int blankNum = 0;blankNum < rightAnsComOneBlank.count(); blankNum++)
        {
//            qDebug() << "lineEditNum:" << lineEditNum;
            QLineEdit *le = (QLineEdit*)lSpCompletion.at(questionNum)->widget(lineEditNum);
            le->setText(rightAnsComOneBlank.value(blankNum));
            lineEditNum = lineEditNum + 3;
        }
    }


}


/**
 * 把正确答案存入的在rightAns这个map中，仅仅用于单独存放的答案
 */
void PreviewPaper::addChoiceRightAns(QString type, QString str)
{
    QString strAll = str;
    if(strAll.contains("-答案-"))
    {
        QStringList strAllSplit = strAll.split("答案");
        int last = strAllSplit.count() - 1;//因为这里可能在题目里也有"答案"这个字样
        strAll = strAllSplit[last];
    }

    QMap<int, QString> rightAnsChoice;
    QString strLine = strAll.trimmed();
    strLine = strLine.remove(QRegExp("\\s"));//去掉所有空格
    strLine = strLine.remove(QRegExp("[0-9]"));//去掉所有数字
    strLine = strLine.remove("(");//去掉所有(符号
    //            ////qDebug() << "x:" << strLine;
    if(strLine.contains(")"))
    {
        //                ////qDebug() << strLine;
        QStringList strList = strLine.split(")");
        int ansCount = 0;//这个是题目序号
        for(int i = 0;i < strList.count(); i++)
        {
            QString strRightAns = strList[i];
            // 排除非选项，比如中文题目等等
            // 也许可能通过奇偶数来判断，选项应该只是偶数
            if(!strRightAns.isEmpty() &&
                    regExIsContainsEnglish(strRightAns) &&
                    strRightAns.toLocal8Bit().length() == 1)//答案必须为字母，而且必须是单个字符，当然不能为空
            {
                int rightAnsIdx = 0;

                if(compTitleNum > 0)
                {
                    rightAnsIdx = ansCount + choiceTitleNum;
                }
                else
                {
                    rightAnsIdx = ansCount;
                }
                rightAnsChoice.insert(rightAnsChoice.count(),strRightAns);//把正确结果写入realAns
                qDebug() << "rightAnsIdx:" << rightAnsIdx << ":" << rightAnsChoice;
                ansCount++;
            }
        }
    }
    qDebug() << "type:" << type << "rightAnsChoice.count():" << rightAnsChoice.count();
    rightAnsChoiceMap.insert(type, rightAnsChoice);
}


/**
 * 选中正确选项，仅供测试使用
 */
void PreviewPaper::selectRigthAnsChoice(QString type)
{
    QMap<int, QString> rightAns;
    QList<QSplitter*> lSp;
    rightAns = rightAnsChoiceMap.value(type);
    lSp = lSpMap.value(type);

    //可以把学生正确与否的结果也保存为map，然后序列化之后发送
    QMapIterator<int, QString> it(rightAns);//迭代器
    int titleNum = 0;
    while (it.hasNext())
    {
        it.next();
        // 从1开始是因为0是题目Label
        for(int choiceNum = 1; choiceNum < lSp.at(titleNum)->count();choiceNum++)
        {

            //choice为当前遍历的radio序号，1234.
            QRadioButton *radioB = (QRadioButton*)lSp.at(titleNum)->widget(choiceNum);
            if(it.key() == titleNum)//如果当前的题目和保存的rightAns一样，就是题目的序号
            {
                QStringList c =
                {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O"};


                //正确答案
                int py;//正确的结果
                for(int i = 0;i < c.count();i++)
                {
                    if(it.value() == c.at(i))
                    {
                        py = i;
                    }
                }

                if(choiceNum - 1 == py)
                {

                    radioB->setChecked(true);
                }
            }
        }
        titleNum++;
    }
}

/**
 * 填空题
 */
void PreviewPaper::loadCompletion(QString type, QString str)
{
    QLabel *labelType = new QLabel;
    QVBoxLayout *vboxTemp = new QVBoxLayout;
    labelType->setText(type);
    labelType->setStyleSheet("font: 16pt;");
//    vbox->addWidget(labelType);
    vboxTemp->addWidget(labelType);

    QStringList lineSplit = str.split("\n");
    // 一道填空题里面可能又多个填空的问题
    QMap<int, QString> rightAnsComOneBlank;
    int titleNumCom;
    QList<QSplitter*> lSpCompletion;
    for(titleNumCom = 0;titleNumCom < lineSplit.count();titleNumCom++)
    {
        QString strLine = lineSplit[titleNumCom];//每次从文件中读取一行
        QStringList splitCount = strLine.split("<completion>");
        QStringList splitA = strLine.split("<completion>");
        for(int i = 0; i < splitCount.count() - 1; i++)
        {
            lSpCompletion.append(new QSplitter);
            QStringList splitB = splitA[i+1].split("</completion>");
            QString completionRightAns  = splitB[0];
            qDebug() << "completionRightAns:" << completionRightAns;
            rightAnsComOneBlank.insert(i,completionRightAns);
            qDebug() << "rightAnsComOneBlank:" << rightAnsComOneBlank;

    //        QString fuck = xxx.replace(tr("<completion>%1</completion>").arg(completionRightAns),"__________");

            QLabel *lb = new QLabel(this);
            QLabel *lb2 = new QLabel(this);
            QLineEdit *le = new QLineEdit(this);
            QString headText = splitA[0];// 答案前面的内容
            splitA[0] = ""; //head只有一次使用，其他的都放在end里面了
            lb->setText(headText);
            lb->setStyleSheet("background-color: rgb(255, 85, 0);");
//            lb->adjustSize();
            // 模式sizePolicy是prefer，会自动伸展。
            // 这里有多长就显示多长就行了
            lb2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            lb2->sizePolicy();
            lSpCompletion.at(titleNumCom)->addWidget(lb);

            le->setText("");
            le->adjustSize();
            lSpCompletion.at(titleNumCom)->addWidget(le);
            QString endText = splitB[1];    //答案后面的内容
            lb2->setText(endText);
            lb2->setStyleSheet("background-color: rgb(0, 255, 127);");
//            lb2->adjustSize();
            lb2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            lSpCompletion.at(titleNumCom)->addWidget(lb2);
            lSpCompletion.at(titleNumCom)->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        }
//        vbox->addWidget(lSpCompletion.at(titleNumCom));
        vboxTemp->addWidget(lSpCompletion.at(titleNumCom));
//        wgNothing->adjustSize();

        rightAnsCom.insert(titleNumCom, rightAnsComOneBlank);
        rightAnsComOneBlank.clear();
//        lSpCompletionMap.clear();
    }
    lSpCompletionMap.insert(type, lSpCompletion);
    vbox->addLayout(vboxTemp);

    wgNothing->adjustSize();
    qDebug() << "rightAnsCom:" << rightAnsCom;
    rightAnsComMap.insert(type, rightAnsCom);
    rightAnsCom.clear();
//    totalScoreCompletion = titleNumCom * examInfo->perScoreCompletion;
//    int totalScore = totalScoreCompletion + totalScoreChoice;
//    si->setTotalScore(totalScore);
//    qDebug() << "totalSocre:" << totalScore;
}


void PreviewPaper::makeJustRadio(QString strAll)
{
    QStringList xx = strAll.split("<content>");
    QStringList contentSplit = xx[1].split("</content>");//这个是content

    JustRadio = contentSplit[1];
//    //qDebug() << "JustRadio:" << JustRadio;

    QStringList JustNextContentSplit = strAll.split(JustRadio);
    qDebug() << "JustNextContentSplit.count():" << JustNextContentSplit.count();
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

void PreviewPaper::makeContent(QString type, int i, QString strAll)
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
    qDebug() << "lLbContent.count():" << lLbContent.count();
    qDebug() << "lLbContentMap.count():" << lLbContentMap.count();
    wgNothing->adjustSize();
}

/**
 * 选择题题型，基本通杀所有选择题题型
 * 单选，阅读理解，选词填空等
 * TODO: 不支持多选
 */
void PreviewPaper::loadChoice(QString type,QString str)
{
    qDebug() << "type:" << type << "str:" << str;
    QString strAll = str;
    // 没有<content>就加一个，方便统一的处理
    // 后期进行判断，如果<content>标签里面的内容为空，就不添加
    if(!strAll.contains("<content>"))
    {
        strAll = "<content></content>" + strAll;
    }
    if(strAll.contains("<content>"))
    {
        QStringList strAllSplit = strAll.split("</content>");
        for(int i = 0;i < strAllSplit.count() - 1;i++)
        {
            makeJustRadio(strAll);
            makeContent(type, i, strAll);

            strAll = justNextContent;
            QString split;
            if(JustRadio.contains("A)") &&
                    JustRadio.contains("B)") &&
                    JustRadio.contains("C)"))
            {
                split = "A)";
            }
            else if (JustRadio.contains("A.") &&
                     JustRadio.contains("B.") &&
                     JustRadio.contains("C."))
            {
                split = "A.";
            }

            if(JustRadio.contains(split))
            {
                // 暂时还没法有多重换行符\n

                QStringList strAllSplit = JustRadio.split(split);
//                qDebug() << "JustRadio:" << endl << JustRadio;
                QStringList temp = strAllSplit[0].split("\n");
                QString firstQuestion = temp[temp.count() - 2];

                QString paperStart = firstQuestion + "\r\n";
                for(int i = 1; i < strAllSplit.count(); i++)
                {
                    //            ////qDebug() << strAllSplit[i];
                    paperStart += split + strAllSplit[i];//因为以"A)"为分界线的话，要切割很多块，所以要组合起来
                }
                ////qDebug() << "paperStart:" << paperStart;


                QString paperEnd;
                if(paperStart.contains("-答案-"))
                {
                    QStringList paperStartSplit = paperStart.split("-答案-");
                    int index = paperStartSplit.count() - 2;//确保是最后一个含有"答案"字符串
                    paperEnd = paperStartSplit[index];
                }
                else
                {
                    paperEnd = paperStart;
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
void PreviewPaper::makeWidget(QString type, QString str)
{
    str = str.replace("\n\n", "\n");
    QStringList splitByLine = str.split("\n");
    for(int i = 0;i < splitByLine.count();i++)
    {
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
        if(offset == 0)
        {
            if(i % (5+offset) == 0)//题目
            {
                lSp.append(new QSplitter(Qt::Vertical,0));
                QLabel *lbTitle = new QLabel(this);
                QString temp = strLine.mid(0,4);
                if(regExIsContainsNumber(temp))//如果本身有序号，就不自动填写了
                {
                    ////qDebug() << tr("strLine%1").arg(strLine);
                    lbTitle->setText(strLine);
                }
                else
                {
                    lbTitle->setText(tr("%1.%2").arg(titleNum+1).arg(strLine));
                    lbTitle->setText(strLine);
                }

                lbTitle->adjustSize();
                lSp.at(titleNum)->addWidget(lbTitle);
                titleNum++;
            }
            //以下是选项
            else
            {
                QRadioButton *radioB = new QRadioButton(this);
                QString choiceText;
                if(strLine.trimmed().isEmpty())
                {
                    choiceText = "";
                }
                else
                {
                    choiceText = strLine.trimmed();
                }
                radioB->setText(choiceText);
                radioB->adjustSize();
                int spNum = titleNum - 1;//sp的数量和title数量一致
                lSp.at(spNum)->addWidget(radioB);


                if(lSp.at(spNum)->count() == (5+offset))//一个title四个choice
                {

                    for(int q = 0;q < (4+offset); q++)//禁止splitter调节大小
                    {
                        QSplitterHandle *splitterHandle = lSp.at(spNum)->handle(q);//使splitter大小不可调节
                        if(splitterHandle)
                        {
                            splitterHandle->setDisabled(true);
                        }
                    }
                    lSp.at(spNum)->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
                    lSp.at(spNum)->adjustSize();
                }
            }
//            qDebug() << "type:" << type << "lSp.count():" << lSp.count();

        }

        // 如果offset>0，也就是说总题目数>4
        // 那么就是英语中的选词填空
        else if(offset > 0)
        {
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
                if(regExIsContainsNumber(temp))//如果本身有标题，就不自动填写了
                {
                    //qDebug() << tr("strLine%1").arg(strLine);
                    lbTitle->setText(strLine);
                }
                else
                {

                    //                        lbTitle->setText(tr("%1.%2").arg(titleNum+1).arg(strLine));
                    lbTitle->setText(strLine);
                }

                lbTitle->adjustSize();
                //                    //qDebug() << "titleNumtitleNumtitleNum:" << titleNum;
                lSp.at(titleNum)->addWidget(lbTitle);
                titleNum++;
                qDebug() << "WXTK,titleNum:" << titleNum;
            }
            else
            {
                // 选项都被改成了[]，这里把选项提取出来
                QStringList strLineSplit = strLine.split("][");
//                //qDebug() << "strLineSplit.count():" << strLineSplit.count();
                //qDebug() << offset + 5;
                QString choice = "";
                int spNum = lSp.count() - 1;//sp的数量和title数量一致
//                qDebug() << "compTitleNum:" << compTitleNum << ",lSp.count():" << lSp.count();
                for(int i = 0;i < strLineSplit.count();i++)
                {
                    QRadioButton *radioB = new QRadioButton(this);
                    choice = strLineSplit[i];
                    if(choice.contains("["))
                    {
                        QStringList strLinesplitB = choice.split("[");
                        choice = strLinesplitB[1];
                    }
                    if(choice.contains("]"))
                    {
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

//                if(lSp.at(spNum)->count() == (5+offset))
//                {
//                    lSp.at(spNum)->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
//                    lSp.at(spNum)->adjustSize();
//                    lSp.append(new QSplitter(Qt::Horizontal,0));
//                    qDebug() << "add,type:" << type << "lSp.count()" << lSp.count();
//                }
            }
        }
    }
    titleNumMap.insert(type, titleNum);
    lSpMap.insert(type, lSp);
    // 是不是随机模式
    if(examInfo->isRandomItem)
    {
//        randomAddWidget();
        normalAddWidget(type);
    }
    else
    {
        normalAddWidget(type);
    }
//    if(str == "choice2.txt") choiceTitleNum = titleNum;
//    if(str == "havaContentChoice2.txt") compTitleNum = titleNum - choiceTitleNum;



}

// 都是选择题，抽取出来
void PreviewPaper::normalAddWidget(QString type)
{
    qDebug() << "add,type:" << type;
    int titleNum = 0;

    titleNum = titleNumMap.value(type);

    QList<QLabel*> lLbContent =
            lLbContentMap.value(type);

    qDebug() << "lLbContent.count():" << lLbContent.count();

    QVBoxLayout *vboxTemp = new QVBoxLayout;
    QLabel *labelType = new QLabel;
    labelType->setText(type);
    labelType->setStyleSheet("font: 16pt;");

    if(lLbContent.count() <= 1)
    {
        vboxTemp->addWidget(labelType);
    }

    QList<QSplitter*> lSp = lSpMap.value(type);
    qDebug() << "lSpMap.count():" << lSpMap.count();
    qDebug() << "lSp.count():" << lSp.count();
    // 根据题目数量来判断该插哪里
    for(int i = 0;i < lSp.count();i++)
    {
        // 如果有题目的话
        if(lLbContent.count() > 0)
        {
            int temp = (5+offset);
            temp = 5;   // 选项数量
            // 所有选项个数 除以 题目个数
            temp = titleNum / lLbContent.count();
            qDebug() << "titleNum:" << titleNum << "lLbContent.count():" << lLbContent.count();

            if(i % (5) == 0)
            {
                int contentIdx = i / (temp);
                qDebug() << "i:" << i << ",temp + 1:" << temp + 1;
                qDebug() << "contentIdx:" << contentIdx;
                if(contentIdx < lLbContent.count() &&
                        !lLbContent.at(contentIdx)->text().isEmpty())
                {
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

void PreviewPaper::randomAddWidget()
{
//    //应该只有选择题随机，诸如阅读理解的不随机
//    if(lLbContent.count() > 0)
//    {
//        normalAddWidget();
//        return;
//    }

//    for(int i = 0;i < lSp.count();i++)
//    {
//        if(i >= titleNum)
//        {
//            lSp.removeAt(i);
//        }
//    }

//    QList<int> randomList;
//    randomList = randomNumList(titleNum);
//    for(int i = 0;i < randomList.count();i++)
//    {
//        int randomIdx = randomList.at(i);
//        QLabel *lbTitle = new QLabel;
//        lbTitle = (QLabel*)lSp.at(randomIdx)->widget(0);
//        QString oringText = lbTitle->text();

//        //是否含有题目序号
//        QString isContainsIdx = oringText.mid(0,2);
//        QStringList oringTextSplit = oringText.split(isContainsIdx);
//        if(regExIsContainsNumber(isContainsIdx))
//        {
//            oringTextSplit[0] = oringTextSplit[0].remove(QRegExp("[0-9]"));//去掉所有数字，可能题目中有数字
//            oringTextSplit[0] = oringTextSplit[0].remove(".");//去除标点符号
//        }
//        QString oringText2 = oringTextSplit[0] + oringTextSplit[1];
//        lbTitle->setText(tr("%1.%2").arg(i+1).arg(oringText2));
//        vbox->addWidget(lSp.at(randomIdx));
//    }
}





int PreviewPaper::choiceNum2(QString strAll)
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



bool PreviewPaper::regExIsContainsChinese(QString str)
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

bool PreviewPaper::regExIsContainsEnglish(QString str)
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

bool PreviewPaper::regExIsContainsNumber(QString str)
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


QByteArray PreviewPaper::encodeStr(QString strSource)
{
    QByteArray baEncode = strSource.toUtf8();
    baEncode = baEncode.toBase64();
    qDebug() << "baEncode:" << baEncode;
    return baEncode;

}

QString PreviewPaper::decodeBa(QByteArray baEncode)
{
    QString strDecode = QString::fromUtf8(QByteArray::fromBase64(baEncode));
    qDebug() << "strDecode:" << strDecode;
    return strDecode;
}

QString PreviewPaper::decodeStr(QString strEncode)
{
    QByteArray ba = strEncode.toUtf8();
    QString strDecode = QString::fromUtf8(QByteArray::fromBase64(ba));
    qDebug() << "strDecode:" << strDecode;
    return strDecode;
}
