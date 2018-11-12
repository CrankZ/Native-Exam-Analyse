#include "tempform.h"
#include "ui_tempform.h"

TempForm::TempForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TempForm)
{
    ui->setupUi(this);
}

TempForm::~TempForm()
{
    delete ui;
}

void TempForm::on_toolButton_restart_clicked()
{

}
