#include "addstudent.h"
#include "ui_addstudent.h"
#include <QRegularExpression>
#include <QMessageBox>
addStudent::addStudent(QWidget *parent, School* schoolptr, EntityType identifier)
    : QDialog(parent)
    , ui(new Ui::addStudent)
    , id (identifier)
    , school(schoolptr)
{
    ui->setupUi(this);
    if( id == EntityType::Teacher)
    {
        ui->GroupLabel->hide();
        ui->GrouplineEdit->hide();
        ui->Password_yearlabel->setText("Password");
    }
    else
    {
        ui->Password_yearlabel->setText("Year");
    }
}

addStudent::~addStudent()
{
    delete ui;
}

void addStudent::on_AddButton_clicked() // we will do the verification and validation, we create a Student object. we create a function that inserts into the database the Students and pass the student object to it. also remember to change the delete Student function or delete group, and make it delete not based on the database but on the IDs
{
    bool isvalid = true;
    QString name = ui->NamelineEdit->text();
    QString ID = ui->IDlineEdit->text();
    QString phonenumber = ui->PhoneNumberlineEdit->text();
    QString email = ui->EmaillineEdit->text();
    QString Gender = ui->GenderlineEdit->text();
    QMessageBox::StandardButton message;


    QRegularExpression re("^[A-Za-zÀ-ÖØ-öø-ÿ]+([ '-][A-Za-zÀ-ÖØ-öø-ÿ]+)*$");
    if (! re.match(name).hasMatch())
    {
        message=QMessageBox::warning(this,"Warning!!","the Name is not Valid here!!");
        qDebug()<<"the chosen wrong name is "<<name;
        isvalid = false;
    }

    re.setPattern("^[0-9]+$");
    if (! re.match(ID).hasMatch())
    {
        message=QMessageBox::warning(this,"Warning!!","the ID is not Valid here!!");
        isvalid = false;
    }

    re.setPattern("^[0-9]{10}$");
    if (! re.match(phonenumber).hasMatch())
    {
        message=QMessageBox::warning(this,"Warning!!","the Phone Number is not Valid here!!");
        isvalid = false;
    }

    re.setPattern("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");// to match any email of the form : something@SchoolName.edu.country_web_domain_name
    if (! re.match(email).hasMatch())
    {
        message=QMessageBox::warning(this,"Warning!!","the E-Mail is not Valid here!!");
        isvalid = false;
    }

    re.setPattern("^[FM]$");
    if (! re.match(Gender).hasMatch())
    {
        message=QMessageBox::warning(this,"Warning!!","the Gender is not Valid here!!");
        isvalid = false;
    }



    if ( id == EntityType::Student)
    {
        QString year = ui->Password_YearlineEdit->text();
        re.setPattern("^[1-5]$");
        if (! re.match(year).hasMatch())
        {
            message=QMessageBox::warning(this,"Warning!!","the Year is not Valid here!!");
            isvalid = false;
        }
        else
        {
            int Year = year.toInt();
            QString group = ui->GrouplineEdit->text();
            re.setPattern("^[0-9][0-9]?$");
            if (! re.match(group).hasMatch())
            {
                message=QMessageBox::warning(this,"Warning!!","the Group is not Valid here!!");
                qDebug()<<"the chosen wrong year is : "<<Year;
                isvalid = false;
            }
            else
            {
                int Group = group.toInt();
                if (!school->groupExists(Year,Group))
                {
                    message=QMessageBox::warning(this,"Warning!!","the Group Does not Exist, Create it first!!");
                    qDebug()<<"the chosen wrong group is : "<<Group;
                    isvalid = false;
                }
                else
                {
                    if (isvalid)
                    {
                        for (int i = 0 ; i < 5; i++)
                        {
                            for ( auto group : school->getYears()[i].getGroups())
                            {
                                for( auto student : group->getGroupStudents())
                                {
                                    if ( student->getID() == ID.toInt())
                                    {
                                        message = QMessageBox::warning(this,"Warning !!!"," This ID is ALREADY in USE !!!!");
                                        return;
                                    }
                                }
                            }
                        }
                        Student* student = new Student(school->getdatabase(),name,email,phonenumber,ID.toInt(),Year,Group,0,"",Gender,"Student",false);//QSqlDatabase* sqlbase,QString name, QString email, QString phoneNumber, int id, int year, int Group,int Sqlbaseid, QString path,QString gender, QString Position,bool deletion
                        school->AddStudentTodatabase(student);
                        emit this->addingStudent( name + " (" + ID + ")" );
                        QMessageBox::information(this,"Success","Student Added Successfully");
                        this->deleteLater();
                    }
                }
            }
        }
    }
    else
    {
        QString password = ui->Password_YearlineEdit->text();
        re.setPattern("^[A-Za-z0-9!?;:&é]+$");
        if (! re.match(password).hasMatch())
        {
            QMessageBox::warning(this,"Warning!!","the Password is not Valid here!!");
            isvalid = false;
        }
        else
        {
            if (isvalid)
            {    for ( auto teacher : school->getTeachers())
                {
                    if ( teacher->getID() == ID.toInt())
                    {
                        QMessageBox::warning(this,"Warning !!!"," This ID is ALREADY in USE !!");
                        return ;
                    }
                }
                Teacher* teacher = new Teacher(school->getdatabase(),name,email,phonenumber,ID.toInt(),password,0,"",Gender,0); // QSqlDatabase* sqlbase,QString name, QString email, QString phoneNumber, int id,QString password,int Sqlbaseid,QString path,QString gender,bool deletion
                school->AddTeacherTodatabase(teacher);
                emit this->addingTeacher( name + " (" + ID + ")" );
                QMessageBox::information(this,"Success","Teacher Added Successfully");
                this->deleteLater();
            }
        }
    }

}


