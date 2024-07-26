#ifndef ADMINER_H
#define ADMINER_H

#include <QMainWindow>
#include<QtSql>
#include"Entity.h"
#include<QListWidget>

class MainWindow;
namespace Ui {
class Adminer;
}

class Adminer : public QMainWindow
{
    Q_OBJECT

public:
    explicit Adminer(QWidget *parent = nullptr, MainWindow* = nullptr, QSqlDatabase* = nullptr);
    void handleVerticalHeaderClicked(int );
    void setTheExamSchedule();
    void setGroupsSchedule(Group *group);
    void setStudentsGradesTableWidget(Group *group);
    void loadStudentExamToTableWidget(int row,Student * student);


    ~Adminer();

private slots:
    void on_addtimeslotbutton_clicked();
    void on_removetimeslotbutton_clicked();
    void on_LogOutbutton_clicked();

    //loaders
    void load_Teachers_To_ListWidget( QList<Teacher*> );
    void load_teacher_to_list_Widget(Teacher* );
    void load_Students_To_ListWidget( Group*);
    void load_student_to_list_Widget(Student*);
    void load_Groups_To_ListWidget(Year*);
    void load_Course_To_ListWidget(Course*);
    void load_Year_Courses_To_ListWidget(Year*);
    //will handle the action of clicking on an item (will display a profile of the Entity)
    void on_TeacherListWidget_itemClicked(QListWidgetItem *item);
    void on_studentsListWidget_itemClicked(QListWidgetItem *item);
    //used to find teachers and students using their names
    // used to control the Students' year and group comboboxes
    void on_yearcomboBox_activated(int index);
    void on_groupCombobox_activated(int index);
    //used to clear the List Widgets
    void clearstudentsWiget();
    void clearteachersWiget();
    void clearCoursesWidget();
    void clearGroupsWidget();
    // search mechanisms
    void on_TeachersearchEdit_textChanged( QString );

    void on_StudentSearchEdit_textChanged(QString );


    void on_AddTeacherbutton_clicked();

    void on_GroupListWidget_itemClicked(QListWidgetItem *item);

    void on_Addgroupbutton_clicked();

    //GroupListWisget manipulators
    void addAddedgroupitemtolistWidget();
    void remove_Student_TeacherFromListWidget(Entity*);
    void on_StudentRecycleBinbutton_clicked();

    void on_TeacherRecycleBinbutton_clicked();


    void on_refreshTeachers_clicked();

    void on_refreshstudents_clicked();

    void on_AddStudentbutton_clicked();

    void on_refreshGroups_clicked();

    void on_CourseslistWidget_itemClicked(QListWidgetItem *item);

    void on_Addcoursebutton_clicked();

    void addAddedCourseitemtolistWidget();

    void on_CoursesearchEdit_textChanged(QString arg1);

    void on_refreshCoursesButton_clicked();


    void on_ExamYearComboBox_activated(int index);


    void on_ExamDateTableWidget_cellChanged(int row, int column);

    void on_ExamDateTableWidget_cellClicked(int row, int column);

    void on_refreshExamButton_clicked();

    void on_yearcombobox_Schedule_activated(int index);

    void on_groupcombobox_Schedule_activated(int index);

    void on_CourseCombobox_activated(int index);

    void on_GroupComboBox_activated(int index);

private:
    Ui::Adminer *ui;
    QWidget* login_interface;
    QStringList Days;
    QStringList Time_Slots;
    QSqlDatabase * Sqlbase;
    int* selected_row; // used in the selection of a row in the schedule.
    School* school;
    QString SelectedTextforExam;// this will hold the previous text ( date or time in a certain cell)
    QString SelectedYear_GroupSchedule;
};



#endif // ADMINER_H
