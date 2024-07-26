#ifndef SESSIONHANDLER_H
#define SESSIONHANDLER_H

#include <QDialog>
#include <QApplication>
#include <QTableWidget>
#include <QComboBox>
#include <QList>
#include <QMap>
#include "Entity.h"
#include <QListWidgetItem>
namespace Ui {
class SessionHandler;
}

class SessionHandler : public QDialog
{
    Q_OBJECT

public:
    explicit SessionHandler(QWidget *parent = nullptr, School* = nullptr , Course* = nullptr, SessionType = SessionType::Lecture);
    ~SessionHandler();
    void setupTeacherAssignmentTable();
    void loadTeacher(Teacher * teacher);
    void loadLectureTeacherAttendancetoTableWidget();
    void loadTutorialTeacherAttendancetoTableWidget();
    void loadLabTeacherAttendancetoTableWidget();
    void setAttendanceTableWidget();
    //
    void updateTableWidgetLectureTeacherAttendance(int,int row,Teacher* teacher);
    void updateTableWidgetLabTeacherAttendance(int,int row,Teacher* teacher);
    void updateTableWidgetTutorialTeacherAttendance(int,int row,Teacher* teacher);
    int TableWidgetTableRow(int groupNumber);
    void updateAttendanceStructure();


private slots:
    void on_GroupComboBox_activated(int index);

    void on_TeacherListWidget_itemClicked(QListWidgetItem *item);


    void on_UnassignButton_clicked();

private:
    Ui::SessionHandler *ui;
    School* school;
    Course* course;
    SessionType id;
};

#endif // SESSIONHANDLER_H
