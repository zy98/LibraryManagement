#include "AbWidget.h"

#include <QMessageBox>

AbWidget::AbWidget(QWidget *parent) : QWidget(parent)
{

}

AbWidget::~AbWidget(){}

QString AbWidget::TU8(const char* s)
{
    return QString::fromLocal8Bit(s);
}

QString AbWidget::U8(const char* s)
{
    return AbWidget::TU8(s);
}

int AbWidget::showError(const char* s)
{
    return showError(U8(s));
}

int AbWidget::showError(const QString& s)
{
    QMessageBox dialog(QMessageBox::Critical,U8("Error"),s);
    dialog.setStandardButtons(QMessageBox::Reset | QMessageBox::Cancel);
    dialog.setButtonText(QMessageBox::Reset,U8("撤销"));
    dialog.setButtonText(QMessageBox::Cancel,U8("取消"));
    return dialog.exec();
}

int AbWidget::showDone(const char* s)
{
    return showDone(U8(s));
}
int AbWidget::showDone(const QString& s)
{
    QMessageBox dialog(QMessageBox::Information,U8("成功"),s);
    return dialog.exec();
}


