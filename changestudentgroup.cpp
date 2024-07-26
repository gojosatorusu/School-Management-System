#include "changestudentgroup.h"
#include "ui_changestudentgroup.h"
#include<QMessageBox>

ChangeStudentGroup::ChangeStudentGroup(QWidget *parent,School* Schoolptr,Student* studentptr)
    : QDialog(parent)
    , ui(new Ui::ChangeStudentGroup)
    ,school(Schoolptr)
    ,student(studentptr)
{
    ui->setupUi(this);
}

ChangeStudentGroup::~ChangeStudentGroup()
{
    delete ui;
}


void ChangeStudentGroup::on_yearcombobox_activated(int index)
{
    ui->Groupcombobox->clear();
    ui->Groupcombobox->addItem("Group");
    QVector<int> GroupNumber;
    if ( index != 0)// for instance :this will select the first years so here we must set the Group combobox to the Groupnumbers of the first years
    {
        for (auto group : (school->getYears()[index-1].getGroups()) )
        {
            GroupNumber.append(group->getGroupNumber());
        }
        std::sort(GroupNumber.begin(),GroupNumber.end());
        for (auto groupNumber : GroupNumber )
        {
            ui->Groupcombobox->addItem(QString::number(groupNumber));
        }
    }
}




void ChangeStudentGroup::on_pushButton_clicked() // this will work as a save functionality
{
    if (ui->yearcombobox->currentIndex() != 0 && ui->Groupcombobox->currentIndex() != 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Save?", "Would you like to change student " + student->getName() + "'s year and group to : Y" + ui->yearcombobox->currentText() + "G" + ui->Groupcombobox->currentText(), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Remove the student from the original group
            QList<Group*> groups = school->getYears()[student->getYear()-1].getGroups();
            for (auto group : groups) {
                if (group->getGroupNumber() == student->getgroup()) {
                    group->removeStudent(student);
                    qDebug() << "Student removed from the original group successfully";
                    break; // Exit the loop once the student is removed
                }
            }

            // Update the student's year and group
            int newYear = ui->yearcombobox->currentText().toInt();
            int newGroupNumber = ui->Groupcombobox->currentText().toInt();
            groups = school->getYears()[newYear-1].getGroups();
            student->setyear(newYear);
            student->setgroup(newGroupNumber);

            // Add the student to the new group
            for (auto group : groups) {
                if (group->getGroupNumber() == newGroupNumber) {
                    group->addStudent(student);
                    qDebug() << "Student added to the chosen group successfully";
                    break; // Exit the loop once the student is added
                }
            }

            // Print the updated list of students in the new group
            for (auto group : groups) {
                qDebug()<<"Group : "<<group->getGroupNumber();
                for (auto student : group->getGroupStudents()) {
                    qDebug() << "Group student:" << student->getName();

                }
            }

            // Update the student in the database
            student->update_Student(school->getdatabase());
            this->deleteLater();
        } else {
            delete this;
        }
    }
}

