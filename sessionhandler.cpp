#include "sessionhandler.h"
#include "ui_sessionhandler.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

SessionHandler::SessionHandler(QWidget *parent,School* schoolptr , Course* courseptr, SessionType ID)
    : QDialog(parent)
    , ui(new Ui::SessionHandler)
    ,school(schoolptr)
    ,course(courseptr)
    ,id(ID)
{
    ui->setupUi(this);
    qDebug()<<"issue1";
    setAttendanceTableWidget();
    qDebug()<<"issue 2";
    for( auto group : school->getYears()[course->getyear()-1].getGroups())
    {
        ui->GroupComboBox->addItem(QString::number(group->getGroupNumber()));
    }
    qDebug()<<"issue3";
    for ( auto teacher : course->getCourseTeacher())
    {
        loadTeacher(teacher);
    }
    if (id == SessionType::Lecture)
        ui->SessionName->setText("Lecture");
    else if (id == SessionType::Lab)
        ui->SessionName->setText("Lab");
    else if (id == SessionType::Tutorial)
        ui->SessionName->setText("Tutorial");
    connect(this,&QDialog::rejected,this,[this]()
    {
        this->updateAttendanceStructure();
        if ( id == SessionType::Lab)
        {
            course->updateLabTeachersAttendanceinDatabase();
        }
        if ( id == SessionType::Lecture)
        {
            course->updateLectureTeachersAttendanceinDatabase();
        }
        if ( id == SessionType::Tutorial)
        {
            course->updateTutorialTeachersAttendanceinDatabase();
        }
    });
}

SessionHandler::~SessionHandler()
{
    delete ui;
}

void SessionHandler::loadTeacher(Teacher * teacher)
{
    QListWidgetItem* item = new QListWidgetItem(teacher->getName() + " ("+QString::number(teacher->getID()) +")");
    item->setSizeHint(QSize(10, 20));
    ui->TeacherListWidget->addItem(item);
}

void SessionHandler::on_GroupComboBox_activated(int index)
{
    if(index != 0)
    {
        int Group = ui->GroupComboBox->currentText().toInt();
        if ( id == SessionType::Lecture)
        {
            QMap<int,Teacher*> Teachers(course->getLectureTeachers());
            if (Teachers.contains(Group))
            {
                Teacher* teacher = Teachers.value(Group);
                ui->TeacherlineEdit->setText(teacher->getName() + " (" + QString::number(teacher->getID())+")");
            }
            else
            {
                ui->TeacherlineEdit->setText("");
            }
        }

        else if ( id == SessionType::Lab)
        {
            QMap<int,Teacher*> Teachers(course->getLabTeachers());
            if (Teachers.contains(Group))
            {
                Teacher* teacher = Teachers.value(Group);
                ui->TeacherlineEdit->setText(teacher->getName() + " (" + QString::number(teacher->getID())+")");
            }
            else
            {
                ui->TeacherlineEdit->setText("");
            }
        }

        else if ( id == SessionType::Tutorial)
        {
            QMap<int,Teacher*> Teachers(course->getTutorialTeachers());
            if (Teachers.contains(Group))
            {
                Teacher* teacher = Teachers.value(Group);
                ui->TeacherlineEdit->setText(teacher->getName() + " (" + QString::number(teacher->getID())+")");
            }
            else
            {
                ui->TeacherlineEdit->setText("");
            }
        }
    }
    else
    {
        ui->TeacherlineEdit->setText("");
    }
}

void SessionHandler::on_TeacherListWidget_itemClicked(QListWidgetItem *item)
{
    if ( ui->GroupComboBox->currentIndex() != 0)
    {
        QMessageBox::StandardButton message = QMessageBox::question(this,"Assign Group ?"," Would you like to assign this group to teacher : "+item->text(), QMessageBox::Yes | QMessageBox::No);
        if( message == QMessageBox::Yes )
        {
            Teacher* newteacher = school->findTeacherByName( item->text());
            int Group = ui->GroupComboBox->currentText().toInt();
            Teacher* oldteacher = school->findTeacherByName(ui->TeacherlineEdit->text());
            if( ui->TeacherlineEdit->text() != "")
            {
                if ( id == SessionType::Lecture)
                {
                    course->removeLectureTeacher(Group);
                    course->removeLectureTeacherFromDatabase(Group);
                    // will empty his attendance for lectures with this specific group
                    oldteacher->removeLectureTeacherToGroupAttendance(Group);

                    course->addLectureTeacher(Group,newteacher);
                    course->addLectureTeachertoDatabase(Group,newteacher);
                    int row = this->TableWidgetTableRow(Group);
                    if ( row != -1)
                        this->updateTableWidgetLectureTeacherAttendance(Group,row,newteacher);

                }
                else if ( id == SessionType::Lab)
                {
                    course->removeLabTeacher(Group);
                    course->removeLabTeacherFromDatabase(Group);

                    oldteacher->removeLabTeacherToGroupAttendance(Group);

                    course->addLabTeacher(Group,newteacher);
                    course->addLabTeachertoDatabase(Group,newteacher);

                    int row = this->TableWidgetTableRow(Group);
                    if (row != -1)
                        this->updateTableWidgetLabTeacherAttendance(Group,row,newteacher);
                }
                else if ( id == SessionType::Tutorial)
                {
                    course->removeTutorialTeacher(Group);
                    course->removeTutorialTeacherFromDatabase(Group);

                    oldteacher->removeTutorialTeacherToGroupAttendance(Group);

                    course->addTutorialTeacher(Group,newteacher);
                    course->addTutorialTeachertoDatabase(Group,newteacher);

                    int row = this->TableWidgetTableRow(Group);
                    if ( row != -1 )
                        this->updateTableWidgetTutorialTeacherAttendance(Group,row,newteacher);
                };
            }
            else
            {
                if ( id == SessionType::Lecture)
                {
                    course->addLectureTeacher(Group,newteacher);
                    course->addLectureTeachertoDatabase(Group,newteacher);
                    int row = this->TableWidgetTableRow(Group);
                    if ( row != -1 )
                        this->updateTableWidgetLectureTeacherAttendance(Group,row,newteacher);
                }
                else if ( id == SessionType::Lab)
                {
                    course->addLabTeacher(Group,newteacher);
                    course->addLabTeachertoDatabase(Group,newteacher);

                    int row = this->TableWidgetTableRow(Group);
                    if ( row != -1 )
                        this->updateTableWidgetLabTeacherAttendance(Group,row,newteacher);
                }
                else if ( id == SessionType::Tutorial)
                {
                    course->addTutorialTeacher(Group,newteacher);
                    course->addTutorialTeachertoDatabase(Group,newteacher);

                    int row = this->TableWidgetTableRow(Group);
                    if ( row != -1 )
                        this->updateTableWidgetTutorialTeacherAttendance(Group,row,newteacher);
                }
            }
            ui->TeacherlineEdit->setText(newteacher->getName() + " (" + QString::number(newteacher->getID()) + ")");
        }
    }
}
void SessionHandler::loadLectureTeacherAttendancetoTableWidget()
{
    // Set the column count and headers
    ui->TeacherAttendanceTableWidget->setColumnCount(course->getNumberofLectureSessions() + 1);
    QStringList HorizontalHeaders;
    HorizontalHeaders << "Teachers";

    for (int i = 0; i < course->getNumberofLectureSessions(); i++) {
        HorizontalHeaders << QString::number(i + 1);
    }
    ui->TeacherAttendanceTableWidget->setHorizontalHeaderLabels(HorizontalHeaders);

    // Initialize items in the table
    for (int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount(); row++) {
        for (int col = 0; col < ui->TeacherAttendanceTableWidget->columnCount(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            ui->TeacherAttendanceTableWidget->setItem(row, col, item);
        }
    }

    QMap<int, Teacher*> LectureTeachers = course->getLectureTeachers();
    int groupNumber;
    QString VerticalHeader;
    QTableWidgetItem *Item;

    for (int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount(); row++) {
        VerticalHeader = ui->TeacherAttendanceTableWidget->verticalHeaderItem(row)->text();
        groupNumber = VerticalHeader.split(" ")[1].toInt();
        qDebug() << "Processing group number:" << groupNumber;

        if (LectureTeachers.contains(groupNumber)) {
            qDebug() << "There is a teacher for group" << groupNumber;
            Teacher *teacher = LectureTeachers[groupNumber];
            this->updateTableWidgetLectureTeacherAttendance(groupNumber,row,teacher);
        } else {
            qDebug() << "No teacher for group" << groupNumber;
        }
    }

}
void SessionHandler::loadTutorialTeacherAttendancetoTableWidget()
{
    // Set the column count and headers
    ui->TeacherAttendanceTableWidget->setColumnCount(course->getNumberofTutorialSessions() + 1);
    QStringList HorizontalHeaders;
    HorizontalHeaders << "Teachers";

    for (int i = 0; i < course->getNumberofTutorialSessions(); i++) {
        HorizontalHeaders << QString::number(i + 1);
    }
    ui->TeacherAttendanceTableWidget->setHorizontalHeaderLabels(HorizontalHeaders);

    // Initialize items in the table
    for (int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount(); row++) {
        for (int col = 0; col < ui->TeacherAttendanceTableWidget->columnCount(); col++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            ui->TeacherAttendanceTableWidget->setItem(row, col, item);
        }
    }

    QMap<int, Teacher*> TutorialTeachers = course->getTutorialTeachers();
    int groupNumber;
    QString VerticalHeader;
    QTableWidgetItem *Item;

    for (int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount(); row++) {
        VerticalHeader = ui->TeacherAttendanceTableWidget->verticalHeaderItem(row)->text();
        groupNumber = VerticalHeader.split(" ")[1].toInt();
        qDebug() << "Processing group number:" << groupNumber;

        if (TutorialTeachers.contains(groupNumber))
        {
            Teacher *teacher = TutorialTeachers[groupNumber];
            updateTableWidgetTutorialTeacherAttendance(groupNumber,row,teacher);
        } else {
            qDebug() << "No teacher for group" << groupNumber;
        }
    }

}
void SessionHandler::loadLabTeacherAttendancetoTableWidget()
{
    // Set the column count and headers
    ui->TeacherAttendanceTableWidget->setColumnCount(course->getNumberofLabSessions() + 1);
    qDebug()<<"the number of lab sessions is : "<<ui->TeacherAttendanceTableWidget->columnCount();
    QStringList HorizontalHeaders;
    HorizontalHeaders << "Teachers";

    for (int i = 0; i < course->getNumberofLabSessions(); i++) {
        HorizontalHeaders << QString::number(i + 1);
    }
    ui->TeacherAttendanceTableWidget->setHorizontalHeaderLabels(HorizontalHeaders);

    // Initialize items in the table
    for (int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount(); row++)
    {
        for (int col = 0; col < ui->TeacherAttendanceTableWidget->columnCount(); col++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            ui->TeacherAttendanceTableWidget->setItem(row, col, item);
        }
    }

    QMap<int, Teacher*> LabTeachers = course->getLabTeachers();
    int groupNumber;
    QString VerticalHeader;
    QTableWidgetItem *Item;
    qDebug()<<"the issue is here";
    for (int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount(); row++)
    {
        VerticalHeader = ui->TeacherAttendanceTableWidget->verticalHeaderItem(row)->text();
        groupNumber = VerticalHeader.split(" ")[1].toInt();
        if (LabTeachers.contains(groupNumber))
        {
            Teacher *teacher = LabTeachers[groupNumber];
            updateTableWidgetLabTeacherAttendance(groupNumber,row,teacher);
        }
        else
        {
            qDebug() << "No teacher for group" << groupNumber;
        }
    }

}
void SessionHandler::setAttendanceTableWidget()
{
    QStringList verticalHeaders;
    int numberOfGroups = 0;
    for (const Group* group : school->getYears()[course->getyear()-1].getGroups()) {
        verticalHeaders << "Group " + QString::number(group->getGroupNumber());
        numberOfGroups++;
    }
    ui->TeacherAttendanceTableWidget->setRowCount(numberOfGroups);
    ui->TeacherAttendanceTableWidget->setVerticalHeaderLabels(verticalHeaders);
    qDebug()<<"here , go here";
    // Call the appropriate function based on session type using a switch statement
    switch (id) {
    case SessionType::Lecture:
        loadLectureTeacherAttendancetoTableWidget();
        break;
    case SessionType::Lab:
        loadLabTeacherAttendancetoTableWidget();
        break;
    case SessionType::Tutorial:
        loadTutorialTeacherAttendancetoTableWidget();
        break;
    default:
        // Handle unexpected session type (optional)
        break;
    }
}


void SessionHandler::updateTableWidgetLectureTeacherAttendance(int groupNumber,int row,Teacher* teacher)
{
    QTableWidgetItem *Item = ui->TeacherAttendanceTableWidget->item(row, 0);

    if (Item) {
        Item->setText(teacher->getName()+" (" + QString::number(teacher->getID())+")");

        for (int i = 1; i <= course->getNumberofLectureSessions(); i++) {
            Item = ui->TeacherAttendanceTableWidget->item(row, i);

            if (Item) {
                qDebug()<<teacher->getName()<<teacher->getLectureAttendance()[groupNumber][i - 1];
                if (teacher->getLectureAttendance()[groupNumber][i - 1] == false)
                {
                    Item->setCheckState(Qt::Unchecked);
                }
                else if (teacher->getLectureAttendance()[groupNumber][i - 1] == true)
                {
                    Item->setCheckState(Qt::Checked);
                }
                else
                {
                    qDebug()<<"item checking is neither true nor false";
                }
            }
        }
    }
}
void SessionHandler::updateTableWidgetLabTeacherAttendance(int groupNumber,int row,Teacher* teacher)
{
    QTableWidgetItem *Item = ui->TeacherAttendanceTableWidget->item(row, 0);

    if (Item) {
        Item->setText(teacher->getName()+" (" + QString::number(teacher->getID())+")");

        for (int i = 1; i <= course->getNumberofLabSessions(); i++) {
            Item = ui->TeacherAttendanceTableWidget->item(row, i);

            if (Item) {
                qDebug()<<teacher->getName()<<teacher->getLabAttendance()[groupNumber][i - 1];
                if (teacher->getLabAttendance()[groupNumber][i - 1] == false)
                {
                    Item->setCheckState(Qt::Unchecked);
                }
                else if (teacher->getLabAttendance()[groupNumber][i - 1] == true)
                {
                    Item->setCheckState(Qt::Checked);
                }
                else
                {
                    qDebug()<<"item checking is neither true nor false";
                }
            }
        }
    }

}
void SessionHandler::updateTableWidgetTutorialTeacherAttendance(int groupNumber,int row,Teacher* teacher)
{

    QTableWidgetItem *Item = ui->TeacherAttendanceTableWidget->item(row, 0);

    if (Item) {
        Item->setText(teacher->getName()+" (" + QString::number(teacher->getID())+")");

        for (int i = 1; i <= course->getNumberofTutorialSessions(); i++) {
            Item = ui->TeacherAttendanceTableWidget->item(row, i);

            if (Item) {
                qDebug()<<teacher->getName()<<teacher->getTutorialAttendance()[groupNumber][i - 1];
                if (teacher->getTutorialAttendance()[groupNumber][i - 1] == false)
                {
                    Item->setCheckState(Qt::Unchecked);
                }
                else if (teacher->getTutorialAttendance()[groupNumber][i - 1] == true)
                {
                    Item->setCheckState(Qt::Checked);
                }
                else
                {
                    qDebug()<<"item checking is neither true nor false";
                }
            }
        }
    }
}

int SessionHandler::TableWidgetTableRow(int groupNumber)
{
    QString Group = "Group "+ QString::number(groupNumber);
    for ( int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount(); row++ )
    {
        if( Group == ui->TeacherAttendanceTableWidget->verticalHeaderItem(row)->text())
        {
            return row;
        }
    }
    return -1;
}


void SessionHandler::updateAttendanceStructure()
{
    QTableWidgetItem* item;
    QVector<bool> attendance;
    qDebug()<<"the new attendance is :";
    if( id == SessionType::Lecture )
    {
        for ( int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount() ; row++ )
        {

            item = ui->TeacherAttendanceTableWidget->item(row,0);
            if ( item->text() != "")
            {
                Teacher* teacher = school->findTeacherByName(item->text());
                QString VerticalHeader = ui->TeacherAttendanceTableWidget->verticalHeaderItem(row)->text();
                int groupNumber = VerticalHeader.split(" ")[1].toInt();
                QTableWidgetItem* iterator;
                for ( int column = 1; column < ui->TeacherAttendanceTableWidget->columnCount(); column++)
                {
                    iterator = ui->TeacherAttendanceTableWidget->item(row,column);
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
                teacher->setLectureAttendance(groupNumber,attendance);
                attendance.clear();
            }
        }
    }
    else if ( id == SessionType::Lab )
    {
        for ( int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount() ; row++ )
        {

            item = ui->TeacherAttendanceTableWidget->item(row,0);
            if ( item->text() != "")
            {
                Teacher* teacher = school->findTeacherByName(item->text());
                QString VerticalHeader = ui->TeacherAttendanceTableWidget->verticalHeaderItem(row)->text();
                int groupNumber = VerticalHeader.split(" ")[1].toInt();
                QTableWidgetItem* iterator;
                for ( int column = 1; column < ui->TeacherAttendanceTableWidget->columnCount(); column++)
                {
                    iterator = ui->TeacherAttendanceTableWidget->item(row,column);
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
                teacher->setLabAttendance(groupNumber,attendance);
                attendance.clear();
            }
        }
    }
    else if( id == SessionType::Tutorial)
    {
        for ( int row = 0; row < ui->TeacherAttendanceTableWidget->rowCount() ; row++ )
        {

            item = ui->TeacherAttendanceTableWidget->item(row,0);
            if ( item->text() != "")
            {
                Teacher* teacher = school->findTeacherByName(item->text());
                QString VerticalHeader = ui->TeacherAttendanceTableWidget->verticalHeaderItem(row)->text();
                int groupNumber = VerticalHeader.split(" ")[1].toInt();
                QTableWidgetItem* iterator;
                for ( int column = 1; column < ui->TeacherAttendanceTableWidget->columnCount(); column++)
                {
                    iterator = ui->TeacherAttendanceTableWidget->item(row,column);
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
                teacher->setTutorialAttendance(groupNumber,attendance);
                attendance.clear();
            }
        }
    }
}





void SessionHandler::on_UnassignButton_clicked()
{
    if( ui->TeacherlineEdit->text() != "")
    {
        int Group = ui->GroupComboBox->currentText().toInt();
        if ( id == SessionType::Lecture)
        {
            Teacher* teacher = school->findTeacherByName(ui->TeacherlineEdit->text());
            course->removeLectureTeacher(Group);
            course->removeLectureTeacherFromDatabase(Group);
            // will empty his attendance for lectures with this specific group
            teacher->removeLectureTeacherToGroupAttendance(Group);
            int row = this->TableWidgetTableRow(Group);
            QTableWidgetItem* item = new QTableWidgetItem("");
            for (int col = 0; col < ui->TeacherAttendanceTableWidget->columnCount(); ++col)
            {
                QTableWidgetItem* item = new QTableWidgetItem("");
                delete ui->TeacherAttendanceTableWidget->item(row,col);
                ui->TeacherAttendanceTableWidget->setItem(row, col, item);
            }
            ui->TeacherlineEdit->setText("");
        }
        else if ( id == SessionType::Lab)
        {
            Teacher* teacher = school->findTeacherByName(ui->TeacherlineEdit->text());
            course->removeLabTeacher(Group);
            course->removeLabTeacherFromDatabase(Group);
            // will empty his attendance for lectures with this specific group
            teacher->removeLabTeacherToGroupAttendance(Group);
            int row = this->TableWidgetTableRow(Group);
            QTableWidgetItem* item = new QTableWidgetItem("");
            for (int col = 0; col < ui->TeacherAttendanceTableWidget->columnCount(); ++col)
            {
                QTableWidgetItem* item = new QTableWidgetItem("");
                delete ui->TeacherAttendanceTableWidget->item(row,col);
                ui->TeacherAttendanceTableWidget->setItem(row, col, item);
            }
            ui->TeacherlineEdit->setText("");
        }
        else if ( id == SessionType::Tutorial)
        {
            Teacher* teacher = school->findTeacherByName(ui->TeacherlineEdit->text());
            course->removeTutorialTeacher(Group);
            course->removeTutorialTeacherFromDatabase(Group);
            // will empty his attendance for lectures with this specific group
            teacher->removeTutorialTeacherToGroupAttendance(Group);
            int row = this->TableWidgetTableRow(Group);
            QTableWidgetItem* item = new QTableWidgetItem("");
            for (int col = 0; col < ui->TeacherAttendanceTableWidget->columnCount(); ++col)
            {
                QTableWidgetItem* item = new QTableWidgetItem("");
                delete ui->TeacherAttendanceTableWidget->item(row,col);
                ui->TeacherAttendanceTableWidget->setItem(row, col, item);
            }
            ui->TeacherlineEdit->setText("");
        }
    }
}
