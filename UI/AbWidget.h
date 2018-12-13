#ifndef ABWIDGET_H
#define ABWIDGET_H

#include <QWidget>

enum WidgetStatus
{
    ReaderAdmin = 1,
    BorrowAdmin = 2,
    BookAdmin = 3,
    Reader = 4,
    Display = 5,
    Create = 6,
    Alter = 7
};

class AbWidget : public QWidget
{
    Q_OBJECT

signals:
    void statusMes(const QString&, int);

public:
    explicit AbWidget(QWidget *parent = nullptr);

    virtual ~AbWidget() = 0;

    static QString TU8(const char* s);

    virtual void setStatusFor(WidgetStatus status) = 0;

protected:
    QString U8(const char* s);
    int showError(const char* s);
    int showError(const QString& s);

    bool checkError(int status);

    int showDone(const char* s);
    int showDone(const QString& s);
};

#endif // ABWIDGET_H
