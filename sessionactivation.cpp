#include "sessionactivation.h"
#include "ui_sessionactivation.h"
#include <QMessageBox>
SessionActivation::SessionActivation(QWidget *parent, School * schoolptr, Course* courseptr)
    : QDialog(parent)
    , ui(new Ui::SessionActivation)
    ,course(courseptr)
    ,school(schoolptr)
{
    ui->setupUi(this);
    if(course->hasLecture())
    {
        ui->LectureCheckBox->setChecked(true);
        ui->numberofLecture->setText(QString::number(course->getNumberofLectureSessions()));
    }
    else
    {
        ui->numberofLecture->setReadOnly(true);
    }
    if(course->hasLab())
    {
        ui->LabCheckBox->setChecked(true);
        ui->numberOflabs->setText(QString::number(course->getNumberofLabSessions()));
    }
    else
    {
        ui->numberOflabs->setReadOnly(true);
    }
    if(course->hasTutorial())
    {
        ui->TutorialCheckBox->setChecked(true);
        ui->NumberofTutorials->setText(QString::number(course->getNumberofTutorialSessions()));
    }
    else
    {
        ui->NumberofTutorials->setReadOnly(true);
    }
}

SessionActivation::~SessionActivation()
{
    delete ui;
}

void SessionActivation::on_pushButton_clicked()
{
    QMessageBox::StandardButton message = QMessageBox::question(this,"Save ? ","Would you like to save the changes ? this will cause data loss for the sessions you have deleted, and if you change the number of sessions you are risking of deleting the Attendance of the teachers and students of that Session type !!!!!",QMessageBox::Yes | QMessageBox::No);
    if (message == QMessageBox::Yes )
    {
        if( ui->LectureCheckBox->isChecked())
        {
            QString Number_Of_Lecture_sessions = ui->numberofLecture->text();
            QRegularExpression re("^(0?[1-9]|[1-9][0-9])$");
            if(re.match(Number_Of_Lecture_sessions).hasMatch())
            {
                course->setNumberofLectureSessions(Number_Of_Lecture_sessions.toInt());
                school->updateLectureSessionNumberinDatabase(course);
            }
            if(!course->hasLecture())
            {
                school->AddLectureSessiontoDatabase(course);
            }
        }
        else if(course->hasLecture())
        {
            school->removeLectureSessionfromDatabase(course);
        }

        if( ui->TutorialCheckBox->isChecked())
        {
            QString Number_Of_Tutorial_sessions = ui->NumberofTutorials->text();
            QRegularExpression re("^(0?[1-9]|[1-9][0-9])$");
            if(re.match(Number_Of_Tutorial_sessions).hasMatch())
            {
                course->setNumberofTutorialSessions(Number_Of_Tutorial_sessions.toInt());
                school->updateTutorialSessionNumberinDatabase(course);
            }
            if(!course->hasTutorial())
            {
                school->AddTutorialSessionstoDatabase(course);
            }
        }
        else if(course->hasTutorial())
        {
            school->removeTutorialSessionfromDatabase(course);
        }

        if( ui->LabCheckBox->isChecked())
        {
            QString Number_Of_Lab_sessions = ui->numberOflabs->text();
            QRegularExpression re("^(0?[1-9]|[1-9][0-9])$");
            if(re.match(Number_Of_Lab_sessions).hasMatch())
            {
                course->setNumberofLabSessions(Number_Of_Lab_sessions.toInt());
                school->updateLabSessionNumberinDatabase(course);
            }

            if(!course->hasLab())
            {
                school->AddLabSessionstoDatabase(course);
            }

        }
        else if(course->hasLab())
        {
            school->removeLabSessionsfromDatabase(course);
        }
        course->UpdateCourseinDatabase();
    }
    this->deleteLater();
}

void SessionActivation::on_LectureCheckBox_stateChanged(int arg1)
{
    ui->numberofLecture->setReadOnly(arg1 == Qt::Unchecked);
}

void SessionActivation::on_LabCheckBox_stateChanged(int arg1)
{
    ui->numberOflabs->setReadOnly(arg1 == Qt::Unchecked);
}

void SessionActivation::on_TutorialCheckBox_stateChanged(int arg1)
{
    ui->NumberofTutorials->setReadOnly(arg1 == Qt::Unchecked);
}

