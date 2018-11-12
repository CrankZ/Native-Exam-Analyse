#ifndef TYPEADDWIDGET_H
#define TYPEADDWIDGET_H

#include <QWidget>
#include <QFileDialog>

#include "viewpaper.h"

namespace Ui {
class TypeAddWidget;
}

class TypeAddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TypeAddWidget(QWidget *parent = 0);
    ~TypeAddWidget();

public:
    void clear();

private slots:
    void on_pushButton_addType_clicked();

    void on_pushButton_done_clicked();

    void on_comboBox_perScore_currentTextChanged(const QString &currentText);

private:
    Ui::TypeAddWidget *ui;

private:
    QString typeName;
    QString paper;
    int questionNum = 0;
    int questionNumCom = 0;
    int blankNum = 0;
    int typeScore = 0;
    int perScore = 0;
    bool isRandom = false;

signals:
    typeAddSignal(QString, bool, int, int, int, QString);

};

#endif // TYPEADDWIDGET_H
