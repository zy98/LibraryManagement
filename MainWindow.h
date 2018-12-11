#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Widget.h"

#include <QMainWindow>
#include <QSqlRecord>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,const QSqlRecord& rec = QSqlRecord());
    ~MainWindow();

public slots:
    void setStatusMsg(const QString& text, int timeout = 3000);
    void setStatusMsg(const char* text, int timeout = 3000);

private slots:
    void reLogin();
    void on_actionChangePwd_triggered();
    void createAction(bool status);
    void alterAction(bool status);

private:
    Ui::MainWindow *ui;
    Widget* widget;
    QSqlRecord record;

    Widget* initWiget(const QSqlRecord &rec);
    //连接工具栏信号与自定义信号
    void connectWidget();
    //设置工具栏状态
    void setStatusFor(WidgetStatus status);

    void setActionVisable(bool status);

};

#endif // MAINWINDOW_H
