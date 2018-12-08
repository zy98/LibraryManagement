#ifndef ABWIDGET_H
#define ABWIDGET_H

#include <QWidget>

class AbWidget : public QWidget
{
    Q_OBJECT

signals:
    void statusMes(const QString&, int);

public:
    explicit AbWidget(QWidget *parent = nullptr);

    virtual ~AbWidget() = 0;

    static QString TU8(const char* s);

protected:
    QString U8(const char* s);
    int showError(const char* s);
    int showError(const QString& s);

    int showDone(const char* s);
    int showDone(const QString& s);
};

#endif // ABWIDGET_H
