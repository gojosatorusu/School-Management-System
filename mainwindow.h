#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtSql/QSql>
#include<QtSql/QSqlDatabase>
#include"reguser.h"
#include"adminer.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Log_in_button_clicked();

    void on_teacherNameInput_textChanged(const QString &arg1);

private:

    QSqlDatabase *sqlbase ;
    Ui::MainWindow *ui;
    RegUser* reguser;
    Adminer* Admin;
};

#endif // MAINWINDOW_H
