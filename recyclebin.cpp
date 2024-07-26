#include "recyclebin.h"
#include "ui_recyclebin.h"
#include <QMessageBox>
#include "changestudentgroup.h"
Recyclebin::Recyclebin(QWidget *parent, School* schoolptr,EntityType identifier) // this will contain procedures for both teachers and students
    : QDialog(parent)
    , ui(new Ui::Recyclebin)
    , school(schoolptr)
    , id (identifier)
{
    ui->setupUi(this);
    if (id == EntityType::Student)
    {
        ui->listWidget->item(0)->setText("Students");
        loaddeletedstudents();
    }
    else if ( id == EntityType::Teacher)
    {
        ui->listWidget->item(0)->setText("Teachers");
        loaddeletedteachers();
    }
}

Recyclebin::~Recyclebin()
{
    delete ui;
}

void Recyclebin::loaddeletedstudents()
{
    for ( auto student : school->getRemovedStudents())
    {
        load_student_to_list_Widget(student);
    }

}
void Recyclebin::loaddeletedteachers()
{
    for (auto teacher : school->getTeachers())
    {
        if ( teacher->getdeletion_State())
            load_teacher_to_list_Widget(teacher);
    }
}
void Recyclebin::load_student_to_list_Widget(Student* student)
{
    QListWidgetItem* item = new QListWidgetItem(student->getName()+" ("+QString::number(student->getID())+")", ui->listWidget);
    item->setSizeHint(QSize(30, 60));
    ui->listWidget->addItem(item);
}

void Recyclebin::load_teacher_to_list_Widget(Teacher* teacher)
{
    QListWidgetItem* item = new QListWidgetItem(teacher->getName()+" ("+QString::number(teacher->getID())+")", ui->listWidget);
    item->setSizeHint(QSize(30, 60));
    ui->listWidget->addItem(item);
}



void Recyclebin::on_restorebutton_clicked()
{
    QList <QListWidgetItem*> list =ui->listWidget->selectedItems();

    QString text;
    if (id == EntityType::Student)
    {
        if (!list.isEmpty())
        {
            Student* student(nullptr);
            for ( auto item : list)
            {
                text = item->text();
                if ( text != "Students")
                {
                    QMessageBox::StandardButton reply = QMessageBox::question(this, "Restore ?", "Would you like to Restore the following student : " + text, QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        student = school->findStudentByName(text);
                        if ( school->groupExists(student->getYear(),student->getgroup()))
                        {
                            student->setdeletion_State(false);
                            student->update_Student(school->getdatabase());
                            Group * group = school->findGroupByName("Y"+QString::number(student->getYear())+"G"+QString::number(student->getgroup()));
                            school->recoverRemovedstudent(student);
                            if (!group->getGroupStudents().contains(student))// this will be put in a scenario like the following : the deleted student used to belong to group 7, he had been deleted, then the group was deleted. and then group 8 took the number 7 groupNumber, now when we try to recover that student it will try to return the student to group 7 he will be added to the former group 8, but this group 8 in its student list it does not contain this student so we must add it first
                                group->addStudent(student);
                            emit this->studentrecovery();
                            delete item;
                        }
                        else
                        {
                            reply = QMessageBox::question(this, "Warning !! ", "the group that this student used to belong to have been deleted !!  Would you like to change his/her group ? ", QMessageBox::Yes | QMessageBox::No);
                            if ( reply == QMessageBox::Yes )
                            {
                                ChangeStudentGroup* student_group_changer = new ChangeStudentGroup(this,school,student);
                                student_group_changer->show();
                                connect(student_group_changer,&QObject::destroyed ,this,&Recyclebin::on_restorebutton_clicked);
                                emit this->studentrecovery();
                                delete item;
                            }
                            else
                            {
                                return;
                            }

                        }
                    }
                }
            }
        }
    }
    else
    {
        if (!list.isEmpty())
        {
            Teacher* teacher(nullptr);
            for ( auto item : list)
            {
                text = item->text();
                if ( text != "Teachers")
                {
                    QMessageBox::StandardButton reply = QMessageBox::question(this, "Restore ?", "Would you like to Restore the following teacher : " + text, QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        teacher = school->findTeacherByName(text);
                        teacher->setdeletion_State(false);
                        teacher->update_Teacher(school->getdatabase());
                        emit this->teacherrecovery(text);
                        delete item;
                    }
                }
            }
        }

    }

}


void Recyclebin::on_deletebutton_clicked()
{
    QList <QListWidgetItem*> list =ui->listWidget->selectedItems();

    QString text;
    if (id == EntityType::Student)
    {
        if (!list.isEmpty())
        {
            Student* student(nullptr);
            for ( auto item : list)
            {
                text = item->text();
                if ( item->text() != "Students")
                {
                    QMessageBox::StandardButton reply = QMessageBox::question(this, "Restore ?", "Would you like to delete the following student permanently : " + text, QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        student = school->findStudentByName(text);
                        school->recoverRemovedstudent(student);
                        school->removeStudentDefinitively(student);
                        delete item;
                    }
                }
            }
        }
    }
    else
    {
        if (!list.isEmpty())
        {
            Teacher* teacher(nullptr);
            for ( auto item : list)
            {
                text = item->text();
                if ( item->text() != "Teachers")
                {
                    QMessageBox::StandardButton reply = QMessageBox::question(this, "Remove ?", "Would you like to Remove the following teacher permanently : " + text, QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        teacher = school->findTeacherByName(text);
                        school->removeTeacherDefinitively(teacher);
                        delete item;
                    }
                }
            }
        }

    }

}


void Recyclebin::on_Student_teacherSearchEdit_textChanged(QString search)
{
    if ( id == EntityType::Teacher)
    {    if (!search.trimmed().isEmpty())
        {
            qDebug() << "Search string:" << search;
            QRegularExpression re("^#?[a-zA-Z\\s:1-9]+$");// we will be using regular expressions to make comparing and checking if the QString is valid easier
            if (re.match(search).hasMatch())
            {
                clearteachersWiget();
                if (search.startsWith("#"))
                {
                    qDebug() << "The string starts with a #.";
                    qDebug()<<search.remove("#").trimmed();
                    qDebug()<<"search after being trimmed and all : "<<search;                    // Remove and trim leading #
                    QStringList command = search.split(":");
                    qDebug() << "Command:" << command;

                    if (command.size() != 2)
                    {
                        qDebug() << "Invalid command size";
                        return;
                    }

                    QString specifier = command[0].trimmed().toUpper();
                    QString value = command[1].trimmed();
                    qDebug() << "Specifier:" << specifier << "Value:" << value;

                    if (specifier == "ID")
                    {
                        int searchID = value.toInt();
                        for (auto teacher : school->getTeachers())
                        {
                            if (teacher->getID() == searchID && teacher->getdeletion_State() )
                                load_teacher_to_list_Widget(teacher);
                        }
                    }
                    else if (specifier == "NAME")
                    {
                        for (auto teacher : school->getTeachers())
                        {
                            if (teacher->getName().contains(value, Qt::CaseInsensitive) && teacher->getdeletion_State())
                                load_teacher_to_list_Widget(teacher);
                        }
                    }
                    else if (specifier == "EMAIL")
                    {
                        for (auto teacher : school->getTeachers())
                        {
                            if (teacher->getEmail().contains(value, Qt::CaseInsensitive) && teacher->getdeletion_State())
                                load_teacher_to_list_Widget(teacher);
                        }
                    }
                    else
                    {
                        qDebug() << "Unknown specifier:" << specifier;
                    }
                }
                else
                {
                    qDebug() << "Normal search by name";
                    for (auto teacher : school->getTeachers())
                    {
                        if (teacher->getName().contains(search, Qt::CaseInsensitive) && teacher->getdeletion_State())
                            load_teacher_to_list_Widget(teacher);
                    }
                }
            }
            else
            {
                qDebug() << "Invalid search format";
            }
        }
        else {
            clearteachersWiget();
            loaddeletedteachers();
        }
    }
    else
    {
        // Check if the string is empty or contains only whitespace
        if (!search.trimmed().isEmpty())
        {
            qDebug() << "Search string:" << search;
            QRegularExpression re("^#?[a-zA-Z\\s:1-9]+$");// we will be using regular expressions to make comparing and checking if the QString is valid easier
            if (re.match(search).hasMatch())
            {
                clearstudentsWiget();
                if (search.startsWith("#"))
                {
                    qDebug() << "The string starts with a #.";
                    qDebug() << search.remove("#").trimmed();
                    qDebug() << "search after being trimmed and all: " << search; // Remove and trim leading #
                    QStringList command = search.split(":");
                    qDebug() << "Command:" << command;
                    if (command.size() != 2)
                    {
                        qDebug() << "Invalid command size";
                        return;
                    }
                    QString specifier = command[0].trimmed().toUpper();
                    QString value = command[1].trimmed();
                    qDebug() << "Specifier:" << specifier << "Value:" << value;
                    for (int i = 0; i < 5; i++)
                    {
                        for (auto group : school->getYears()[i].getGroups())
                        {
                            if (specifier == "ID")
                            {
                                int searchID = value.toInt();
                                for (auto student : school->getRemovedStudents())
                                {
                                    if (student->getID() == searchID )
                                    {
                                        load_student_to_list_Widget(student);
                                    }
                                }
                            }
                            else if (specifier == "NAME")
                            {
                                for (auto student : school->getRemovedStudents())
                                {
                                    if (student->getName().contains(value, Qt::CaseInsensitive))
                                    {
                                        load_student_to_list_Widget(student);
                                    }
                                }
                            }
                            else if (specifier == "EMAIL")
                            {
                                for (auto student : school->getRemovedStudents())
                                {
                                    if (student->getEmail().contains(value, Qt::CaseInsensitive) )
                                    {
                                        load_student_to_list_Widget(student);
                                    }
                                }
                            }
                            else
                            {
                                qDebug() << "Unknown specifier:" << specifier;
                                return;
                            }
                        }
                    }

                }
                else
                {
                    qDebug() << "Normal search by name";
                    for (auto student : school->getRemovedStudents())
                    {
                        if (student->getName().contains(search, Qt::CaseInsensitive))
                            load_student_to_list_Widget(student);
                    }
                }
            }
            else
            {
                qDebug() << "Invalid search format";
            }
        }

        else
        {
            qDebug() << "Search string is empty or contains only whitespace";
            clearstudentsWiget();
            loaddeletedstudents();
        }

    }
}

void Recyclebin::clearstudentsWiget(){
    if (ui->listWidget->count() > 1)
    {
        // Remove items from the end to the second item
        for (int i = ui->listWidget->count() - 1; i > 0; --i)
        {
            delete ui->listWidget->takeItem(i);
        }
    }
}

void Recyclebin::clearteachersWiget()
{
    if (ui->listWidget->count() > 1)
    {
        // Remove items from the end to the second item
        for (int i = ui->listWidget->count() - 1; i > 0; --i)
        {
            delete ui->listWidget->takeItem(i);
        }
    }
}

void Recyclebin::on_refresh_clicked()
{

    on_Student_teacherSearchEdit_textChanged(ui->Student_teacherSearchEdit->text());
}

