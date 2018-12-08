#-------------------------------------------------
#
# Project created by QtCreator 2018-11-16T14:50:44
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LibraryManagement
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    AbWidget.cpp \
    Database.cpp \
    LoginDialog.cpp \
    main.cpp \
    MainWindow.cpp \
    ReaderInfo.cpp \
    ReaderWidget.cpp \
    BookInfo.cpp \
    Widget.cpp \
    ReaderDelegate.cpp \
    BookWidget.cpp \
    BorrowWidget.cpp \
    BorrowInfo.cpp




HEADERS += \
    AbWidget.h \
    Database.h \
    LoginDialog.h \
    MainWindow.h \
    ReaderInfo.h \
    ReaderWidget.h \
    BookInfo.h \
    Widget.h \
    ReaderDelegate.h \
    BookWidget.h \
    BorrowWidget.h \
    InfoWidget.h \
    BorrowInfo.h



FORMS += \
    LoginDialog.ui \
    MainWindow.ui \
    ReaderInfo.ui \
    ReaderWidget.ui \
    BookInfo.ui \
    BookWidget.ui \
    BorrowWidget.ui \
    BorrowInfo.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
