#include "addcourse.h"
#include "ui_addcourse.h"

#include <QMessageBox>
AddCourse::AddCourse(QWidget *parent, School * schoolptr)
    : QDialog(parent)
    , ui(new Ui::AddCourse)
    ,school(schoolptr)
{
    ui->setupUi(this);
    ui->NumberofLabedit->setReadOnly(true);
    ui->NumberofLectureedit->setReadOnly(true);
    ui->Numberoftutorialedit->setReadOnly(true);
}

AddCourse::~AddCourse()
{
    delete ui;
}

void AddCourse::on_LectureCheckBox_stateChanged(int arg1)
{
    ui->NumberofLectureedit->setReadOnly(arg1 == Qt::Unchecked);
}


void AddCourse::on_LabCheckBox_stateChanged(int arg1)
{
    ui->NumberofLabedit->setReadOnly(arg1 == Qt::Unchecked);
}


void AddCourse::on_TutorialCheckBox_stateChanged(int arg1)
{
    ui->Numberoftutorialedit->setReadOnly(arg1 == Qt::Unchecked);
}


void AddCourse::on_Addbutton_clicked()
{
    bool isvalid = true;
    QString name = ui->NameLineedit->text();
    QString ID = ui->Idedit->text();
    QString coef = ui->coefedit->text();
    QString semester = ui->semesteredit->text();
    QString year = ui->yearedit->text();
    QMessageBox::StandardButton message;


    QRegularExpression re("^[A-Za-zÀ-ÖØ-öø-ÿ ]+([ '-][A-Za-zÀ-ÖØ-öø-ÿ ]+)*[0-9]*$");
    if (! re.match(name).hasMatch())
    {
        QMessageBox::warning(this,"Warning!!","the Name is not Valid here!!");
        qDebug()<<"the chosen wrong name is "<<name;
        isvalid = false;
    }

    re.setPattern("^[0-9]+$");
    if (! re.match(ID).hasMatch())
    {
        QMessageBox::warning(this,"Warning!!","the ID is not Valid here!!");
        isvalid = false;
    }

    re.setPattern("^[1-9]$");
    if (! re.match(coef).hasMatch())
    {
        QMessageBox::warning(this,"Warning!!","the coefficient Number is not Valid here!!");
        isvalid = false;
    }

    re.setPattern("^[1-5]$");// to match any email of the form : something@SchoolName.edu.country_web_domain_name
    if (! re.match(year).hasMatch())
    {
        QMessageBox::warning(this,"Warning!!","the year is not Valid here!!");
        isvalid = false;
    }

    re.setPattern("^[1-9]$");
    if (! re.match(semester).hasMatch())
    {
        QMessageBox::warning(this,"Warning!!","the Gender is not Valid here!!");
        isvalid = false;
    }



    QString number_of_Lecture_sessions;
    QString number_of_Lab_sessions;
    QString number_of_tutorial_sessions;
    QString state="";

    if (isvalid)
    {
        if( ui->LectureCheckBox->isChecked())
        {
            state+="1";
            number_of_Lecture_sessions= ui->NumberofLectureedit->text();
            re.setPattern("^(0?[1-9]|[1-9][0-9])$");
            if (! re.match(number_of_Lecture_sessions).hasMatch())
            {
                QMessageBox::warning(this,"Warning!!","the number of lecture sessions is not Valid here!!");
                isvalid = false;
            }

        }
        else
        {
            state+="0";
            number_of_Lecture_sessions= "0";
        }
            qDebug()<<" after lecture "+state;


        if( ui->LabCheckBox->isChecked())
        {
            state+="1";
            number_of_Lab_sessions= ui->NumberofLabedit->text();
            re.setPattern("^(0?[1-9]|[1-9][0-9])$");
            if (! re.match(number_of_Lab_sessions).hasMatch())
            {
                QMessageBox::warning(this,"Warning!!","the number of Lab sessions is not Valid here!!");
                isvalid = false;
            }
        }
        else
        {
            state+="0";
            number_of_Lab_sessions= "0";
        }
        qDebug()<<" After Lab" +state;

        if(ui->TutorialCheckBox->isChecked())
        {
            state+="1";
            number_of_tutorial_sessions= ui->Numberoftutorialedit->text();
            re.setPattern("^(0?[1-9]|[1-9][0-9])$");
            if (! re.match(number_of_tutorial_sessions).hasMatch())
            {
                QMessageBox::warning(this,"Warning!!","the number of Tutorial sessions is not Valid here!!");
                isvalid = false;
            }
        }
        else
        {
            state+="0";
            number_of_tutorial_sessions= "0";
        }
        qDebug()<<" After Tutorial" <<state;
    }


    if ( isvalid)
    {
        for(int i = 0 ; i < 5 ; i++)
        {
            for ( auto course : school->getYears()[i].getCourses())
            {
                if ( course->getCourseName() == name)
                {
                    QMessageBox::warning(this,"Warning !!!"," This Name is ALREADY in USE");
                    return;
                }
                if ( course->getID() == ID.toInt())
                {
                    message = QMessageBox::question(this,"Warning !!!"," This ID is ALREADY in USE, Would You Like the System to assign the ID instead ?");
                    if (message == QMessageBox::Yes)
                    {
                        ID = "-1";
                    }
                    else
                    {
                        return;
                    }
                }
            }
        }
        Course* course = new Course(school->getdatabase(),ID.toInt(),name,coef.toInt(),number_of_tutorial_sessions.toInt(),number_of_Lab_sessions.toInt(),school->getTeachers(),number_of_Lecture_sessions.toInt(),semester.toInt(),state,year.toInt());
        school->AddCourseTodatabase(course);
        QMessageBox::information(this,"Success","Course Added Successfully");
        this->deleteLater();

        school->setAddedCourse(course);
        emit CourseAdded();
        accept();

    }
}

