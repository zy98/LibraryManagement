#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QDataWidgetMapper>
#include <QSqlRecord>
#include "InfoWidget.h"

namespace Ui {
class BookInfo;
}

class BookInfo : public InfoWidget
{
    Q_OBJECT

signals:
    void prev();
    void next();

public:
    explicit BookInfo(QWidget *parent = nullptr);
    ~BookInfo();

    void setWidgetMapper(QDataWidgetMapper* mapper);

    void readRecord(const QSqlRecord& rec);
    void writeRecord(QSqlRecord& rec);

    void setStatusFor(WidgetStatus status);

public slots:
    void clear();

    void setEnable(bool flag);

protected:
    //void closeEvent(QCloseEvent* event);

    void setStatusForDisplay();
    void setStatusForCreate();
    void setStatusForAlter();

private slots:
    void on_btn_submit_clicked();

    void on_btn_upload_clicked();

private:
    Ui::BookInfo *ui;
};

#endif // BOOKINFO_H
