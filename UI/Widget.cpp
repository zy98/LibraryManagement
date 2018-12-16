#include "Widget.h"
#include <QSqlError>
#include <QDebug>
#include <QInputDialog>
#include <QSqlQuery>

Widget::Widget(QWidget *parent) : AbWidget(parent)
{
}

Widget::~Widget(){}

bool Widget::setRecord(const QSqlRecord& rec)
{
    bool ret = !rec.isEmpty();
    if(ret) record = rec;
    return ret;
}

void Widget::first()
{
    viewPtr()->selectRow(0);
    emit viewPtr()->clicked(modelPtr()->index(0,0));
}

void Widget::last()
{
    auto row = modelPtr()->rowCount() - 1;
    viewPtr()->selectRow(row);
    emit viewPtr()->clicked(modelPtr()->index(row,0));
}

void Widget::next()
{
    auto row = viewPtr()->currentIndex().row() + 1;

    if(row > modelPtr()->rowCount() - 1) row = 0;

    viewPtr()->selectRow(row);
    emit viewPtr()->clicked(modelPtr()->index(row,0));
}

void Widget::prev()
{
    auto row = viewPtr()->currentIndex().row() - 1;
    if(row < 0) row = modelPtr()->rowCount() -1;

    viewPtr()->selectRow(row);
    emit viewPtr()->clicked(modelPtr()->index(row,0));
}

void Widget::submitItem()
{
    if(! (modelPtr()->submitData()) )
        showError(modelPtr()->dbError());
}

void Widget::deleteItem()
{
    modelPtr()->deleteItem(viewPtr()->selectionModel());
}

void Widget::changePwd()
{
    QString srcPwd = QInputDialog::getText(this,Widget::TU8("修改密码"),Widget::TU8("原密码："),
                                           QLineEdit::Password);
    if(srcPwd == "") return;

    QString newPwd = QInputDialog::getText(this,Widget::TU8("修改密码"),Widget::TU8("新密码："),
                                           QLineEdit::PasswordEchoOnEdit);
    if(newPwd == "") return;

    if(srcPwd != newPwd)
    {
        QSqlDatabase db = QSqlDatabase::database("Library");

        QSqlQuery query(db);

        query.prepare("exec proc_update_pwd ?, ?, ?");
        query.addBindValue(record.value(0));
        query.addBindValue(srcPwd);
        query.addBindValue(newPwd);

        if(!query.exec())
            emit statusMes(query.lastError().databaseText(),3000);

        else
            emit statusMes(U8("修改密码成功！"),4000);
        return;
    }

    emit statusMes(U8("新密码与原密码相同，已取消修改！"),4000);
}

bool Widget::createItem(QSqlRecord& rec)
{
    bool ret = modelPtr()->fillRowItem(0,rec);
    if(!ret) showError(modelPtr()->dbError());
    return ret;
}

bool Widget::updatePicture(QSharedPointer<QByteArray> dataImg)
{
    qDebug()<<"call updatePicture";
    return modelPtr()->uploadPicture(viewPtr()->selectionModel(),dataImg);
}

