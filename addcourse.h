#ifndef ADDCOURSE_H
#define ADDCOURSE_H

#include <QDialog>
#include "Entity.h"
namespace Ui {
class AddCourse;
}

class AddCourse : public QDialog
{
    Q_OBJECT

public:
    explicit AddCourse(QWidget *parent = nullptr, School* = nullptr);
    ~AddCourse();

private slots:
    void on_LectureCheckBox_stateChanged(int arg1);
    void on_LabCheckBox_stateChanged(int arg1);
    void on_TutorialCheckBox_stateChanged(int arg1);
    void on_Addbutton_clicked();

public: signals:
    void CourseAdded();

private:
    Ui::AddCourse *ui;
    School* school;
};

#endif // ADDCOURSE_H
