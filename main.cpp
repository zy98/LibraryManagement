#include "MainWindow.h"
#include <QApplication>
#include "LoginDialog.h"
#include <QDebug>
//#include "vld.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Database db("Library");

    LoginDialog dialog;

    int ret = 0;
    int i = dialog.exec();

    if(i == QDialog::Accepted)
    {
        auto info = dialog.get_info();

        MainWindow w(nullptr,info);
        w.show();

        ret = a.exec();
    }

    return ret;
}
