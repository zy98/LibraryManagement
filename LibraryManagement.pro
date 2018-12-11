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
    Database.cpp \
    LoginDialog.cpp \
    main.cpp \
    MainWindow.cpp \
    Delegate/ReaderDelegate.cpp \
    UI/AbWidget.cpp \
    UI/BookInfo.cpp \
    UI/BookWidget.cpp \
    UI/BorrowInfoWidget.cpp \
    UI/BorrowWidget.cpp \
    UI/ReaderInfo.cpp \
    UI/ReaderWidget.cpp \
    UI/Widget.cpp \
    Database.cpp \
    LoginDialog.cpp \
    main.cpp \
    MainWindow.cpp \
    Model/AbModel.cpp




HEADERS += \
    Database.h \
    LoginDialog.h \
    MainWindow.h \
    Delegate/ReaderDelegate.h \
    UI/AbWidget.h \
    UI/BookInfo.h \
    UI/BookWidget.h \
    UI/BorrowInfoWidget.h \
    UI/BorrowWidget.h \
    UI/InfoWidget.h \
    UI/ReaderInfo.h \
    UI/ReaderWidget.h \
    UI/Widget.h \
    Database.h \
    LoginDialog.h \
    MainWindow.h \
    Model/AbModel.h



FORMS += \
    LoginDialog.ui \
    MainWindow.ui \
    UI/BookInfo.ui \
    UI/BookWidget.ui \
    UI/BorrowInfoWidget.ui \
    UI/BorrowWidget.ui \
    UI/ReaderInfo.ui \
    UI/ReaderWidget.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
