#include "typeaddwidget.h"
#include "ui_typeaddwidget.h"

TypeAddWidget::TypeAddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TypeAddWidget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout_main);
}

TypeAddWidget::~TypeAddWidget()
{
    delete ui;
}

void TypeAddWidget::on_pushButton_addType_clicked()
{
    typeName = ui->lineEdit_typeName->text();
    if(typeName.isEmpty()) {
        QMessageBox::warning(this, "错误", "请添加题型名称！");
        return;
    }
//    if(ui->lineEdit_perScore->text().isEmpty()) {
//        QMessageBox::warning(this, "错误", "请添加每题分数！");
//        return;
//    }

    QString strAll;
    QString filePathChoice = QFileDialog::getOpenFileName(
                this,
                tr("打开配置文件"),
                tr("."),
                tr("Text files(*.txt);;All files(*)")
                );
    if(filePathChoice.isEmpty()) {
        //空字符串不处理，返回
        return;
    } else {
        //定义文件对象
        QFile fileIn(filePathChoice);
        if(!fileIn.open(QIODevice::ReadOnly)) {
            return; //不处理文件
        }
        //读取并解析文件
        while(!fileIn.atEnd()) {
            QByteArray baAll =fileIn.readAll();
            strAll = QString::fromLocal8Bit(baAll);
            if(strAll.isEmpty()) {
                QMessageBox::warning(this,"错误", "文件不能为空！");
                return;
            }
        }
    }
    paper = strAll;
    ViewPaper *vp = new ViewPaper(ui->label_previewPaper);
    vp->isRandom = false;
    if(ui->checkBox_randomItem->isChecked()) {
        vp->isRandom = true;
    }
    vp->loadAuto(typeName, strAll);
    // 如果不能设置成随机模式vp->isRandom会改变为false
    isRandom = vp->isRandom;
    if(!isRandom) {
        ui->checkBox_randomItem->setChecked(false);
    }
    vp->selectRightAnsAll();
//    emit typeAddSignal(typeName, strAll);
    vp->resize(ui->label_previewPaper->width(), ui->label_previewPaper->height());
    vp->show();

    // 题目数量
    questionNum = vp->titleNumMap.values().at(0);
    ui->label_questionNum->setText(tr("%1题").arg(questionNum));
    blankNum = 0;
    questionNumCom = 0;
    if (questionNum == 0) {
        QMap<int, int> titleNumCom;
        titleNumCom = vp->titleNumMapCom.values().at(0);
        questionNumCom = titleNumCom.keys().at(0);
        blankNum = titleNumCom.values().at(0);
        ui->label_questionNum->setText(tr("%1题").arg(questionNumCom));
    }
    // 填空数量
    ui->label_blankNum->setText(tr("%1个").arg(blankNum));


    // 每题分值
    perScore = ui->comboBox_perScore->currentText().toInt();
    typeScore = perScore * questionNum;
    if(blankNum != 0 ) {
        typeScore = perScore * blankNum;
    }

    // 题型总分
    ui->label_typeScore->setText(tr("%1分").arg(typeScore));
}

void TypeAddWidget::on_pushButton_done_clicked()
{
//    qDebug() << "typeName:" << typeName << "typeScore:" << typeScore;

    if(paper.isEmpty()) {
        QMessageBox::warning(this, "错误", "试卷内容为空！");
        return;
    }
    if(questionNumCom != 0) {
        questionNum = questionNumCom;
    }
    emit typeAddSignal(typeName, isRandom, perScore, questionNum, blankNum, paper);
    this->close();
}

void TypeAddWidget::clear()
{
    ui->label_previewPaper->clear();
    ui->label_previewPaper->children().at(0)->deleteLater();
    ui->lineEdit_typeName->clear();
    ui->label_typeScore->setText("0分");
    ui->label_questionNum->setText("0题");
}


void TypeAddWidget::on_comboBox_perScore_currentTextChanged(const QString &currentText)
{
    perScore = currentText.toInt();
    typeScore = perScore * questionNum;
    if(blankNum != 0) {
        typeScore = perScore * blankNum;
    }
    ui->label_typeScore->setText(tr("%1分").arg(typeScore));
}
