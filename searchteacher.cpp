#include "searchteacher.h"
#include "ui_searchteacher.h"
#include <QMessageBox>
SearchTeacher::SearchTeacher(QWidget *parent, School* schoolptr, Course* courseptr)
    : QDialog(parent)
    , ui(new Ui::SearchTeacher)
    ,school(schoolptr)
    ,course(courseptr)
{
    ui->setupUi(this);
    load_Teachers_To_ListWidget(school->getTeachers());
}

SearchTeacher::~SearchTeacher()
{
    delete ui;
}

void SearchTeacher::clearteachersWiget()
{
    if (ui->TeacherListWidget->count() > 1)
    {
        // Remove items from the end to the second item
        for (int i = ui->TeacherListWidget->count() - 1; i > 0; --i) {
            delete ui->TeacherListWidget->takeItem(i);
        }
    }
}

void SearchTeacher::load_Teachers_To_ListWidget( QList<Teacher*> teachers)
{
    clearteachersWiget();
    for (auto teacher: teachers ){
        if ( teacher != nullptr && teacher->getName() != "Admin" && !teacher->getdeletion_State() && !course->getCourseTeacher().contains(teacher) ){
            load_teacher_to_list_Widget(teacher);
        }
    }
}
void SearchTeacher::load_teacher_to_list_Widget(Teacher* teacher){
    QListWidgetItem* item = new QListWidgetItem(teacher->getName()+" ("+QString::number(teacher->getID())+")", ui->TeacherListWidget);
    item->setSizeHint(QSize(30, 60));
    ui->TeacherListWidget->addItem(item);
}

void SearchTeacher::on_TeacherListWidget_itemClicked(QListWidgetItem *item)
{
    if( item->text() != "Teachers")
    {
        QMessageBox::StandardButton message = QMessageBox::question(this,"ADD TEACHER TO COURSE ?"," Would you like to add "+ item->text() +" to the Course Teachers ? ",QMessageBox::Yes | QMessageBox::No);
        if ( message == QMessageBox::Yes)
        {
            Teacher* teacher = school->findTeacherByName(item->text());
            course->ADDCourseTeacherToList(teacher);
            course->AddCourseTeacherToDatabase(teacher);
            emit this->AddedTeacherToCourse(teacher);
            delete item;
            item = nullptr;
        }
    }
}


void SearchTeacher::on_TeachersearchEdit_textChanged(QString search)
{
    // Check if the string is empty or contains only whitespace
    if (!search.trimmed().isEmpty())
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
                        if (teacher->getID() == searchID && !teacher->getdeletion_State() )
                            load_teacher_to_list_Widget(teacher);
                    }
                }
                else if (specifier == "NAME")
                {
                    for (auto teacher : school->getTeachers())
                    {
                        if (teacher->getName().contains(value, Qt::CaseInsensitive) && !teacher->getdeletion_State())
                            load_teacher_to_list_Widget(teacher);
                    }
                }
                else if (specifier == "EMAIL")
                {
                    for (auto teacher : school->getTeachers())
                    {
                        if (teacher->getEmail().contains(value, Qt::CaseInsensitive) && !teacher->getdeletion_State())
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
                    if (teacher->getName().contains(search, Qt::CaseInsensitive) && !teacher->getdeletion_State())
                        load_teacher_to_list_Widget(teacher);
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
        load_Teachers_To_ListWidget(school->getTeachers());
    }
}

