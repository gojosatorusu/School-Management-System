#ifndef RECYCLEBIN_H
#define RECYCLEBIN_H

#include <QDialog>
#include "Entity.h"

namespace Ui {
class Recyclebin;
}

class Recyclebin : public QDialog
{
    Q_OBJECT

public:
    explicit Recyclebin(QWidget *parent = nullptr, School* = nullptr, EntityType =EntityType::Student);
    ~Recyclebin();
    void loaddeletedstudents();
    void loaddeletedteachers();
    void load_student_to_list_Widget(Student*);
    void load_teacher_to_list_Widget(Teacher* teacher);
    void clearstudentsWiget();
    void clearteachersWiget();
private slots:

    void on_restorebutton_clicked();

    void on_deletebutton_clicked();

    void on_Student_teacherSearchEdit_textChanged(QString arg1);

    void on_refresh_clicked();

public: signals:
    void studentrecovery();
    void teacherrecovery(QString);
private:
    Ui::Recyclebin *ui;
    School* school;
    EntityType id;
};

#endif // RECYCLEBIN_H
