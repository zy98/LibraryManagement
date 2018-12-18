#ifndef READERTYPE_H
#define READERTYPE_H

#include <QContextMenuEvent>
#include <QDialog>
#include <QMenu>
#include <QSqlRelationalTableModel>
#include <QWidget>


namespace Ui {
class ReaderType;
}

class ReaderType : public QDialog
{
    Q_OBJECT

public:
    explicit ReaderType(QWidget *parent = nullptr);
    ~ReaderType();

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void on_actionSubmit_triggered();

    void on_actionNew_triggered();

    void on_actionRemove_triggered();

private:
    Ui::ReaderType *ui;

    QSqlRelationalTableModel* model;

    QMenu* menu;
};

#endif // READERTYPE_H
