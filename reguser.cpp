#include "reguser.h"
#include "ui_reguser.h"
#include"mainwindow.h"
#include"schedulesetter.h"
#include<QBrush>
#include "displayer.h"
RegUser::RegUser(QWidget *parent, MainWindow* origin,QString Teacher, QSqlDatabase *sqlbase )
    : QMainWindow(parent)
    , ui(new Ui::RegUser)
    ,login_interface(origin)
    ,Sqlbase(sqlbase)
    ,id(SessionType::NoSession)
    ,course(nullptr)
    ,group(nullptr)
{
    ui->setupUi(this);

    try{
        school = new School(sqlbase);
        qDebug()<<"there is no problem in defining the school object";

    }
    catch(...){
        qDebug()<<"the problem is within the school Dtabase handling";
    }
    // here we will initialize the teacher pointer :
    teacher = school->findTeacherByName(Teacher);
    if( !teacher )
    {
        qDebug()<<"teacher not found : "<<Teacher;
        return;
    }
    // after that we will fill the Courses Combobox courses Assigned to this Teacher
    for( int i = 0; i < 5; i++)
    {
        for( auto course : school->getYears()[i].getCourses())
        {
            if( course->getCourseTeacher().contains(teacher))
            {
                ui->CoursesCombobox->addItem(course->getCourseName());
            }
        }
    }
    school->loadTeacherScheduleItems(teacher);

    //here we will set the headers for the schedule table
    Days<<"Saturday"<<"Sunday"<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday";
    ui->ScheduleWidget->setHorizontalHeaderLabels(Days);

    this->setTeacherScheuleWidget();


    QString verticalHeaderStyleRGB = "QHeaderView::section { background-color: rgb(116,9,130); }"; // Light blue using RGB
    ui->ScheduleWidget->verticalHeader()->setStyleSheet(verticalHeaderStyleRGB);
    // we will set the height and width of the cells in the schedule table using code because it is simpler
    for (int i = 0; i < ui->ScheduleWidget->rowCount();i++)
        ui->ScheduleWidget->setRowHeight(i,60);
    for (int i = 0; i < ui->ScheduleWidget->columnCount();i++)
        ui->ScheduleWidget->setColumnWidth(i,178);

    // this will enable the selection of only one row
    ui->ScheduleWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ScheduleWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ScheduleWidget->verticalHeader()->setSectionsClickable(true);
    ui->ScheduleWidget->verticalHeader()->setSectionsMovable(false); // Optional: Prevent moving rows

    // Connect signal for row header click to handle selection
    connect(ui->ScheduleWidget->verticalHeader(), &QHeaderView::sectionClicked,
            this, &RegUser::handleVerticalHeaderClicked);

    //the selected row selector
    selected_row = new int(-1);

    //here we will set the Exam Table Widget :
    ui->ExamTableWidget->setColumnCount(5);
    ui->ExamTableWidget->setColumnWidth(0,233);
    ui->ExamTableWidget->setColumnWidth(1,233);
    ui->ExamTableWidget->setColumnWidth(2,233);
    ui->ExamTableWidget->setColumnWidth(3,233);
    ui->ExamTableWidget->setColumnWidth(4,233);



    ui->ExamTableWidget->verticalHeader()->hide();
    QStringList Exams;
    Exams << "Students" << "Midterm Exam " << "Tutorial & Lab Average " << "Final Exam " << "Average ";
    ui->ExamTableWidget->setHorizontalHeaderLabels(Exams);
}

//a helper function that will select the row when the user selects the Vertical header to facilitate for him the process of removing a
void RegUser::handleVerticalHeaderClicked(int logicalIndex)
{
    qDebug() << "Clicked on vertical header of row:" << logicalIndex;
    *selected_row= logicalIndex;
    // Select the entire row when clicking on the header
    ui->ScheduleWidget->selectRow(logicalIndex);
}

RegUser::~RegUser()
{
    delete school;
    delete ui;
    delete selected_row;
}


void RegUser::on_LogOutbutton_clicked()
{
    if (group && course)
    {
        this->UpdateGradesStructure(group);
    }
    this->saveTeacherScheuleWidget();
    if( ui->RoleCombobox->currentText() == "Lecture" )
        id = SessionType::Lecture;
    else if( ui->RoleCombobox->currentText() == "Lab" )
        id = SessionType::Lab;
    else if( ui->RoleCombobox->currentText() == "Tutorial" )
        id = SessionType::Tutorial;
    this->updateAttendanceStructure();

    login_interface->showNormal();
    delete this;
}


void RegUser::on_addtimeslotbutton_clicked()
{
    ScheduleSetter* Adder = new ScheduleSetter(Time_Slots,this,ui->ScheduleWidget,EntityType::Teacher);
    Adder->show();
    Teacher* Teacher = teacher;
    connect(Adder,&ScheduleSetter::Added_Time_Slot,this,[this,Teacher](QString slot)
            {
                qDebug()<<"the connected signal has done its job "<<slot;
                qDebug()<<Teacher->getName();
                qDebug()<<"keys in the Signal the saving function";
                for ( auto key : group->getGroupScheduleItems().keys())
                {
                    qDebug()<<key;
                }
                teacher->AddTimeSlotToSchedule(slot,QStringList());
            });
}


void RegUser::on_removetimeslotbutton_clicked()
{
    // the behaviour of the Schedule QtableWidget has been altered so that the user can not select multiple rows.
    // in addition to that we must control the selected items so that if he selects the time slot or the Group/teacher/room row , both will be removed just like they are added
    // after the user selects the header we will
    if ( *selected_row > -1)
    {
        if (*selected_row % 2 == 0)
        {
            teacher->removeTimeSlotFromSchedule(ui->ScheduleWidget->verticalHeaderItem(*selected_row)->text());
            ui->ScheduleWidget->removeRow(*selected_row);
            ui->ScheduleWidget->removeRow(*selected_row);
        }
        else if (*selected_row % 2 == 1)
        {
            teacher->removeTimeSlotFromSchedule(ui->ScheduleWidget->verticalHeaderItem(*selected_row-1)->text());
            ui->ScheduleWidget->removeRow(--(*selected_row));
            ui->ScheduleWidget->removeRow(*selected_row);
        }
    }
}



void RegUser::on_CoursesCombobox_activated(int index)
{
    if (group && course)
    {
        this->UpdateGradesStructure(group);
    }
    if ( ui->CoursesCombobox->currentIndex() != 0)
    {
        ui->GroupCombobox->clear();
        ui->GroupCombobox->addItem("Group");
        this->LabGroups.clear();
        this->LectureGroups.clear();
        this->TutorialGroups.clear();
        this->Groups.clear();
        ui->RoleCombobox->clear();
        ui->RoleCombobox->addItem("Assigned Role");
        course = school->findCoursebyName(ui->CoursesCombobox->currentText());
        for ( auto Group_Number : course->getLectureTeachers().keys())
        {
            if (course->getLectureTeachers().value(Group_Number) == teacher)
            {
                this->LectureGroups.append(Group_Number);
                this->Groups.insert(Group_Number);
            }
        }
        for ( auto Group_Number : course->getTutorialTeachers().keys())
        {
            if (course->getTutorialTeachers().value(Group_Number) == teacher)
            {
                this->TutorialGroups.append(Group_Number);
                this->Groups.insert(Group_Number);
            }
        }
        for ( auto Group_Number : course->getLabTeachers().keys())
        {
            if (course->getLabTeachers().value(Group_Number) == teacher)
            {
                this->LabGroups.append(Group_Number);
                this->Groups.insert(Group_Number);
            }
        }
        for ( auto group : this->Groups)
        {
            ui->GroupCombobox->addItem(QString::number(group));
        }
        for( int row = 0; row < ui->ExamTableWidget->rowCount();row++)
        {
            for (int column = 0; column < ui->ExamTableWidget->columnCount(); column ++)
            {
                QTableWidgetItem* Item = ui->ExamTableWidget->item(row,column);
                delete Item;
            }
        }
        ui->ExamTableWidget->setRowCount(0);
    }
    else
    {
        course = nullptr;
    }


}


void RegUser::on_GroupCombobox_activated(int index)
{

    this->clearstudentsWiget();
    ui->RoleCombobox->clear();
    ui->RoleCombobox->addItem("Assigned Role");
    if (group && course)
    {
        this->UpdateGradesStructure(group);
    }
    group = school->findGroupByName("Y" + QString::number(course->getyear()) + "G" + ui->GroupCombobox->currentText());
    if( group && course)
    {
        this->load_Students_To_ListWidget(group);
        if( this->LectureGroups.contains(group->getGroupNumber()))
        {
            ui->RoleCombobox->addItem("Lecture");
        }
        if( this->LabGroups.contains(group->getGroupNumber()))
        {
            ui->RoleCombobox->addItem("Lab");
        }
        if( this->TutorialGroups.contains(group->getGroupNumber()))
        {
            ui->RoleCombobox->addItem("Tutorial");
        }
        school->loadCourseGroupAttendance(course,group);
        course->loadGroupGradesFromDatabase(group);
        this->setStudentsExamTableWidgetWidget();
    }
}

void RegUser::load_Students_To_ListWidget( Group* group) // load students of a specific group to the Students ListWidget
{
    //we should check if we want to clear the listwidget or not later
    for (auto student : group->getGroupStudents() ){
        if ( student != nullptr && !student->getdeletion_State()){
            load_student_to_list_Widget(student);
        }
    }
}
void RegUser::load_student_to_list_Widget(Student* student)
{
    QListWidgetItem* item = new QListWidgetItem(student->getName()+" ("+QString::number(student->getID())+")", ui->StudentListWidget);
    item->setSizeHint(QSize(30, 60));
    // Set the background color using RGB values
    QColor backgroundColor(227,193,255); // Red color (you can use any RGB values)
    QBrush backgroundBrush(backgroundColor);
    item->setBackground(backgroundBrush);

    ui->StudentListWidget->addItem(item);
}
void RegUser::clearstudentsWiget()
{
    if (ui->StudentListWidget->count() > 1) {
        // Remove items from the end to the second item
        for (int i = ui->StudentListWidget->count() - 1; i > 0; --i)
        {
            delete ui->StudentListWidget->takeItem(i);
        }
    }
}

void RegUser::on_StudentListWidget_itemClicked(QListWidgetItem *item)
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
    if (student == nullptr) {
        qDebug() << "Student not found";
    } else {
        qDebug() << "studentfound: " << student->getName() << " " << student->getID();
        Displayer* StudentInfo = new Displayer(this, student, item,Sqlbase,school,EntityType::Teacher);
        connect(StudentInfo, &QDialog::finished, StudentInfo, &QDialog::deleteLater);// to ensure that it is deleted when closed
        qDebug() << "Showing Displayer for: " << item->text();
        StudentInfo->show();
    }
}




void RegUser::setAttendanceTableWidget()
{
    ui->AttendancetableWidget->verticalHeader()->hide();
    ui->AttendancetableWidget->setRowCount(group->getNumberOfStudents());
    qDebug()<<"here , go here";
    // Call the appropriate function based on session type using a switch statement
    SessionType Session;
    if( ui->RoleCombobox->currentText() == "Lecture")
    {
        this->loadStudentLectureAttendance();
    }
    else if (ui->RoleCombobox->currentText() == "Lab")
    {
        this->loadStudentLabAttendance();
    }
    else if(ui->RoleCombobox->currentText() == "Tutorial")
    {
        this->loadStudentTutorialAttendance();
    }
    else
    {
        qDebug()<<"no Session has been chosen";
    }
}


void RegUser::loadStudentLectureAttendance()
{
    ui->AttendancetableWidget->setColumnCount(course->getNumberofLectureSessions() + 1);
    QStringList HorizontalHeaders;
    HorizontalHeaders << "Students";

    for (int i = 0; i < course->getNumberofLectureSessions(); i++) {
        HorizontalHeaders << QString::number(i + 1);
    }
    ui->AttendancetableWidget->setHorizontalHeaderLabels(HorizontalHeaders);

    // Initialize items in the table
    for (int row = 0; row < ui->AttendancetableWidget->rowCount(); row++) {
        for (int col = 0; col < ui->AttendancetableWidget->columnCount(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            if( col == 0)
            {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                item->setSizeHint(QSize(30, 120));
            }
            ui->AttendancetableWidget->setItem(row, col, item);
        }
    }
    qDebug()<<" Here 2";
    int row = 0;
    for (auto student : group->getGroupStudents()) {
            this->updateTableWidgetLectureStudentAttendance(row,student);
            row++;
    }
}

void RegUser::loadStudentLabAttendance()
{
    ui->AttendancetableWidget->setColumnCount(course->getNumberofLabSessions() + 1);
    QStringList HorizontalHeaders;
    HorizontalHeaders << "Students";

    for (int i = 0; i < course->getNumberofLabSessions(); i++) {
        HorizontalHeaders << QString::number(i + 1);
    }
    ui->AttendancetableWidget->setHorizontalHeaderLabels(HorizontalHeaders);

    // Initialize items in the table
    for (int row = 0; row < ui->AttendancetableWidget->rowCount(); row++) {
        for (int col = 0; col < ui->AttendancetableWidget->columnCount(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            ui->AttendancetableWidget->setItem(row, col, item);
        }
    }

    int row = 0;
    for (auto student : group->getGroupStudents()) {
        this->updateTableWidgetLectureStudentAttendance(row,student);
        row++;
    }
}

void RegUser::loadStudentTutorialAttendance()
{
    ui->AttendancetableWidget->setColumnCount(course->getNumberofTutorialSessions() + 1);
    QStringList HorizontalHeaders;
    HorizontalHeaders << "Students";

    for (int i = 0; i < course->getNumberofTutorialSessions(); i++) {
        HorizontalHeaders << QString::number(i + 1);
    }
    ui->AttendancetableWidget->setHorizontalHeaderLabels(HorizontalHeaders);

    // Initialize items in the table
    for (int row = 0; row < ui->AttendancetableWidget->rowCount(); row++) {
        for (int col = 0; col < ui->AttendancetableWidget->columnCount(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            ui->AttendancetableWidget->setItem(row, col, item);
        }
    }

    int row = 0;
    for (auto student : group->getGroupStudents()) {
        this->updateTableWidgetTutorialStudentAttendance(row,student);
        row++;
    }
}

void RegUser::updateTableWidgetLectureStudentAttendance(int row, Student* student) {
    QTableWidgetItem *Item = ui->AttendancetableWidget->item(row, 0);
    if (Item) {
        Item->setText(student->getName() + " (" + QString::number(student->getID()) + ")");
        QList<bool> lectureAttendance = student->getLectureAttendance();

        // Ensure the lecture attendance list has the expected number of sessions
        if (lectureAttendance.size() != course->getNumberofLectureSessions()) {
            qDebug() << "Lecture attendance size mismatch for student" << student->getName();
            return;
        }

        for (int i = 0; i < course->getNumberofLectureSessions(); i++) {
            Item = ui->AttendancetableWidget->item(row, i + 1);  // +1 to align with column headers
            if (Item) {
                qDebug() << student->getName() << lectureAttendance;
                if (lectureAttendance[i] == false) {
                    Item->setCheckState(Qt::Unchecked);
                } else if (lectureAttendance[i] == true) {
                    Item->setCheckState(Qt::Checked);
                } else {
                    qDebug() << "Item checking is neither true nor false";
                }
            }
        }
    }
}

void RegUser::updateTableWidgetLabStudentAttendance(int row, Student* student) {
    QTableWidgetItem *Item = ui->AttendancetableWidget->item(row, 0);
    if (Item) {
        Item->setText(student->getName() + " (" + QString::number(student->getID()) + ")");
        QList<bool> labAttendance = student->getLabAttendance();

        // Ensure the lab attendance list has the expected number of sessions
        if (labAttendance.size() != course->getNumberofLabSessions()) {
            qDebug() << "Lab attendance size mismatch for student" << student->getName();
            return;
        }

        for (int i = 0; i < course->getNumberofLabSessions(); i++) {
            Item = ui->AttendancetableWidget->item(row, i + 1);  // +1 to align with column headers
            if (Item) {
                qDebug() << student->getName() << labAttendance;
                if (labAttendance[i] == false) {
                    Item->setCheckState(Qt::Unchecked);
                } else if (labAttendance[i] == true) {
                    Item->setCheckState(Qt::Checked);
                } else {
                    qDebug() << "Item checking is neither true nor false";
                }
            }
        }
    }
}

void RegUser::updateTableWidgetTutorialStudentAttendance(int row, Student* student) {
    QTableWidgetItem *Item = ui->AttendancetableWidget->item(row, 0);
    if (Item) {
        Item->setText(student->getName() + " (" + QString::number(student->getID()) + ")");
        QList<bool> tutorialAttendance = student->getTutorialAttendance();

        // Ensure the tutorial attendance list has the expected number of sessions
        if (tutorialAttendance.size() != course->getNumberofTutorialSessions()) {
            qDebug() << "Tutorial attendance size mismatch for student" << student->getName();
            return;
        }

        for (int i = 0; i < course->getNumberofTutorialSessions(); i++) {
            Item = ui->AttendancetableWidget->item(row, i + 1);  // +1 to align with column headers
            if (Item) {
                qDebug() << student->getName() << tutorialAttendance;
                if (tutorialAttendance[i] == false) {
                    Item->setCheckState(Qt::Unchecked);
                } else if (tutorialAttendance[i] == true) {
                    Item->setCheckState(Qt::Checked);
                } else {
                    qDebug() << "Item checking is neither true nor false";
                }
            }
        }
    }
}


void RegUser::on_RoleCombobox_activated()
{

    if( id == SessionType::NoSession && ui->RoleCombobox->currentIndex() != 0)
    {
        if( ui->RoleCombobox->currentText() == "Lecture" )
            id = SessionType::Lecture;
        else if( ui->RoleCombobox->currentText() == "Lab" )
            id = SessionType::Lab;
        else if( ui->RoleCombobox->currentText() == "Tutorial" )
            id = SessionType::Tutorial;
    }
    else
    {
        this->updateAttendanceStructure();
    }
    this->setAttendanceTableWidget();
    if( ui->RoleCombobox->currentText() == "Lecture" )
        id = SessionType::Lecture;
    else if( ui->RoleCombobox->currentText() == "Lab" )
        id = SessionType::Lab;
    else if( ui->RoleCombobox->currentText() == "Tutorial" )
        id = SessionType::Tutorial;
}


void RegUser::updateAttendanceStructure()
{
    QTableWidgetItem* item;
    QVector<bool> attendance;
    for ( int row = 0; row < ui->AttendancetableWidget->rowCount() ; row++ )
    {
        item = ui->AttendancetableWidget->item(row,0);
        if ( item->text() != "")
        {
            Student* student = school->findStudentByName(item->text());
            QTableWidgetItem* iterator;
            for ( int column = 1; column < ui->AttendancetableWidget->columnCount(); column++)
            {
                iterator = ui->AttendancetableWidget->item(row,column);
                if( iterator->checkState() == Qt::Checked)
                {
                    attendance.append(true);
                    qDebug()<<true;
                }
                else if ( iterator->checkState() == Qt::Unchecked)
                {
                    attendance.append(false);
                    qDebug()<<false;
                }
            }
            switch(id)
            {
            case SessionType::Lab :
                student->setLabAttendance(attendance);
                course->updateLabStudentsAttendanceinDatabase(group);
                break;
            case SessionType::Lecture:
                student->setLectureAttendance(attendance);
                course->updateLectureStudentsAttendanceinDatabase(group);
                break;
            case SessionType::Tutorial:
                student->setTutorialAttendance(attendance);
                course->updateTutorialStudentsAttendanceinDatabase(group);
                break;
            }
            attendance.clear();
        }
    }
}

void RegUser::setStudentsExamTableWidgetWidget()
{
    qDebug()<<" Here .... , , , , , "<<group->getNumberOfStudents();
    ui->ExamTableWidget->setRowCount(group->getNumberOfStudents());
    for (int row = 0; row < ui->ExamTableWidget->rowCount(); row++) {
        for (int col = 0; col < ui->ExamTableWidget->columnCount(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            if( col == 0)
            {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                item->setSizeHint(QSize(60, 120));
            }
            ui->ExamTableWidget->setItem(row, col, item);
        }
    }
    int row = 0;
    for (auto student : group->getGroupStudents())
    {
        loadStudentExamToTableWidget(row,student);
        row++;
    }


}

void RegUser::loadStudentExamToTableWidget(int row,Student * student)
{
    QTableWidgetItem *Item = ui->ExamTableWidget->item(row, 0);
    if (Item) {
        Item->setText(student->getName() + " (" + QString::number(student->getID()) + ")");
        QList<float> Grades = student->getGrades();

        // Ensure the tutorial attendance list has the expected number of sessions

        for (int i = 0; i < 4; i++) {
            Item = ui->ExamTableWidget->item(row, i + 1);  // +1 to align with column headers
            if (Item)
            {
                Item->setText(QString::number(Grades[i]));
            }
        }
    }
}
void RegUser::UpdateGradesStructure(Group* group) {
    if (group )
    {
        QRegularExpression re("^([0-9]|1[0-9]|20)(\\.[0-9]{1,10})?$");
        for (int row = 0; row < ui->ExamTableWidget->rowCount(); row++) {
            QTableWidgetItem* Item = ui->ExamTableWidget->item(row, 0);
            if (!Item) {
                qWarning() << "Item at row" << row << "column 0 is null!";
                continue;
            }
            Student* student = school->findStudentByName(Item->text());
            if (!student) {
                qWarning() << "Student not found for name:" << Item->text();
                continue;
            }
            for (int i = 0; i < 4; i++) {
                Item = ui->ExamTableWidget->item(row, i + 1);
                if (!Item) {
                    qWarning() << "Item at row" << row << "column" << (i + 1) << "is null!";
                    continue;
                }
                bool ok;
                float grade = Item->text().toFloat(&ok);
                if (!ok) {
                    qWarning() << "Invalid grade value:" << Item->text();
                    continue;
                }
                student->setGradee(i,grade);
            }
        }
        for (auto student : group->getGroupStudents() ) {
            if (!student) {
                qWarning() << "Null student found in group!";
                continue; // Skip this iteration
            }
            qDebug() << "Processing student:" << student->getName();
            course->WriteStudentGradesToDatabase(student);
        }
    }
}


void RegUser::saveTeacherScheuleWidget()
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
            teacher->AddTimeSlotToSchedule(Key,DataBlock);
            DataBlock.clear();
        }
    }
    school->WriteTeacherScheduleItemsToDatabase(teacher);
}


void RegUser::setTeacherScheuleWidget()
{
    qDebug() << "Setting the Schedule For" << teacher->getName();

    int row = 0;
    QMap<QString, QStringList> Items = teacher->getTeacherScheduleItems();
    ui->ScheduleWidget->setRowCount(Items.count() * 2);

    QStringList Time_Slots;

    for (auto slot : Items.keys())
    {
        Time_Slots << slot << "Group / room";
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

