#include "adminer.h"
#include "ui_adminer.h"
#include "mainwindow.h"
#include "schedulesetter.h"
#include "displayer.h"
#include <algorithm> // will be used for sorting: std::sort
#include <QRegularExpression> // will be used for matching words and checking the validity of the entered QString in the Search bar
#include "groupeditor.h"
#include <QMessageBox>
#include "addgroup.h"
#include "recyclebin.h"
#include "addstudent.h"
#include "courseeditor.h"
#include "addcourse.h"
#include <QDate>
#include <QTime>

Adminer::Adminer(QWidget *parent,MainWindow* origin, QSqlDatabase *sqlbase)
    : QMainWindow(parent)
    , ui(new Ui::Adminer)
    ,login_interface(origin)
    ,Sqlbase(sqlbase)

{
    ui->setupUi(this);
    //here is a slight customization of the list widget :
     ui->TeacherListWidget->setMouseTracking(true);
     ui->TeacherListWidget->setStyleSheet("QListWidget::item:hover { background-color: purple; }");
     ui->studentsListWidget->setMouseTracking(true);
     ui->studentsListWidget->setStyleSheet("QListWidget::item:hover { background-color: purple; }");


    // //here we will create the Structure of the Whole Management System, first we will dynamically create the School object
        try{
        school = new School(sqlbase);
        qDebug()<<"there is no problem in defining the school object";

        }
        catch(...){
            qDebug()<<"the problem is within the school Database handling";
        }

    // here we will start getting the interface ready for the user

        //first we will load the teachers
        load_Teachers_To_ListWidget(school->getTeachers());
        //now we will load the Group to the Groups ListWidget
        for (auto i = 0; i < 5;  i++)
        {
            load_Groups_To_ListWidget(&school->getYears()[i]);
        }
        // and we will do the same for the courses :
        for (auto i = 0; i < 5;  i++)
        {
            load_Year_Courses_To_ListWidget(&school->getYears()[i]);
        }



    ui->ScheduleWidget->setColumnCount(6);
    Days<<"Saturday"<<"Sunday"<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday";
    ui->ScheduleWidget->setHorizontalHeaderLabels(Days);
    ui->ScheduleWidget->setColumnWidth(0,197);
    ui->ScheduleWidget->setColumnWidth(1,197);
    ui->ScheduleWidget->setColumnWidth(2,197);
    ui->ScheduleWidget->setColumnWidth(3,197);
    ui->ScheduleWidget->setColumnWidth(4,197);
    ui->ScheduleWidget->setColumnWidth(5,197);
    // this will enable the selection of only one row
    ui->ScheduleWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ScheduleWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ScheduleWidget->verticalHeader()->setSectionsClickable(true);
    ui->ScheduleWidget->verticalHeader()->setSectionsMovable(false); // Optional: Prevent moving rows

    // Connect signal for row header click to handle selection
    connect(ui->ScheduleWidget->verticalHeader(), &QHeaderView::sectionClicked,
            this, &Adminer::handleVerticalHeaderClicked);

    //the selected row selector
    selected_row = new int(-1);


     // this will set the Exam Schedule Table Widget ( just adding the horizontal headers )

    setTheExamSchedule();



    ui->GradesTable->setColumnCount(5);
    ui->GradesTable->setColumnWidth(0,226);
    ui->GradesTable->setColumnWidth(1,226);
    ui->GradesTable->setColumnWidth(2,226);
    ui->GradesTable->setColumnWidth(3,225);
    ui->GradesTable->setColumnWidth(4,225);

    ui->GradesTable->verticalHeader()->hide();
    QStringList Exams;
    Exams << "Students" << "Midterm Exam " << "Tutorial & Lab Average " << "Final Exam " << "Average ";
    ui->GradesTable->setHorizontalHeaderLabels(Exams);

    for ( int i = 0 ; i < 5; i++)
    {
        for ( auto course: school->getYears()[i].getCourses())
        {
            ui->CourseCombobox->addItem(course->getCourseName());
        }
    }

}



Adminer::~Adminer()
{
    for(int i = 0; i < 5 ; i++)
    {
        for ( auto group : school->getYears()[i].getGroups())
        {
            group->WriteScheduleItemsToDatabase();
        }
    }

    delete ui;
    delete this->selected_row;

    school->UpdateExamDetailsInDatabase();
    delete school;
}

void Adminer::handleVerticalHeaderClicked(int logicalIndex)
{
    qDebug() << "Clicked on vertical header of row:" << logicalIndex;
    *selected_row = logicalIndex;
    // Select the entire row when clicking on the header
    ui->ScheduleWidget->selectRow(logicalIndex);
}


void Adminer::on_addtimeslotbutton_clicked()
{
    if( ui->yearcombobox_Schedule->currentIndex() != 0 && ui->groupcombobox_Schedule->currentIndex() != 0)
    {
        ScheduleSetter* Adder = new ScheduleSetter(&Time_Slots,this,ui->ScheduleWidget,EntityType::Admin);
        Adder->show();
        Group* group = school->findGroupByName("Y"+ui->yearcombobox_Schedule->currentText() + "G" + ui->groupcombobox_Schedule->currentText());
        if( group)
        {
            connect(Adder,&ScheduleSetter::Added_Time_Slot,this,[this,group](QString slot)
            {
                qDebug()<<"the connected signal has done its job "<<slot;
                qDebug()<<group->getGroupNumber();
                qDebug()<<"keys in the Signal the saving function";
                for ( auto key : group->getGroupScheduleItems().keys())
                {
                    qDebug()<<key;
                }
                group->AddTimeSlotToSchedule(slot,QStringList());
            });
        }
    }
    else
    {
        QMessageBox::warning(this,"Warning !!!","Choose a Year and a Group first");
    }
}


void Adminer::on_removetimeslotbutton_clicked()
{
    // the behaviour of the Schedule QtableWidget has been altered so that the user can not select multiple rows.
    // in addition to that we must control the selected items so that if he selects the time slot or the Group/teacher/room row , both will be removed just like they are added
    // after the user selects the header we will
    if ( *selected_row > -1)
    {
        Group * group = school->findGroupByName("Y"+ui->yearcombobox_Schedule->currentText()+"G"+ui->groupcombobox_Schedule->currentText());
        if (*selected_row % 2 == 0)
        {
            group->removeTimeSlotFromSchedule(ui->ScheduleWidget->verticalHeaderItem(*selected_row)->text());
            ui->ScheduleWidget->removeRow(*selected_row);
            ui->ScheduleWidget->removeRow(*selected_row);    
        }
        else if (*selected_row % 2 == 1)
        {
            group->removeTimeSlotFromSchedule(ui->ScheduleWidget->verticalHeaderItem(*selected_row-1)->text());
            ui->ScheduleWidget->removeRow(--(*selected_row));
            ui->ScheduleWidget->removeRow(*selected_row);
        }
    }
}


void Adminer::on_LogOutbutton_clicked()
{
    if( ui->yearcombobox_Schedule->currentIndex() != 0 && ui->groupcombobox_Schedule != 0)
    {
        SelectedYear_GroupSchedule = "Y"+ui->yearcombobox_Schedule->currentText() + "G"+ ui->groupcombobox_Schedule->currentText();
        Group * group = school->findGroupByName(SelectedYear_GroupSchedule);
        QStringList DataBlock;
        QString Key;
        QString Course;
        QString Details;
        for ( int row = 0 ; row < ui->ScheduleWidget->rowCount(); row++)
        {
            if ( row % 2 == 0)
            {
                Key = ui->ScheduleWidget->verticalHeaderItem(row)->text(); // Ensure the key is based on the correct time slot row
                for( int column = 0; column < ui->ScheduleWidget->columnCount(); column ++)
                {
                    Course = ui->ScheduleWidget->item(row,column)->text();
                    Details = ui->ScheduleWidget->item(row+1,column)->text();
                    DataBlock.append( Course + ":(" + Details + ")" );
                }
                group->AddTimeSlotToSchedule(Key,DataBlock);
                DataBlock.clear();
            }
        }
    }
    login_interface->showNormal();
    delete this;
}


void Adminer::load_Teachers_To_ListWidget( QList<Teacher*> teachers)
{
    clearteachersWiget();
    for (auto teacher: teachers ){
        if ( teacher != nullptr && teacher->getName() != "Admin" && !teacher->getdeletion_State() ){
            load_teacher_to_list_Widget(teacher);
        }
    }
}
void Adminer::load_teacher_to_list_Widget(Teacher* teacher){
    QListWidgetItem* item = new QListWidgetItem(teacher->getName()+ +" ("+QString::number(teacher->getID())+")", ui->TeacherListWidget);
    item->setSizeHint(QSize(30, 60));
    ui->TeacherListWidget->addItem(item);
}



void Adminer::load_student_to_list_Widget(Student* student)
{
    QListWidgetItem* item = new QListWidgetItem(student->getName()+" ("+QString::number(student->getID())+")", ui->studentsListWidget);
    item->setSizeHint(QSize(30, 60));
    ui->studentsListWidget->addItem(item);
}



void Adminer::load_Students_To_ListWidget( Group* group) // load students of a specific group to the Students ListWidget
{
    //we should check if we want to clear the listwidget or not later
    for (auto student : group->getGroupStudents() ){
        if ( student != nullptr && !student->getdeletion_State()){
            load_student_to_list_Widget(student);
        }
    }
}



void Adminer::load_Groups_To_ListWidget(Year* year) // will load Groups to their List Widget in the Group List Widget
{
    QListWidgetItem* item = new QListWidgetItem("Y"+ QString::number(year->getYear()), ui->GroupListWidget);
    item->setSizeHint(QSize(30, 60));
    ui->GroupListWidget->addItem(item);
    item->setForeground(QBrush(QColor(Qt::color0))); // Change text color to blue
    item->setBackground(QBrush(QColor(Qt::lightGray)));
    for ( auto group : year->getGroups())
    {
        QListWidgetItem* item = new QListWidgetItem("Y"+ QString::number(year->getYear())+"G"+QString::number(group->getGroupNumber()), ui->GroupListWidget);
        item->setSizeHint(QSize(30, 60));
        ui->GroupListWidget->addItem(item);
    }
}

void Adminer::load_Year_Courses_To_ListWidget(Year* year)
{
    QListWidgetItem* item = new QListWidgetItem("Y"+ QString::number(year->getYear()), ui->CourseslistWidget);
    item->setSizeHint(QSize(30, 60));
    ui->CourseslistWidget->addItem(item);
    item->setForeground(QBrush(QColor(Qt::color0))); // Change text color to blue
    item->setBackground(QBrush(QColor(Qt::lightGray)));
    for ( auto course : year->getCourses())
    {
        load_Course_To_ListWidget(course);
    }

}

void Adminer::load_Course_To_ListWidget(Course* course)
{
    QListWidgetItem* item = new QListWidgetItem(course->getCourseName());
    item->setSizeHint(QSize(30, 60));
    ui->CourseslistWidget->addItem(item);
}


void Adminer::on_TeacherListWidget_itemClicked(QListWidgetItem *item) {

    if (!item)
    {
        qDebug() << "Item is nullptr";
        return;
    }
    if (item->text() == "Teachers"){
        qDebug() << "Item is unselectable";
        return;
    }

    Teacher* teacher = school->findTeacherByName(item->text());

    if (teacher == nullptr) {
        qDebug() << "Teacher not found";
    } else {
        qDebug() << "Teacher found: " << teacher->getName() << " " << teacher->getID();
        Displayer* teacherInfo = new Displayer(this, teacher, item,Sqlbase,school);
        connect(teacherInfo, &QDialog::finished, teacherInfo, &QDialog::deleteLater);
        qDebug() << "Showing Displayer for: " << item->text();
        teacherInfo->show();
        connect( teacherInfo,&Displayer::removeteacher,this,[this,item]()
        {
            delete item;
        });
    }


}

void Adminer::remove_Student_TeacherFromListWidget(Entity* entity)
{
    Teacher* teacher = dynamic_cast<Teacher*>(entity);
    Student* student = dynamic_cast<Student*>(entity);
    if ( teacher )
    {
        QList<QListWidgetItem*> Items = ui->TeacherListWidget->findItems(teacher->getName()+ +" ("+QString::number(teacher->getID())+")",Qt::MatchExactly);
        ui->TeacherListWidget->removeItemWidget(Items[0]);
    }
    else if ( student )
    {
        QList<QListWidgetItem*> Items = ui->studentsListWidget->findItems(student->getName()+ +" ("+QString::number(student->getID())+")",Qt::MatchExactly);
        ui->studentsListWidget->removeItemWidget(Items[0]);
    }
}



void Adminer::on_yearcomboBox_activated(int index)
{
    clearstudentsWiget();
    ui->groupCombobox->clear();
    ui->groupCombobox->addItem("Group");
    QVector<int> GroupNumber;
    if ( index != 0)//this will select the first years so here we must set the Group combobox to the Groupnumbers of the first years
    {
        for (auto group : (school->getYears()[index-1].getGroups()) )
        {
            GroupNumber.append(group->getGroupNumber());
        }
        std::sort(GroupNumber.begin(),GroupNumber.end());
        for (auto groupNumber : GroupNumber )
        {
            ui->groupCombobox->addItem(QString::number(groupNumber));
        }
    }

}


void Adminer::on_groupCombobox_activated(int index)
{
    clearstudentsWiget();
    QList <Group*> Groups = school->getYears()[ui->yearcomboBox->currentText().toInt()-1].getGroups();
    for (auto group : Groups)
    {
        if ( group->getGroupNumber() == ui->groupCombobox->currentText().toInt())
        {
            load_Students_To_ListWidget(group);
            break;
        }
    }
}
void Adminer::clearstudentsWiget(){
if (ui->studentsListWidget->count() > 1) {
    // Remove items from the end to the second item
    for (int i = ui->studentsListWidget->count() - 1; i > 0; --i) {
        delete ui->studentsListWidget->takeItem(i);
    }
}
}

void Adminer::clearteachersWiget(){
    if (ui->TeacherListWidget->count() > 1)
    {
        // Remove items from the end to the second item
        for (int i = ui->TeacherListWidget->count() - 1; i > 0; --i) {
            delete ui->TeacherListWidget->takeItem(i);
        }
    }
}

void Adminer::clearCoursesWidget()
{
    for (int i = ui->CourseslistWidget->count()-1 ; i > 0; --i)
    {
        delete ui->CourseslistWidget->takeItem(i);
    }
}
void Adminer::clearGroupsWidget()
{
    for (int i = ui->GroupListWidget->count()-1 ; i > 0; --i)
    {
        delete ui->GroupListWidget->takeItem(i);
    }
}

void Adminer::on_studentsListWidget_itemClicked(QListWidgetItem *item)
{

    if (!item) {
        qDebug() << "Item is nullptr";
        return;
    }
    if (item->text() == "Students"){
        qDebug() << "Item is unselectable";
        return;
    }


    Student* student = school->findStudentByName(item->text());// return the pointer to the student
    ui->yearcomboBox->setCurrentText(QString::number(student->getYear()));
    ui->groupCombobox->setCurrentText(QString::number(student->getgroup()));
    if (student == nullptr) {
        qDebug() << "Student not found";
    }
    else
    {
        ui->yearcomboBox->setCurrentText(QString::number(student->getYear()));
        ui->groupCombobox->setCurrentText(QString::number(student->getgroup()));
        qDebug() << "Teacher found: " << student->getName() << " " << student->getID();
        Displayer* StudentInfo = new Displayer(this, student, item,Sqlbase,school);
        connect(StudentInfo, &QDialog::finished, StudentInfo, &QDialog::deleteLater);// to ensure that it is deleted when closed
        qDebug() << "Showing Displayer for: " << item->text();
        StudentInfo->show();
        connect( StudentInfo,&Displayer::removeStudent,this,[this,item]()
        {
            delete item;
        });
    }

}


void Adminer::on_TeachersearchEdit_textChanged(QString search) // here we will use a search system that will handle the sort in two different ways, first the normal sort, in which the user will write the name of the teacher in order to make him appear, the second is using specifiers after # for example: if the user wants to get a specific individual through his ID he will use it as follows : #ID:(the wanted individual's id). like this #ID:12. the available specifiers will be : (ID, Name, Email)
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



void Adminer::on_StudentSearchEdit_textChanged(QString search) // the search system here is similar to that of the teachers, but with one additional feature which will be the Group And year specifiers that the user will use as follows #Y1G12 to make all the students of the first year Group one appear. also the following use will be valid: #Y1 as it will display all students of 1st year
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
                re.setPattern("^[Yy][0-9][0-9]?");
                if (re.match(search).hasMatch())
                {
                    re.setPattern("^[Yy][0-9][0-9]?$");
                    if (re.match(search).hasMatch())
                    {
                        int Year = search.mid(1).toInt();
                        on_yearcomboBox_activated(Year);
                        if (Year < 1 || Year > 5)
                        {
                            return;
                        }
                        for (auto group : school->getYears()[Year - 1].getGroups())
                        {
                            load_Students_To_ListWidget(group);
                        }
                        ui->yearcomboBox->setCurrentText(QString::number(Year));

                    }
                    re.setPattern("^[Yy][0-9][0-9]?[Gg][0-9][0-9]?$");
                    if (re.match(search).hasMatch())
                    {
                        QStringList command = search.split("G");
                        if (command.size() == 1)
                        {
                            command = search.split("g");
                        }
                        qDebug() << "Command:" << command;
                        qDebug() << "Command size:" << command.size();
                        if (command.size() != 2)
                        {
                            qDebug() << "Invalid command size";
                            return;
                        }
                        int Year = command[0].mid(1).toInt();
                        int groupnumber = command[1].toInt();
                        if (Year < 1 || Year > 5)
                        {
                            return;
                        }
                        for (auto group : school->getYears()[Year - 1].getGroups())
                        {
                            if (group->getGroupNumber() == groupnumber)
                                load_Students_To_ListWidget(group);
                        }
                        ui->yearcomboBox->setCurrentText(QString::number(Year));
                        ui->groupCombobox->setCurrentText(QString::number(groupnumber));
                    }
                }
                else
                {
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
                                for (auto student : group->getGroupStudents())
                                {
                                    if (student->getID() == searchID && !student->getdeletion_State())
                                    {
                                        load_student_to_list_Widget(student);
                                        ui->yearcomboBox->setCurrentText(QString::number(student->getYear()));
                                        ui->groupCombobox->setCurrentText(QString::number(student->getgroup()));
                                    }
                                }
                            }
                            else if (specifier == "NAME")
                            {
                                for (auto student : group->getGroupStudents())
                                {
                                    if (student->getName().contains(value, Qt::CaseInsensitive) && !student->getdeletion_State())
                                    {
                                        load_student_to_list_Widget(student);
                                        ui->yearcomboBox->setCurrentText(QString::number(student->getYear()));
                                        ui->groupCombobox->setCurrentText(QString::number(student->getgroup()));
                                    }
                                }
                            }
                            else if (specifier == "EMAIL")
                            {
                                for (auto student : group->getGroupStudents())
                                {
                                    if (student->getEmail().contains(value, Qt::CaseInsensitive) && !student->getdeletion_State())
                                    {
                                        load_student_to_list_Widget(student);
                                        ui->yearcomboBox->setCurrentText(QString::number(student->getYear()));
                                        ui->groupCombobox->setCurrentText(QString::number(student->getgroup()));
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
            }
            else
            {
                qDebug() << "Normal search by name";
                for (int i = 0; i < 5; i++)
                {
                    for (auto group : school->getYears()[i].getGroups())
                    {
                        for (auto student : group->getGroupStudents())
                        {
                            if (student->getName().contains(search, Qt::CaseInsensitive) && !student->getdeletion_State())
                                load_student_to_list_Widget(student);
                        }
                    }
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
    }
}

void Adminer::on_AddTeacherbutton_clicked()
{
    addStudent * addedTeacher = new addStudent(this,school,EntityType::Teacher);
    addedTeacher->show();
    connect( addedTeacher, &addStudent::addingTeacher,this,[this](QString teacher)
    {
        QListWidgetItem* item = new QListWidgetItem(teacher, ui->TeacherListWidget);
        item->setSizeHint(QSize(30, 60));
        ui->TeacherListWidget->addItem(item);
    });
}




void Adminer::on_GroupListWidget_itemClicked(QListWidgetItem *item)
{

    if (item->text().contains("G"))
    {
        GroupEditor* Editor = new GroupEditor(this, school->findGroupByName(item->text()),school);
        Editor->show();
        connect(Editor, &GroupEditor::GroupHasBeenChanged,this, [this,item](int newnumber)
        {
            QString text = item->text().split("G")[0];
            item->setText(text+"G"+QString::number(newnumber));
        });



        connect(Editor, &GroupEditor::deletegroup,this, [this,item]()
                {
                    delete item;
                });
    }
    else {
        QMessageBox::warning(this,"Warning !! "," this item is not valid");
    }

}

void Adminer::on_Addgroupbutton_clicked()
{
    AddGroup* groupAdder = new AddGroup(this, school);
    connect(groupAdder, &AddGroup::groupAdded, this, &Adminer::addAddedgroupitemtolistWidget);

    if (groupAdder->exec() == QDialog::Accepted)
    {
        // Dialog was accepted, proceed
        addAddedgroupitemtolistWidget();
    } else {
        // Dialog was rejected or closed
    }

    delete groupAdder;
}

void Adminer::addAddedgroupitemtolistWidget()
{
    Group* addedGroup = school->getaddedgroup();
    if (!addedGroup) {
        qWarning() << "No group was added.";
        return;
    }

    int year = addedGroup->getyear();
    int group = addedGroup->getGroupNumber();

    QListWidgetItem* newItem = new QListWidgetItem("Y" + QString::number(year) + "G" + QString::number(group));
    newItem->setSizeHint(QSize(30, 60));
    if (group != 1) {
        QList<QListWidgetItem*> items = ui->GroupListWidget->findItems("Y" + QString::number(year) + "G" + QString::number(group - 1), Qt::MatchExactly);
        if (!items.isEmpty())
        {
            int position = ui->GroupListWidget->row(items.first());
            ui->GroupListWidget->insertItem(position + 1, newItem);
        }
    }
    else
    {
        QList<QListWidgetItem*> items = ui->GroupListWidget->findItems("Y" + QString::number(year), Qt::MatchExactly);
        if (!items.isEmpty()) {
            int position = ui->GroupListWidget->row(items.first());
            ui->GroupListWidget->insertItem(position + 1, newItem);
    }
    else
    {
        ui->GroupListWidget->addItem(newItem);
    }
    }
    school->setaddedgroup(nullptr);
}


void Adminer::addAddedCourseitemtolistWidget()
{
    Course* addedCourse = school->getAddedCourse();
    if (!addedCourse) {
        qWarning() << "No course was added.";
        return;
    }

    QListWidgetItem* newItem = new QListWidgetItem(addedCourse->getCourseName());
    newItem->setSizeHint(QSize(30, 60));


    QString searchText = "Y"+ QString::number(addedCourse->getyear());
    QList<QListWidgetItem *> items = ui->CourseslistWidget->findItems(searchText, Qt::MatchExactly);

    QListWidgetItem *item = items.first();
    int row = ui->CourseslistWidget->row(item);


    ui->CourseslistWidget->insertItem(row+1 ,newItem);
    school->setAddedCourse(nullptr);
}


void Adminer::on_StudentRecycleBinbutton_clicked()
{
    Recyclebin * bin = new Recyclebin(this,school,EntityType::Student);
    bin->show();
    connect(bin,&Recyclebin::studentrecovery,this,[this]()
    {
        this->on_refreshstudents_clicked();
    }
            );
}


void Adminer::on_TeacherRecycleBinbutton_clicked()
{
    Recyclebin * bin = new Recyclebin(this,school,EntityType::Teacher);
    bin->show();
    connect(bin, &Recyclebin::teacherrecovery,this,[this](QString teacher)
    {
        QListWidgetItem* item = new QListWidgetItem(teacher, ui->TeacherListWidget);
        item->setSizeHint(QSize(30, 60));
        ui->TeacherListWidget->addItem(item);
    });
}


void Adminer::on_refreshTeachers_clicked()
{
    on_TeachersearchEdit_textChanged(ui->TeachersearchEdit->text());
}


void Adminer::on_refreshstudents_clicked()
{
    on_StudentSearchEdit_textChanged(ui->StudentSearchEdit->text());
    if( ui->yearcomboBox->currentIndex() != 0 && ui->groupCombobox->currentIndex()!=0)
    {
        this->on_StudentSearchEdit_textChanged("#Y"+ui->yearcomboBox->currentText()+"G"+ui->groupCombobox->currentText());
    }
    else
    {
        on_StudentSearchEdit_textChanged(ui->StudentSearchEdit->text());
    }
}


void Adminer::on_AddStudentbutton_clicked()
{
    addStudent * addedStudent = new addStudent(this,school,EntityType::Student);
    addedStudent->show();
    connect( addedStudent, &addStudent::addingStudent,this,[this](QString student)
    {
        this->on_refreshstudents_clicked();
    });
}


void Adminer::on_refreshGroups_clicked()
{
    clearGroupsWidget();
    for (auto i = 0; i < 5;  i++)
    {
        load_Groups_To_ListWidget(&school->getYears()[i]);
    }

}


void Adminer::on_CourseslistWidget_itemClicked(QListWidgetItem *item)
{
    static QRegularExpression re("^Y[1-5]$");
    if(!re.match(item->text()).hasMatch())
    {
        Course*course = school->findCoursebyName(item->text());
        CourseEditor* Editor = new CourseEditor(this, school, course);
        Editor->show();
        connect(Editor, &CourseEditor::CourseNameHasBeenChanged,this, [this,item](QString name)
        {
            item->setText(name);
        });
        connect(Editor, &CourseEditor::courseDeleted, this, [this,item]
        {
            int index = this->ui->CourseCombobox->findText(item->text());

            // If the item exists, remove it
            if (index != -1) {
                this->ui->CourseCombobox->removeItem(index);
            }
            delete item ;
        });

    }
    else
    {
        QMessageBox::warning(this,"Warning !! "," this item is not valid");
    }
}


void Adminer::on_Addcoursebutton_clicked()
{
    AddCourse* CourseAdder = new AddCourse(this, school);
    connect(CourseAdder, &AddCourse::CourseAdded, this, &Adminer::addAddedCourseitemtolistWidget);

    if (CourseAdder->exec() == QDialog::Accepted) {
        // Dialog was accepted, proceed
        addAddedCourseitemtolistWidget();
    } else {
        // Dialog was rejected or closed
    }

    delete CourseAdder;
}

// this function can be far more readable using Enums, and also the ifs could have been replaced by switch cases
void Adminer::on_CoursesearchEdit_textChanged( QString search) // in this search function commands just like usual will start with # and a specifier will follow them : id, name, ss:Le (for courses that have lectures), ss:La(for courses that have labs), ss:T(for courses that have tutorials),Y(for year), S(for semester). the general form is as follows: #id:1, #ss:Le, #ss:La, #ss:T, #ss:Le.La.T, or even  #Y1:S1
{
    if (!search.trimmed().isEmpty())
    {
        qDebug() << "Search string:" << search;
        QRegularExpression re("^#?[a-zA-Z\\s:1-9 ]+$");// we will be using regular expressions to make comparing and checking if the QString is valid easier
        if (re.match(search).hasMatch())
        {
            clearCoursesWidget();
            if (search.startsWith("#"))
            {
                qDebug() << "The string starts with a #.";
                qDebug()<<search.remove("#").trimmed();
                qDebug()<<"search after being trimmed and all : "<<search;                    // Remove and trim leading #
                QStringList command = search.split(":");
                qDebug() << "Command:" << command;
                if (command.size() >= 2 )
                {
                    QString specifier = command[0].trimmed().toUpper();
                    QString value = command[1].trimmed();
                    qDebug() << "Specifier:" << specifier << "Value:" << value;

                    if (specifier == "ID")
                    {
                        int searchID = value.toInt();
                        for ( int i = 0; i < 5; i++)
                        {
                            for (auto course : school->getYears()[i].getCourses())
                            {
                                if ( course->getID() == searchID )
                                    load_Course_To_ListWidget(course);
                            }
                        }
                    }
                    else if (specifier == "NAME")
                    {
                        for ( int i = 0; i < 5; i++)
                        {
                            for (auto course : school->getYears()[i].getCourses())
                            {
                                if (course->getCourseName().contains(value, Qt::CaseInsensitive) )
                                    load_Course_To_ListWidget(course);
                            }
                        }
                    }
                    else if (specifier == "SS")
                    {
                        QStringList sessions = value.split(".");
                        if ( sessions.size() == 1)
                        {
                            if (value == "Le")
                            {
                                for ( int i = 0; i < 5; i++)
                                {
                                    for (auto course : school->getYears()[i].getCourses())
                                    {
                                        if (course->hasLecture() )
                                            load_Course_To_ListWidget(course);
                                    }
                                }
                            }
                            else if (value == "La")
                            {
                                for ( int i = 0; i < 5; i++)
                                {
                                    for (auto course : school->getYears()[i].getCourses())
                                    {
                                        if (course->hasLab() )
                                            load_Course_To_ListWidget(course);
                                    }
                                }
                            }
                            else if (value == "T")
                            {
                                for ( int i = 0; i < 5; i++)
                                {
                                    for (auto course : school->getYears()[i].getCourses())
                                    {
                                        if (course->hasTutorial() )
                                            load_Course_To_ListWidget(course);
                                    }
                                }
                            }
                        }
                        else if (sessions.size() == 2)
                        {
                            if ((sessions[0] == "Le" && sessions[1] == "La") || (sessions[1] == "Le" && sessions[0] == "La"))
                            {
                                for ( int i = 0; i < 5; i++)
                                {
                                    for (auto course : school->getYears()[i].getCourses())
                                    {
                                        if (course->hasLecture() && course->hasLab() )
                                            load_Course_To_ListWidget(course);
                                    }
                                }
                            }
                            if ((sessions[0] == "Le" && sessions[1] == "T") || (sessions[1] == "Le" && sessions[0] == "T"))
                            {
                                for ( int i = 0; i < 5; i++)
                                {
                                    for (auto course : school->getYears()[i].getCourses())
                                    {
                                        if (course->hasLecture() && course->hasTutorial() )
                                            load_Course_To_ListWidget(course);
                                    }
                                }
                            }
                            if ((sessions[0] == "T" && sessions[1] == "La") || (sessions[1] == "T" && sessions[0] == "La"))
                            {
                                for ( int i = 0; i < 5; i++)
                                {
                                    for (auto course : school->getYears()[i].getCourses())
                                    {
                                        if (course->hasTutorial() && course->hasLab() )
                                            load_Course_To_ListWidget(course);
                                    }
                                }
                            }
                        }
                        else if ( sessions.size() == 3)
                        {
                            if ((sessions[0] == "T" && sessions[1] == "La" && sessions[2] == "Le") || (sessions[0] == "T" && sessions[2] == "La" && sessions[1] == "Le") || (sessions[1] == "T" && sessions[0] == "La" && sessions[2] == "Le") || (sessions[1] == "T" && sessions[2] == "La" && sessions[0] == "Le") || (sessions[2] == "T" && sessions[1] == "La" && sessions[0] == "Le") || (sessions[2] == "T" && sessions[0] == "La" && sessions[1] == "Le"))
                            {
                                for ( int i = 0; i < 5; i++)
                                {
                                    for (auto course : school->getYears()[i].getCourses())
                                    {
                                        if (course->hasTutorial() && course->hasLab() )
                                            load_Course_To_ListWidget(course);
                                    }
                                }
                            }
                        }
                    }
                    else if ((specifier.startsWith("Y") ||specifier.startsWith("y"))  && (value.startsWith("S") || value.startsWith("s")) )
                    {
                        specifier=specifier.toUpper();
                        value = value.toUpper();
                        int year = specifier.removeFirst().toInt();
                        if (year < 5 && year > 0)
                        {
                            int semester = value.removeFirst().toInt();
                            for (auto course : school->getYears()[year-1].getCourses())
                            {
                                if (course->getsemester() == semester )
                                    load_Course_To_ListWidget(course);
                            }
                        }
                    }

                    else
                    {
                        qDebug() << "Unknown specifier:" << specifier;
                    }
                }
                else
                {
                    qDebug() << "command not accepted " ;
                }
            }
            else
            {
                qDebug() << "Normal search by name";
                for ( int i = 0; i < 5; i++)
                {
                    for (auto course : school->getYears()[i].getCourses())
                    {
                        if (course->getCourseName().contains(search, Qt::CaseInsensitive) )
                            load_Course_To_ListWidget(course);
                    }
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
        clearCoursesWidget();
        for (auto i = 0; i < 5;  i++)
        {
            load_Year_Courses_To_ListWidget(&school->getYears()[i]);
        }
    }
}


void Adminer::on_refreshCoursesButton_clicked()
{
    on_CoursesearchEdit_textChanged(ui->CoursesearchEdit->text());
}

void Adminer::setTheExamSchedule()
{
    QTableWidget* ExamTable = ui->ExamDateTableWidget;
    ExamTable->verticalHeader()->hide();
    ExamTable->setColumnCount(7);
    QStringList HorizontalHeaders;
    HorizontalHeaders<<"Courses"<<"Midterm Exam Date"<<"Start Time"<<"End Time"<<"Final Exam Date"<<"Start Time"<<"End Time";
    ExamTable->setHorizontalHeaderLabels(HorizontalHeaders);
    ExamTable->setColumnWidth(0, 278); // Adjust width for "Courses"
    ExamTable->setColumnWidth(1, 150); // Adjust width for "Midterm Exam Date"
    ExamTable->setColumnWidth(2, 110);  // Adjust width for "Start Time"
    ExamTable->setColumnWidth(3, 110);  // Adjust width for "End Time"
    ExamTable->setColumnWidth(4, 150); // Adjust width for "Final Exam Date"
    ExamTable->setColumnWidth(5, 110);  // Adjust width for "Start Time"
    ExamTable->setColumnWidth(6, 110);
    on_ExamYearComboBox_activated(0);
}

void Adminer::on_ExamYearComboBox_activated(int index)
{


    QTableWidget* ExamTable = ui->ExamDateTableWidget;

    // Clear the existing table data to avoid outdated information
    ExamTable->clearContents();
    ExamTable->setRowCount(0);

    QList<Course*> Courses = school->getYears()[index].getCourses();
    ExamTable->setRowCount(Courses.size());

    for (int row = 0; row < Courses.size(); ++row)
    {
        Course* course = Courses.at(row);
        Exam* midtermExam = course->getMidterExam();
        Exam* finalExam = course->getFinalExam();

        // Create items for each column in the current row
        QTableWidgetItem* courseNameItem = new QTableWidgetItem(course->getCourseName());
        ExamTable->setItem(row, 0, courseNameItem);
        courseNameItem->setFlags(courseNameItem->flags() & ~Qt::ItemIsEditable); // Set the Course Name column as read-only

        if (midtermExam)
        {
            QTableWidgetItem* midtermDateItem = new QTableWidgetItem(midtermExam->getExamDay().toString("dd/MM/yyyy"));
            QTableWidgetItem* midtermStartTimeItem = new QTableWidgetItem(midtermExam->getStartTime().toString("HH:mm:ss"));
            QTableWidgetItem* midtermEndTimeItem = new QTableWidgetItem(midtermExam->getEndTime().toString("HH:mm:ss"));
            ExamTable->setItem(row, 1, midtermDateItem);
            ExamTable->setItem(row, 2, midtermStartTimeItem);
            ExamTable->setItem(row, 3, midtermEndTimeItem);
        }

        if (finalExam)
        {
            QTableWidgetItem* finalDateItem = new QTableWidgetItem(finalExam->getExamDay().toString("dd/MM/yyyy"));
            QTableWidgetItem* finalStartTimeItem = new QTableWidgetItem(finalExam->getStartTime().toString("HH:mm:ss"));
            QTableWidgetItem* finalEndTimeItem = new QTableWidgetItem(finalExam->getEndTime().toString("HH:mm:ss"));
            ExamTable->setItem(row, 4, finalDateItem);
            ExamTable->setItem(row, 5, finalStartTimeItem);
            ExamTable->setItem(row, 6, finalEndTimeItem);
        }
    }
}


void Adminer::on_ExamDateTableWidget_cellChanged(int row, int column)
{
    QString text = ui->ExamDateTableWidget->item(row,column)->text();
    if( text != this->SelectedTextforExam)
    {
        if( column == 1 || column == 4)
        {
            QRegularExpression re("^[0-9]{2}/[0-9]{2}/2[0-9]{3}$");
            if ( re.match(text).hasMatch())
            {
                QStringList Date = text.split("/");
                int year = Date[2].toInt();
                int month = Date[1].toInt();
                int day = Date[0].toInt();
                QDate date(year,month,day);
                if( date.isValid())
                {
                    QString courseName = ui->ExamDateTableWidget->item(row,0)->text();
                    Course* course = school->findCoursebyName(courseName);
                    if (course)
                    {
                        if(column == 1)
                        {
                            course->getMidterExam()->setExamDay(date);
                        }
                        else if ( column == 4)
                        {
                            course->getFinalExam()->setExamDay(date);
                        }
                    }
                }
                else
                {
                    QMessageBox::warning(this,"INVALID !!"," This Date Is Not Valid !!!");
                    ui->ExamDateTableWidget->item(row,column)->setText(this->SelectedTextforExam);
                }
            }
            else
            {
                QMessageBox::warning(this,"INVALID !!"," This Date Is Not Valid !!!");
                ui->ExamDateTableWidget->item(row,column)->setText(this->SelectedTextforExam);
            }
        }
        else if ( column == 2 || column == 3 || column == 5 || column == 6)
        {
            QRegularExpression re("[0-9][0-9]:[0-9][0-9]:00");
            if(re.match(text).hasMatch())
            {
                QStringList Time = text.split(":");
                int hour = Time[0].toInt();
                int minutes = Time[1].toInt();
                QTime time(hour,minutes);
                if( time.isValid())
                {
                    QString courseName = ui->ExamDateTableWidget->item(row,0)->text();
                    Course* course = school->findCoursebyName(courseName);
                    if(course)
                    {
                        switch(column)
                        {
                        case 2:
                        {
                            course->getMidterExam()->setStartTime(time);
                        }
                            break;
                        case 3:
                        {
                            course->getMidterExam()->setEndTime(time);
                        }
                            break;
                        case 5:
                        {
                            course->getFinalExam()->setStartTime(time);
                        }
                        break;
                        case 6:
                        {
                            course->getFinalExam()->setEndTime(time);
                        }
                        break;
                        }
                    }
                }
                else
                {
                    QMessageBox::warning(this,"INVALID !!"," This Time Is Not Valid !!!");
                    ui->ExamDateTableWidget->item(row,column)->setText(this->SelectedTextforExam);
                }
            }
            else
            {
                QMessageBox::warning(this,"INVALID !!"," This Time Is Not Valid !!!");
                ui->ExamDateTableWidget->item(row,column)->setText(this->SelectedTextforExam);
            }
        }
    }
}


void Adminer::on_ExamDateTableWidget_cellClicked(int row, int column)
{
    this->SelectedTextforExam = ui->ExamDateTableWidget->item(row,column)->text();
}


void Adminer::on_refreshExamButton_clicked()
{
    on_ExamYearComboBox_activated(ui->ExamYearComboBox->currentIndex());
}


void Adminer::on_yearcombobox_Schedule_activated(int index)
{
    if( !this->SelectedYear_GroupSchedule.isEmpty())
    {
        Group * group = school->findGroupByName(SelectedYear_GroupSchedule);
        QStringList DataBlock;
        QString Key;
        QString Course;
        QString Details;
        for ( int row = 0 ; row < ui->ScheduleWidget->rowCount(); row++)
        {
            if ( row % 2 == 0)
            {
                Key = ui->ScheduleWidget->verticalHeaderItem(row)->text(); // Ensure the key is based on the correct time slot row
                for( int column = 0; column < ui->ScheduleWidget->columnCount(); column ++)
                {
                    Course = ui->ScheduleWidget->item(row,column)->text();
                    Details = ui->ScheduleWidget->item(row+1,column)->text();
                    DataBlock.append( Course + ":(" + Details + ")" );
                }
                group->AddTimeSlotToSchedule(Key,DataBlock);
                DataBlock.clear();
            }
        }
        group->WriteScheduleItemsToDatabase();
    }
    ui->ScheduleWidget->setRowCount(0);
    ui->groupcombobox_Schedule->clear();
    ui->groupcombobox_Schedule->addItem("Group");
    QVector<int> GroupNumber;
    if ( index != 0)//this will select the first years so here we must set the Group combobox to the Groupnumbers of the first years
    {
        for (auto group : (school->getYears()[index-1].getGroups()) )
        {
            GroupNumber.append(group->getGroupNumber());
        }
        std::sort(GroupNumber.begin(),GroupNumber.end());
        for (auto groupNumber : GroupNumber )
        {
            ui->groupcombobox_Schedule->addItem(QString::number(groupNumber));
        }
    }
}


void Adminer::setGroupsSchedule(Group * group)
{
    qDebug() << "Setting the Schedule For" << group->getGroupNumber();

    int row = 0;
    QMap<QString, QStringList> Items = group->getGroupScheduleItems();
    ui->ScheduleWidget->setRowCount(Items.count() * 2);

    QStringList Time_Slots;

    for (auto slot : Items.keys())
    {
        Time_Slots << slot << "Teacher / room";
        QStringList courses = Items[slot];

        for (int column = 0; column < 6; column++)
        {
            QString course;
            QString Details;

            if (column < courses.size() && courses[column].contains(":"))
            {
                QStringList parts = courses[column].split(":");
                course = parts[0];
                if (parts.size() > 1)
                    Details = parts[1];
            }

            QTableWidgetItem *item1 = new QTableWidgetItem(course);
            QTableWidgetItem *item2 = new QTableWidgetItem(Details.removeFirst().removeLast());
            ui->ScheduleWidget->setItem(row, column, item1);
            ui->ScheduleWidget->setItem(row + 1, column, item2);
        }
        row += 2;
    }
    qDebug()<<"hello";

    ui->ScheduleWidget->setVerticalHeaderLabels(Time_Slots);
    this->Time_Slots = Time_Slots;

    QString verticalHeaderStyleRGB = "QHeaderView::section { background-color: rgb(116,9,130); }"; // Light blue using RGB
    ui->ScheduleWidget->verticalHeader()->setStyleSheet(verticalHeaderStyleRGB);

    // Set the height and width of the cells in the schedule table
    for (int i = 0; i < ui->ScheduleWidget->rowCount(); i++)
        ui->ScheduleWidget->setRowHeight(i, 60);
    for (int i = 0; i < ui->ScheduleWidget->columnCount(); i++)
        ui->ScheduleWidget->setColumnWidth(i, 175);
}

void Adminer::on_groupcombobox_Schedule_activated(int index)
{
    if (this->SelectedYear_GroupSchedule.isEmpty())
    {
        SelectedYear_GroupSchedule = "Y"+ui->yearcombobox_Schedule->currentText() + "G"+ ui->groupcombobox_Schedule->currentText();
    }
    else
    {
        Group * group = school->findGroupByName(SelectedYear_GroupSchedule);
        if (group)
        {
            QStringList DataBlock;
            QString Key;
            QString Course;
            QString Details;
            for ( int row = 0 ; row < ui->ScheduleWidget->rowCount(); row++)
            {
                if ( row % 2 == 0)
                {
                    Key = ui->ScheduleWidget->verticalHeaderItem(row)->text(); // Ensure the key is based on the correct time slot row
                    for( int column = 0; column < ui->ScheduleWidget->columnCount(); column ++)
                    {
                        Course = ui->ScheduleWidget->item(row,column)->text();
                        Details = ui->ScheduleWidget->item(row+1,column)->text();
                        DataBlock.append( Course + ":(" + Details + ")" );
                    }
                    group->AddTimeSlotToSchedule(Key,DataBlock);
                    DataBlock.clear();
                }
            }
            for(int i = 0; i < 5 ; i++)
            {
                for ( auto group : school->getYears()[i].getGroups())
                {
                    group->WriteScheduleItemsToDatabase();
                }
            }
        }
    }
    ui->ScheduleWidget->setRowCount(0);
    ui->ScheduleWidget->setColumnCount(6);
    Days<<"Saturday"<<"Sunday"<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday";
    SelectedYear_GroupSchedule = "Y"+ui->yearcombobox_Schedule->currentText() + "G"+ ui->groupcombobox_Schedule->currentText();
    qDebug() << ui->yearcombobox_Schedule->currentText().toInt() << " " << ui->groupcombobox_Schedule->currentText().toInt();
    QList<Group*> Groups = school->getYears()[ui->yearcombobox_Schedule->currentText().toInt() - 1].getGroups();
    for (auto group : Groups)
    {
        if (group->getGroupNumber() == ui->groupcombobox_Schedule->currentText().toInt())
        {
            this->setGroupsSchedule(group);
            break;
        }
    }
    ui->ScheduleWidget->setHorizontalHeaderLabels(Days);
    ui->ScheduleWidget->setColumnWidth(0,197);
    ui->ScheduleWidget->setColumnWidth(1,197);
    ui->ScheduleWidget->setColumnWidth(2,197);
    ui->ScheduleWidget->setColumnWidth(3,197);
    ui->ScheduleWidget->setColumnWidth(4,197);
    ui->ScheduleWidget->setColumnWidth(5,197);
}



void Adminer::on_CourseCombobox_activated(int index)
{
    ui->GroupComboBox->clear();
    ui->GroupComboBox->addItem("Group");
    if( index != 0)
    {
        Course* course = school->findCoursebyName(ui->CourseCombobox->currentText());
        for( auto group : school->getYears()[course->getyear()-1].getGroups())
        {
            ui->GroupComboBox->addItem(QString::number(group->getGroupNumber()));
        }
        for( int row = 0; row < ui->GradesTable->rowCount();row++)
        {
            for (int column = 0; column < ui->GradesTable->columnCount(); column ++)
            {
                QTableWidgetItem* Item = ui->GradesTable->item(row,column);
                delete Item;
            }
        }
        ui->GradesTable->setRowCount(0);
    }
}


void Adminer::on_GroupComboBox_activated(int index)
{
    if( index != 0)
    {
        Course* course = school->findCoursebyName(ui->CourseCombobox->currentText());
        Group* group = school->findGroupByName("Y"+QString::number(course->getyear())+"G"+ui->GroupComboBox->currentText());
        course->loadGroupGradesFromDatabase(group);
        this->setStudentsGradesTableWidget(group);
    }
}




void Adminer::loadStudentExamToTableWidget(int row,Student * student)
{
    QTableWidgetItem *Item = ui-> GradesTable->item(row, 0);
    if (Item) {
        Item->setText(student->getName() + " (" + QString::number(student->getID()) + ")");
        QList<float> Grades = student->getGrades();

        // Ensure the tutorial attendance list has the expected number of sessions

        for (int i = 0; i < 4; i++) {
            Item = ui-> GradesTable->item(row, i + 1);  // +1 to align with column headers
            if (Item)
            {
                Item->setText(QString::number(Grades[i]));
            }
        }
    }
}
void Adminer::setStudentsGradesTableWidget(Group* group)
{
    qDebug()<<" Here .... , , , , , "<<group->getNumberOfStudents();
    ui-> GradesTable->setRowCount(group->getNumberOfStudents());
    for (int row = 0; row < ui-> GradesTable->rowCount(); row++) {
        for (int col = 0; col < ui-> GradesTable->columnCount(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setSizeHint(QSize(60, 120));
            ui-> GradesTable->setItem(row, col, item);
        }
    }
    int row = 0;
    for (auto student : group->getGroupStudents())
    {
        loadStudentExamToTableWidget(row,student);
        row++;
    }
}


