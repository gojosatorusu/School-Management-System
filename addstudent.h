#ifndef ADDSTUDENT_H
#define ADDSTUDENT_H

#include <QDialog>
#include "Entity.h"
namespace Ui {
class addStudent;
}

class addStudent : public QDialog
{
    Q_OBJECT

public:
    explicit addStudent(QWidget *parent = nullptr,School* = nullptr, EntityType = EntityType::Student);
    ~addStudent();

private slots:
    void on_AddButton_clicked();
public : signals:
    void addingTeacher(QString);
    void addingStudent(QString);

private:
    Ui::addStudent *ui;
    School* school;
    EntityType id;
};

#endif // ADDSTUDENT_H
