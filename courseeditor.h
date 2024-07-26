#ifndef COURSEEDITOR_H
#define COURSEEDITOR_H

#include <QDialog>
#include "Entity.h"
namespace Ui {
class CourseEditor;
}

class CourseEditor : public QDialog
{
    Q_OBJECT

public:
    explicit CourseEditor(QWidget *parent = nullptr, School* = nullptr , Course* = nullptr);
    ~CourseEditor();

private:
    Ui::CourseEditor *ui;
    School* school;
    Course* course;
public: signals:
    void CourseNameHasBeenChanged(QString);
    void courseDeleted();
private slots:
    void on_DeleteCourseButton_clicked();
    void on_LectureButton_clicked();
    void on_TutorialButton_clicked();
    void on_LabButton_clicked();
    void on_SessionActivation_clicked();
    void on_AddTeacherToCourseButton_clicked();
    void on_RemoveTeacherFromCourse_clicked();
    void on_SaveButton_clicked();
};

#endif // COURSEEDITOR_H
