#include "BookInfo.h"
#include "ui_BookInfo.h"
#include <QDebug>
#include <QFileDialog>

BookInfo::BookInfo(QWidget *parent) :
    InfoWidget(parent),
    ui(new Ui::BookInfo)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Dialog);
    setWindowModality(Qt::WindowModal);

    connect(ui->btn_clear,SIGNAL(clicked()),this,SLOT(clear()));

    connect(ui->btn_next,SIGNAL(clicked()),this,SIGNAL(next()));
    connect(ui->btn_prev,SIGNAL(clicked()),this,SIGNAL(prev()));

}

BookInfo::~BookInfo()
{
    qDebug()<<"BookInfo::~BookInfo()";
    delete ui;
}

void BookInfo::setWidgetMapper(QDataWidgetMapper* mapper)
{
    mapper->addMapping(ui->bkID,0);
    mapper->addMapping(ui->bkName,1);
    mapper->addMapping(ui->bkAuthor,2);
    mapper->addMapping(ui->bkPub,3);
    mapper->addMapping(ui->bkPubDate,4);
    mapper->addMapping(ui->bkISBN,5);
    mapper->addMapping(ui->bkClassfy,6);
    mapper->addMapping(ui->bkLang,7);
    mapper->addMapping(ui->bkPage,8);
    mapper->addMapping(ui->bkPrice,9);
    mapper->addMapping(ui->bkInDate,10);
    mapper->addMapping(ui->bkIntro,11);
    mapper->addMapping(ui->bkCover,12);
    mapper->addMapping(ui->bkStatus,13);
    mapper->addMapping(ui->bkNum,14);
}

void BookInfo::clear()
{
    ui->bkID->clear();
    ui->bkName->clear();
    ui->bkAuthor->clear();
    ui->bkPub->clear();
    //ui->bkPubDate->clear();
    ui->bkISBN->clear();
    ui->bkClassfy->clear();
    //ui->bkLang->clear();
    ui->bkPage->setValue(1);
    ui->bkPrice->setValue(0.00);
    ui->bkInDate->setDate(QDate::currentDate());
    ui->bkIntro->clear();
    ui->bkCover->clear();
    ui->bkNum->setValue(1);
}

void BookInfo::readRecord(const QSqlRecord& rec)
{
    record = rec;

    QPixmap pix;
    pix.loadFromData(rec.value(12).toByteArray());
    ui->bkCover->setScaledContents(true);
    ui->bkCover->setPixmap(pix);
}

void BookInfo::writeRecord(QSqlRecord& rec)
{
    rec = record;
    rec.setValue(0,ui->bkID->text().toLongLong());
    rec.setValue(1,ui->bkName->text());
    rec.setValue(2,ui->bkAuthor->text());
    rec.setValue(3,ui->bkPub->text());
    rec.setValue(4,ui->bkPubDate->text());
    rec.setValue(5,ui->bkISBN->text());
    rec.setValue(6,ui->bkClassfy->text());
    rec.setValue(7,ui->bkLang->text());
    rec.setValue(8,ui->bkPage->text());
    rec.setValue(9,ui->bkPrice->value());
    rec.setValue(10,ui->bkInDate->text());
    rec.setValue(11,ui->bkIntro->toPlainText());

    rec.setNull(12);

    rec.setValue(13,ui->bkStatus->currentIndex());
    rec.setValue(14,ui->bkNum->text());
}

//for create alter display
void BookInfo::setStatusFor(WidgetStatus status)
{
    if(status == Create || status == Alter)
        setEnable(true);
    else
        setEnable(false);
}

void BookInfo::setStatusForDisplay()
{
    setEnable(false);
}
void BookInfo::setStatusForCreate()
{
    setEnable(true);
}
void BookInfo::setStatusForAlter()
{
    setEnable(true);
}

void BookInfo::setEnable(bool flag)
{
    ui->bkName->setEnabled(flag);
    ui->bkAuthor->setEnabled(flag);
    ui->bkPub->setEnabled(flag);
    ui->bkPubDate->setEnabled(flag);
    ui->bkISBN->setEnabled(flag);
    ui->bkClassfy->setEnabled(flag);
    ui->bkLang->setEnabled(flag);
    ui->bkPage->setEnabled(flag);
    ui->bkPrice->setEnabled(flag);
    ui->bkInDate->setEnabled(flag);
    ui->bkIntro->setEnabled(flag);
    ui->bkCover->setEnabled(flag);
    ui->bkNum->setEnabled(flag);
    ui->bkStatus->setEnabled(flag);
    ui->bkName->setEnabled(flag);
}

void BookInfo::on_btn_submit_clicked()
{
    ui->btn_submit->setEnabled(false);
    if(!record.isEmpty())
    {
        QSqlRecord rec = record;
        writeRecord(rec);
        emit new_data(rec);
    }
    ui->btn_submit->setEnabled(true);
}

void BookInfo::on_btn_upload_clicked()
{
    QString fileStr = QFileDialog::getOpenFileName
            (this,U8("选择图片"),"","Images (*.png *.jpg)");

    if(fileStr != "")
    {
        QFile file(fileStr);
        QSharedPointer<QByteArray> data(new QByteArray);
        file.open(QIODevice::ReadOnly);
        *data = file.readAll();
        qDebug()<<"data:"<<data->count();

        QPixmap pix;
        pix.loadFromData(*data);
        ui->bkCover->setPixmap(pix);
        ui->bkCover->setScaledContents(true);
        emit updatePicture(data);
    }
}
