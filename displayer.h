#ifndef DISPLAYER_H
#define DISPLAYER_H

#include <QDialog>
#include<QWidget>
#include<QListWidgetItem>
#include"Entity.h"
#include "changestudentgroup.h"

namespace Ui {
class Displayer;
}

class Displayer : public QDialog
{
    Q_OBJECT

public:
    explicit Displayer(QWidget *parent = nullptr,Entity* = nullptr,QListWidgetItem* = nullptr, QSqlDatabase *  = nullptr, School* = nullptr, EntityType = EntityType::Admin);
    ~Displayer();



private slots:
    void on_Savebutton_clicked();

    void on_Imagebutton_clicked();

    void on_ChangeGroup_PasswordButton_clicked();


    void on_DeleteStudent_teacherButton_clicked();

private: signals:
    void removeStudent();
    void removeteacher();
private:
    Ui::Displayer *ui;
    QSqlDatabase * Sqlbase;
    Entity* item;
    QListWidgetItem* ListItem;
    School * school;
    ChangeStudentGroup* Changer;
    EntityType id;
};

#endif // DISPLAYER_H
