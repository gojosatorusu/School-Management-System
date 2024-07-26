#include "courseeditor.h"
#include "ui_courseeditor.h"
#include <QMessageBox>
#include "sessionactivation.h"
#include "sessionhandler.h"
#include "searchteacher.h"
CourseEditor::CourseEditor(QWidget *parent, School* schoolptr , Course* courseptr)
    : QDialog(parent)
    , ui(new Ui::CourseEditor)
    ,school(schoolptr)
    ,course(courseptr)
{
    school->loadTeacherAttendance(course);
    school->loadStudentAttendance(course);
    ui->setupUi(this);
    ui->CourseNameEdit->setText(course->getCourseName());
    ui->CourseIDedit->setText(QString::number(course->getID()));
    ui->Semesteredit->setText(QString::number(course->getsemester()));
    ui->Coefedit->setText(QString::number(course->getCoef()));
    for ( auto teacher : course->getCourseTeacher())
    {
        QListWidgetItem* item = new QListWidgetItem(teacher->getName() + " ("+QString::number(teacher->getID()) +")");
        item->setSizeHint(QSize(10, 20));
        ui->TeacherListWidget->addItem(item);
    }
}

CourseEditor::~CourseEditor()
{
    school->EmptyCourseTeachersAttendance(course);
    delete ui;
}

void CourseEditor::on_DeleteCourseButton_clicked()
{
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm Deletion",
        QString("Are you sure you want to delete the following Course : '%1'?").arg(course->getCourseName()),
        QMessageBox::Yes | QMessageBox::No
        );

    if (confirm == QMessageBox::Yes) {
        qDebug()<<" You are Here !! 1";
        school->deleteCourseDefinitively(course); // Implement this function to delete the group from the data structure or database
        emit this->courseDeleted();
    }
    this->deleteLater();
}


void CourseEditor::on_LectureButton_clicked()
{
    if( !course->hasLecture())
    {
        QMessageBox::information(this,"NO Lecture !! ","this Course Has No Lectures");
    }
    else
    {
        SessionHandler* session = new SessionHandler(this,school,course,SessionType::Lecture);
        session->show();

    }
}


void CourseEditor::on_TutorialButton_clicked()
{
    if( !course->hasTutorial())
    {
        QMessageBox::information(this,"NO Tutorial !! ","this Course Has No Tutorials");
    }
    else
    {
        SessionHandler* session = new SessionHandler(this,school,course,SessionType::Tutorial);
        session->show();
    }
}


void CourseEditor::on_LabButton_clicked()
{
    if( !course->hasLab())
    {
        QMessageBox::information(this,"NO Lab !! ","this Course Has No Labs");
    }
    else
    {
        SessionHandler* session = new SessionHandler(this,school,course,SessionType::Lab);
        session->show();
    }
}


void CourseEditor::on_SessionActivation_clicked()
{
    SessionActivation* activate = new SessionActivation(this,school,course);
    activate->show();
}


void CourseEditor::on_AddTeacherToCourseButton_clicked()
{
    SearchTeacher * AddTeacher = new SearchTeacher(this,school,course);
    AddTeacher->show();
    connect(AddTeacher,&SearchTeacher::AddedTeacherToCourse, this, [this](Teacher * teacher)
    {
        ui->TeacherListWidget->addItem(teacher->getName() + " (" + QString::number(teacher->getID()) +")" );

    });
}


void CourseEditor::on_RemoveTeacherFromCourse_clicked()
{
    QMessageBox::StandardButton message = QMessageBox::question(this,"remove Teacher ?"," Would you like to remove the Selected Teachers from the Course Teachers ? ",QMessageBox::Yes | QMessageBox::No);
    if ( message == QMessageBox::Yes)
    {
        QList<QListWidgetItem*> Items = ui->TeacherListWidget->selectedItems();
        if( !Items.isEmpty())
        {
            Teacher* teacher;
            for (auto item : Items)
            {
                teacher = school->findTeacherByName(item->text());
                course->removeteacherFromCourse(teacher);
                // this will empty the teacher Attendance before removing him from the Teachers of the Course

                teacher->removeLabAttendance();
                teacher->removeLectureAttendance();
                teacher->removeTutorialAttendance();

                course->removeCourseTeacherFromDatabase(teacher);
                delete item;
            }

        }
    }
}

void CourseEditor::on_SaveButton_clicked()
{
    QString name = ui->CourseNameEdit->text();
    bool semesterok (true);
    bool coefok(true);
    int semester = ui->Semesteredit->text().toInt(&semesterok);
    int coef = ui->Coefedit->text().toInt(&coefok);
    course->setCourseName(name);
    emit this->CourseNameHasBeenChanged(name);
    if ( semesterok)
    {
        course->setSemester(semester);
    }
    if( coefok )
    {
        course->setCoef(coef);
    }
    course->UpdateCourseinDatabase();
}

