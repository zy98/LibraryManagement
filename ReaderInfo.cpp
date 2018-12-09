#include "ReaderInfo.h"
#include "ui_ReaderInfo.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

ReaderInfo::ReaderInfo(QWidget *parent) :
    InfoWidget(parent),
    ui(new Ui::ReaderInfo)
{
    ui->setupUi(this);
    ui->cmb_status->addItem("0","nomal");
    ui->cmb_status->addItem("1","loss");
    ui->cmb_status->addItem("2","discard");

    setStatusFor(Display);
}

ReaderInfo::~ReaderInfo()
{
    delete ui;
}

void ReaderInfo::readRecord(const QSqlRecord& rec)
{    
    if(!rec.isEmpty())
    {
        record = rec;
        ui->edit_id->setText(record.value(0).toString());
        ui->edit_pwd->setText(record.value(1).toString());
        ui->edit_name->setText(record.value(2).toString());
        ui->cmb_sex->setCurrentText(record.value(3).toString());
        ui->cmb_typeid->setCurrentIndex(record.value(4).toInt());
        ui->edit_user_code->setText(record.value(5).toString());
        ui->edit_lend->setText(record.value(6).toString());
        ui->edit_dept->setText(record.value(7).toString());
        ui->date->setDate(record.value(8).toDate());
        //ui->photo->setPicture(record.value(9).to)

        auto status = record.value(10).toInt();
        ui->cmb_status->setCurrentIndex(status);

        ui->edit_tele->setText(record.value(11).toString());
        ui->edit_mail->setText(record.value(12).toString());

        ui->btn_pwd->setEnabled(true);
    }
}

//void ReaderInfo::setTypeModel(QAbstractItemModel *model, int col)
//{
//    ui->cmb_typeid->setModel(model);
//    ui->cmb_typeid->setModelColumn(col);
//}

void ReaderInfo::setWidgetMapper(QDataWidgetMapper *mapper)
{
    mapper->addMapping(ui->edit_id,0);
    mapper->addMapping(ui->edit_pwd,1);
    mapper->addMapping(ui->edit_name,2);
    mapper->addMapping(ui->cmb_sex,3);
    mapper->addMapping(ui->cmb_typeid,4);
    mapper->addMapping(ui->edit_user_code,5);
    mapper->addMapping(ui->edit_lend,6);
    mapper->addMapping(ui->edit_dept,7);
    mapper->addMapping(ui->date,8);
    //mapper->addMapping();
    mapper->addMapping(ui->cmb_status,10);
    mapper->addMapping(ui->edit_tele,11);
    mapper->addMapping(ui->edit_mail,12);
}

//for create alter display
void ReaderInfo::setStatusFor(WidgetStatus status)
{
    if(status == Create)
    {
        setEnable(true);
        ui->btn_submit->show();
        ui->edit_id->setEnabled(true);
        ui->btn_loss->setEnabled(false);
        ui->btn_pwd->setEnabled(false);
    }
    else if(status == Alter)
    {
        setEnable(true);
        ui->edit_id->setEnabled(false);
    }
    else
    {
        setEnable(false);
    }
}

void ReaderInfo::on_btn_loss_clicked()
{  
    auto status = ui->cmb_status->currentIndex();
    auto id = ui->edit_id->text();

    QSqlDatabase db = QSqlDatabase::database("Library");
    QSqlQuery query(db);
    query.prepare("update Reader set rdStatus = ? where rdID = ?");
    if( status != 2 )
    {
        query.addBindValue(1);
        query.addBindValue(id);

        if(query.exec())
            ui->cmb_status->setCurrentIndex(1);
        else
            showError(query.lastError().text());
    }
}

void ReaderInfo::on_btn_pwd_clicked()
{

}

void ReaderInfo::on_btn_upload_clicked()
{

}

void ReaderInfo::on_btn_submit_clicked()
{
    ui->btn_submit->setEnabled(false);

    QSqlRecord record;
    writeRecord(record);

    emit new_data(record);

    ui->btn_submit->setEnabled(true);
}

void ReaderInfo::writeRecord(QSqlRecord& rec)
{
    rec = record;
    rec.setValue(0,ui->edit_id->text());
    rec.setValue(1,ui->edit_pwd->text());

    rec.setValue(2,ui->edit_name->text());
    rec.setValue(3,ui->cmb_sex->currentText());
    rec.setValue(4,ui->cmb_typeid->currentIndex());
    rec.setValue(5,ui->edit_user_code->text());
    rec.setValue(6,ui->edit_lend->text());
    rec.setValue(7,ui->edit_dept->text());
    rec.setValue(8,ui->date->text());
    rec.setNull(9);
    rec.setValue(10,ui->cmb_status->currentIndex());
    rec.setValue(11,ui->edit_tele->text());
    rec.setValue(12,ui->edit_mail->text());
}

void ReaderInfo::setEnable(bool flag)
{
    ui->edit_id->setEnabled(false);
    ui->edit_pwd->setEnabled(flag);
    ui->edit_name->setEnabled(flag);
    ui->cmb_sex->setEnabled(flag);
    ui->cmb_typeid->setEnabled(flag);
    ui->edit_user_code->setEnabled(flag);
    ui->cmb_status->setEnabled(flag);
    ui->date->setEnabled(flag);
    ui->edit_dept->setEnabled(flag);
    ui->photo->setEnabled(flag);
    ui->edit_tele->setEnabled(flag);
    ui->edit_mail->setEnabled(flag);

    ui->btn_loss->setEnabled(flag);
    ui->btn_pwd->setEnabled(flag);

    ui->btn_submit->hide();
}

void ReaderInfo::clear()
{
    ui->edit_id->clear();
    ui->edit_pwd->clear();
    ui->edit_name->clear();

    ui->edit_id->clear();
    ui->edit_user_code->clear();
    ui->edit_lend->setText("0");
    ui->edit_dept->clear();
    ui->date->clear();
    ui->cmb_typeid->setCurrentIndex(4);
    ui->photo->clear();
    ui->date->setDate(QDate::currentDate());

    ui->edit_tele->clear();
    ui->edit_mail->clear();
}

