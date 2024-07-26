#ifndef SEARCHTEACHER_H
#define SEARCHTEACHER_H

#include <QDialog>
#include "Entity.h"
#include <QListWidgetItem>

namespace Ui {
class SearchTeacher;
}

class SearchTeacher : public QDialog
{
    Q_OBJECT

public:
    explicit SearchTeacher(QWidget *parent = nullptr, School* = nullptr, Course* = nullptr);
    ~SearchTeacher();
    void clearteachersWiget();
    void load_teacher_to_list_Widget(Teacher* teacher);
    void load_Teachers_To_ListWidget(QList<Teacher*> teachers);

private slots:
    void on_TeacherListWidget_itemClicked(QListWidgetItem *item);

    void on_TeachersearchEdit_textChanged( QString arg1);

private : signals:
    void AddedTeacherToCourse(Teacher *);

private:
    Ui::SearchTeacher *ui;
    School* school;
    Course* course;
};

#endif // SEARCHTEACHER_H
