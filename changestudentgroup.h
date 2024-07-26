#ifndef CHANGESTUDENTGROUP_H
#define CHANGESTUDENTGROUP_H
#include "Entity.h"
#include <QDialog>

namespace Ui {
class ChangeStudentGroup;
}

class ChangeStudentGroup : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeStudentGroup(QWidget *parent = nullptr, School* = nullptr,Student* = nullptr);
    ~ChangeStudentGroup();

private slots:
    void on_yearcombobox_activated(int index);

    void on_pushButton_clicked();

private:
    Ui::ChangeStudentGroup *ui;
    School* school;
    Student* student;
};

#endif // CHANGESTUDENTGROUP_H
