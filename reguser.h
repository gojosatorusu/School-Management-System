#ifndef REGUSER_H
#define REGUSER_H
#include<QtSql>
#include <QMainWindow>
#include<QTableWidget>
#include<QListWidget>
#include "Entity.h"
#include <Set>
using std::set;
class MainWindow;
namespace Ui {
class RegUser;
}

class RegUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegUser(QWidget *parent = nullptr, MainWindow* = nullptr , QString = "", QSqlDatabase* = nullptr);
    ~RegUser();
    void load_student_to_list_Widget(Student*);
    void load_Students_To_ListWidget( Group* group);
    void clearstudentsWiget();
    void setAttendanceTableWidget();

    void loadStudentLectureAttendance();
    void loadStudentLabAttendance();
    void loadStudentTutorialAttendance();

    void updateTableWidgetLectureStudentAttendance(int row,Student* student);
    void updateTableWidgetLabStudentAttendance(int row,Student* student);
    void updateTableWidgetTutorialStudentAttendance(int row,Student* student);

    void updateAttendanceStructure();

    void setStudentsGradesTableWidget();
    void loadStudentExamToTableWidget(int row, Student * student);
    void UpdateGradesStructure(Group *group);

    void setTeacherScheuleWidget();
    void saveTeacherScheuleWidget();

    void setStudentsExamTableWidgetWidget();
private slots:
    void on_LogOutbutton_clicked();

    void on_addtimeslotbutton_clicked();

    void on_removetimeslotbutton_clicked();
    void handleVerticalHeaderClicked(int );


    void on_CoursesCombobox_activated(int index);

    void on_GroupCombobox_activated(int index);

    void on_StudentListWidget_itemClicked(QListWidgetItem *item);

    void on_RoleCombobox_activated();


private:
    Ui::RegUser *ui;
    Teacher* teacher;
    Course* course;
    Group* group ;
    QSqlDatabase* Sqlbase;
    QWidget* login_interface;
    QStringList Days;
    QStringList Time_Slots;
    int* selected_row;
    School* school;
    QVector<int> LectureGroups;
    QVector<int> TutorialGroups;
    QVector<int> LabGroups;
    set<int> Groups;
    SessionType id;
};

#endif // REGUSER_H
