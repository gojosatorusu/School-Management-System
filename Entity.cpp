#include "Entity.h"
#include <QRegularExpression>
#include <QString>
//  ----------------------------- this section is for the implementation of the Entity class -------------------------------------------//

Entity::Entity(QSqlDatabase* sqlbase,QString Name,QString Email  ,QString Phone ,int id  ,int Sqlbaseid, QString path,QString Gender,bool deletion):Name(Name),Email(Email),phone_Number(Phone),ID(id),Sqlbase(sqlbase),databaseID(Sqlbaseid),imagepath(path),gender(Gender),deletion_State(deletion){}

QString Entity::getName() const {
    return Name;
}

int Entity::getID() const {
    return ID;
}

QString Entity::getEmail() const {
    return Email;
}

QString Entity::getPhone_Number() const {
    return phone_Number;
}
int Entity::getDatabaseID(){
    return databaseID;
}

QString Entity::getImagepath()
{
    return imagepath;
}

QString Entity::getgender()
{
    return gender;
}
bool Entity::getdeletion_State()
{
    return deletion_State;
}

Entity::~Entity(){}
// Implementation of setters
void Entity::setName(const QString &name) {
    Name = name;
}

void Entity::setID(int id) {
    ID = id;
}

void Entity::setEmail(const QString &email) {
    Email = email;
}

void Entity::setPhone_Number(const QString &phoneNumber) {
    phone_Number = phoneNumber;
}

void Entity::setDatabaseID(int id){
    databaseID=id;
}

void Entity::setImagepath(QString image){
    imagepath= image;
}

void Entity::setgender(QString Gender)
{
    gender = Gender;
}
void Entity::setdeletion_State(bool state)
{
    deletion_State = state;
}


//  ----------------------------- this section is for the implementation of the Teacher class -------------------------------------------//


Teacher::Teacher(QSqlDatabase* sqlbase,QString name, QString email, QString phoneNumber, int id,QString password,int Sqlbaseid,QString path,QString gender,bool deletion)
    : Entity(sqlbase,name, email, phoneNumber, id,Sqlbaseid,path,gender,deletion), Password(password){}

Teacher::~Teacher() {}
    QMap<int, QVector<bool>> Teacher::getLectureAttendance() const{
        return Lecture_Attendance;
    }
    QMap<int, QVector<bool>> Teacher::getLabAttendance() const{
        return Lab_Attendance;
    }
    QMap<int, QVector<bool>> Teacher::getTutorialAttendance() const{
        return Tutorial_Attendance;
    }
    QString Teacher::getpassword() const
    {
        return Password;
    }

// Setter for attendance
void Teacher::setLectureAttendance(int Group , QVector<bool> lectureattendance )
{
    this->Lecture_Attendance[Group]=lectureattendance;
}
void Teacher::setLabAttendance(int Group , QVector<bool> labattendance)
{
    this->Lab_Attendance[Group]=labattendance;
}
void Teacher::setTutorialAttendance(int Group , QVector<bool> tutorialattendance)
{
    this->Tutorial_Attendance[Group]=tutorialattendance;
}

//remove specific group attendance
void Teacher::removeLectureTeacherToGroupAttendance(int group)
{
    this->Lecture_Attendance.remove(group);
}
void Teacher::removeTutorialTeacherToGroupAttendance(int group)
{
    this->Tutorial_Attendance.remove(group);
}
void Teacher::removeLabTeacherToGroupAttendance(int group)
{
    this->Lab_Attendance.remove(group);
}
//

void Teacher::setpassword(const QString &pass)
{
    Password=pass;
}

// add attendance :
void Teacher::addLectureAttendance(int group, QVector<bool>& attendance){
    Lecture_Attendance[group]=attendance;
}
void Teacher::addLabAttendance(int group, QVector<bool>& attendance){
    Lab_Attendance[group]=attendance;
}
void Teacher::addTutorialAttendance(int group, QVector<bool>& attendance){
    Tutorial_Attendance[group]=attendance;
}

double Teacher::get_Attendance_rate()
{
    double rate(0.0);
    int sessions(0);
    for (auto attendvector : Lab_Attendance )
    {
        for (auto attend : attendvector)
        {
            sessions++;
            if( attend == false)
                rate++;
        }
    }
    for (auto attendvector : Tutorial_Attendance )
    {
        for (auto attend : attendvector)
        {
            sessions++;
            if( attend == false)
                rate++;
        }
    }
    for (auto attendvector : Lecture_Attendance )
    {
        for (auto attend : attendvector)
        {
            sessions++;
            if( attend == false)
                rate++;
        }
    }
    return (rate/sessions)*100;
}


void Teacher::update_Teacher(QSqlDatabase* sqlbase) {
    if (!sqlbase ) {
        qWarning() << "pointer is nullptr. Cannot update teacher.";
        return;
    }
    if (!sqlbase->open()){
        qWarning()<<"database is not open";
    }

    //Prepare the update query
    QString query = "UPDATE Teachers SET Name = :name, `E-mail` = :email, PhoneNumber = :phone, Password = :password, TeacherID = :id, ImagePath = :path, Gender = :gender, Deleted = :deleted WHERE DatabaseID = :Sqlbaseid ";
    // Create a QSqlQuery object
    QSqlQuery updateQuery(*sqlbase);

    // Bind values to the query parameters
    updateQuery.prepare(query);
    updateQuery.bindValue(":name", getName());
    updateQuery.bindValue(":email", getEmail());
    updateQuery.bindValue(":password", getpassword());
    updateQuery.bindValue(":id", getID());
    updateQuery.bindValue(":Sqlbaseid", getDatabaseID());
    updateQuery.bindValue(":phone", getPhone_Number());
    updateQuery.bindValue(":path", getImagepath());
    updateQuery.bindValue(":gender", getgender());
    updateQuery.bindValue(":deleted",this->getdeletion_State());
    // Execute the update query
    if (!updateQuery.exec()) {
        qWarning() << "Failed to update teacher:" << updateQuery.lastError().text();
        return;
    }

    qDebug() << "Teacher information successfully updated in the database.";
}


void Teacher::setLectureAttendanceString(QString state,int group)
{
    QVector<bool> attendance;
    for (auto bit: state.split(""))
    {
        if( bit == "0")
        {
            attendance.append(false);
        }
        else if ( bit =="1")
        {
            attendance.append(true);
        }
    }
    this->Lecture_Attendance[group] = attendance;

}
void Teacher::setLabAttendanceString(QString state,int group)
{
    QVector<bool> attendance;
    for (auto bit: state.split(""))
    {
        if( bit == "0")
        {
            attendance.append(false);
        }
        else if ( bit =="1")
        {
            attendance.append(true);
        }
    }

    this->Lab_Attendance[group] = attendance;
}
void Teacher::setTutorialAttendanceString(QString state, int group)
{
    QVector<bool> attendance;
    for (auto bit: state.split(""))
    {
        if( bit == "0")
        {
            attendance.append(false);
        }
        else if ( bit =="1")
        {
            attendance.append(true);
        }
    }
    this->Tutorial_Attendance[group] = attendance;
}



void Teacher::removeLectureAttendance()
{
    this->Lecture_Attendance.clear();
}
void Teacher::removeLabAttendance()
{
    this->Lab_Attendance.clear();
}
void Teacher::removeTutorialAttendance()
{
    this->Tutorial_Attendance.clear();
}



QMap<QString,QStringList> Teacher::getTeacherScheduleItems()
{
    return this->TeacherScheduleItems;
}



void Teacher::AddTimeSlotToSchedule(const QString & Time_Slot,QStringList Schedulerow)
{
    if (this->TeacherScheduleItems.contains(Time_Slot)) {
        // Append to existing time slot
        this->TeacherScheduleItems[Time_Slot]=Schedulerow;
    } else {
        // Add new time slot
        this->TeacherScheduleItems.insert(Time_Slot, Schedulerow);
    }

    qDebug() << "Current TeacherScheduleItems:" << TeacherScheduleItems;
}

void Teacher::removeTimeSlotFromSchedule(const QString & Time_Slot)
{
    this->TeacherScheduleItems.remove(Time_Slot);
    qDebug()<<this->TeacherScheduleItems;
}

//  ----------------------------- this section is for the implementation of the STUDENT class -------------------------------------------//


// Constructors


Student::Student(QSqlDatabase* sqlbase,QString name, QString email, QString phoneNumber, int id, int year, int Group,int Sqlbaseid, QString path,QString gender, QString Position,bool deletion)
        : Entity(sqlbase,name, email, phoneNumber, id,Sqlbaseid,path,gender,deletion), year(year),group(Group),position(Position)
{

}

Student::~Student() {}

// Getter for attendance
 QVector<bool> Student::getLabAttendance() const
{
     return this->LabAttendance;
}
QVector<bool> Student::getTutorialAttendance() const{
    return this->TutorialAttendance;
}
QVector<bool> Student::getLectureAttendance() const{
    return this->LectureAttendance;
}


// Setter for attendance

void Student::setLabAttendance( QVector<bool> labAttendance)
{
    this->LabAttendance = labAttendance;
}
void Student::setTutorialAttendance(QVector<bool> tutoattendance )
{
    this->TutorialAttendance = tutoattendance;
}
void Student::setLectureAttendance(QVector<bool> lectureattendance )
{
    this->LectureAttendance = lectureattendance;
}

// Getter for year
int Student::getYear() const {
    return year;
}

int Student::getgroup()const {
    return this->group;
}
QString Student::getposition(){
    return position;
}

void Student::update_Student(QSqlDatabase* sqlbase) // used to update the teacher data in the database after the admin changes them
{
    if (!sqlbase ) {
        qWarning() << "pointer is nullptr. Cannot update teacher.";
        return;
    }
    if (!sqlbase->open()){
        qWarning()<<"database is not open";
    }
    QSqlQuery query(*sqlbase);
    query.prepare("UPDATE Students SET Name = :name, `E-mail` = :email, PhoneNumber = :phone, Year = :year, `Group` = :group, ID = :id, ImagePath = :image, Gender = :gender, Position = :position, Deleted = :deleted WHERE DatabaseID = :Sqlbaseid");
    query.bindValue(":name", this->getName());
    query.bindValue(":email", this->getEmail());
    query.bindValue(":year", this->getYear());
    query.bindValue(":phone", this->getPhone_Number());
    query.bindValue(":group",this->getgroup());
    query.bindValue(":id", this->getID());
    query.bindValue(":Sqlbaseid", this->getDatabaseID());
    query.bindValue(":image",this->getImagepath());
    query.bindValue(":gender",this->getgender());
    query.bindValue(":position",this->getposition());
    query.bindValue(":deleted",this->getdeletion_State());

    if (!query.exec()) {
        qDebug() << "Failed to update student in the database:" << query.lastError().text();
    } else {
        qDebug() << "student updated in the database successfully.";
    }

}
void Student::setyear(int year){
    this->year=year;
}
void Student::setgroup(int group) {
    this->group = group;
}
void Student::setposition(QString pos)
{
    position = pos;
}


void Student::emptyLabAttendance()
{
    this->LabAttendance.clear();
}

void Student::emptyLectureAttendance()
{
    this->LectureAttendance.clear();
}

void Student::emptyTutorialAttendance()
{
    this->TutorialAttendance.clear();
}


void Student::setMidterm(float Grade)
{
    this->Grades[0] = Grade;
}

void Student::setT_L_Avg(float Grade)
{
    this->Grades[1] = Grade;
}

void Student::setFinal_Exam(float Grade)
{
    this->Grades[2] = Grade;
}

void Student::setAvg(float Grade)
{
    this->Grades[3] = Grade;
}

QList<float> Student::getGrades()
{
    return this->Grades;
}

void Student::setGradee(int i , float Grade)
{
    if ( 0 <= i && i <= 3)
        this->Grades[i] = Grade;
}

//  ----------------------------- this section is for the implementation of the Group class -------------------------------------------//           //Finished normally



Group::Group() : group_Number(0), Number_of_students(0),year(0) {}

Group::Group(int groupNumber,int year, QSqlDatabase* sqlbase) : group_Number(groupNumber),year(year), Number_of_students(0),Sqlbase(sqlbase),delegate(nullptr),SubDelegate(nullptr)
{
    loadGroupStudentsFromDatabase();
    loadGroupScheduleItems();

}
void Group::loadGroupStudentsFromDatabase() {
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    query.prepare("SELECT `Name`, `ID`, `E-mail`, `PhoneNumber`, DatabaseID, ImagePath, Gender, Position, Deleted FROM Students WHERE Year = :year AND `Group` = :group_Number");
    query.bindValue(":year", year);
    query.bindValue(":group_Number", group_Number);

    if (!query.exec()) {
        qWarning() << "Failed to query students:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        int id = query.value(1).toInt();
        QString email = query.value(2).toString();
        QString phonenumber = query.value(3).toString();
        int Sqlbaseid = query.value(4).toInt();
        QString Imagepath = query.value(5).toString();
        QString gender = query.value(6).toString();
        QString position = query.value(7).toString();
        bool deleted = query.value(8).toBool();
        Student* student = new Student(Sqlbase, name, email, phonenumber, id, year,this->getGroupNumber(),Sqlbaseid,Imagepath,gender,position,deleted);
        Students.append(student);
        if (position == "Delegate")
        {
            delegate = student;
        }
        else if (position == "Sub-Delegate")
        {
            SubDelegate = student;
        }
        qDebug() << "Loaded student:" << name << " " << id << " " << year << " " << group_Number << " " << email << " " << phonenumber<< "   "<<gender<< " "<<position;
        if (!student->getdeletion_State())
            Number_of_students++;
    }
    qDebug()<<"the number of students in the group number "<<group_Number<<" is : "<<Number_of_students;
}

void Group::loadGroupScheduleItems()    // 10:00-11:00[English:(Hamouda,13).Analysis 1:(Hamouda,2).Analysis 3]/11:00-12:00
{
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    query.prepare("SELECT ScheduleItems FROM StudentSchedules WHERE Year = :year AND `Group` = :group_Number");
    query.bindValue(":year", year);
    query.bindValue(":group_Number", group_Number);

    if (!query.exec()) {
        qWarning() << "Failed to query studentsSchedules" << query.lastError().text();
        return;
    }

    if (query.next()) {
        QString Schedule = query.value(0).toString();
        if (!Schedule.isEmpty()) {
            QStringList Items = Schedule.split("/", Qt::SkipEmptyParts);
            for (const QString &item : Items)
            {
                QString Time_Slot;
                QStringList Courses;

                if (!item.isEmpty())
                {
                    int indexOfBracket = item.indexOf('[');
                    if (indexOfBracket != -1) {
                        Time_Slot = item.left(indexOfBracket);
                        QString coursesStr = item.mid(indexOfBracket + 1, item.length() - indexOfBracket - 2); // Remove last ']' character
                        Courses = coursesStr.split('.', Qt::SkipEmptyParts);
                    }
                    else
                    {
                        Time_Slot = item;
                    }
                }

                if (!Time_Slot.isEmpty()) {
                    this->GroupScheduleItems.insert(Time_Slot, Courses);
                }
            }
        }
    }

}

void Group::WriteScheduleItemsToDatabase()
{
    qDebug() << "Saving the Items";

    QString items;
    qDebug()<<"keys inside the saving function";
    for ( auto key : GroupScheduleItems.keys())
    {
        qDebug()<<key;
    }
    qDebug()<<GroupScheduleItems;
    qDebug()<<year<<"    "<<group_Number;

    for (auto key : GroupScheduleItems.keys()) {
        items += key;

        if (GroupScheduleItems[key].isEmpty()) {
            items += "/";
            continue;
        }

        items += "[";
        for (auto cell : GroupScheduleItems[key]) {
            items += cell + ".";
            qDebug()<<cell;
        }
        items.chop(1);
        items += "]/";
    }
    items.chop(1);

    qDebug() << "Items to save for Group Number Y"<<year<<"G"<<group_Number<<" :"  << items;

    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    query.prepare("UPDATE StudentSchedules SET ScheduleItems = :items WHERE Year = :year AND `Group` = :group_Number");
    query.bindValue(":year", this->year);
    query.bindValue(":group_Number", group_Number);
    query.bindValue(":items", items);

    if (!query.exec()) {
        qWarning() << "Failed to write to StudentSchedules: " << query.lastError().text();
    } else {
        qDebug() << "Items saved successfully";
    }
}



void Group::AddTimeSlotToSchedule(const QString &Time_Slot, QStringList Schedulerow)
{
    if (this->GroupScheduleItems.contains(Time_Slot))
    {
        // Append to existing time slot
        this->GroupScheduleItems[Time_Slot]=Schedulerow;
    } else {
        // Add new time slot
        this->GroupScheduleItems.insert(Time_Slot, Schedulerow);
    }

    qDebug() << "Current GroupScheduleItems:" << GroupScheduleItems;
}


void Group::removeTimeSlotFromSchedule(const QString & Time_Slot)
{
    this->GroupScheduleItems.remove(Time_Slot);
}

Group::~Group()
{
}

void Group::setGroupScheduleItems(const QMap<QString,QStringList> & Items)
{
    this->GroupScheduleItems = Items;
}
QMap<QString,QStringList> Group::getGroupScheduleItems()
{
    return this->GroupScheduleItems;
}


// Getters
int Group::getGroupNumber() const {
    return group_Number;
}

const QList<Student*> Group::getGroupStudents() const  {
    return Students;
}

int Group::getNumberOfStudents() const {
    int number = 0;
    for (auto student : getGroupStudents())
    {
        if ( ! student->getdeletion_State() )
        {
            number ++;
        }
    }
    return number;
}
int Group::getyear(){
    return year;
}

Student* Group::getDelegate()
{
    return delegate;
}

Student* Group::getSubDelegate()
{
    return SubDelegate;
}
double Group::MaleRate()
{
    if ( getNumberOfStudents() != 0)
    {
        double rate = 0;
        for( auto student: Students)
        {
            if (student->getgender() == "M" && !student->getdeletion_State())
                rate++;
        }
        return (rate / getNumberOfStudents());
    }
    return 0;
}
double Group::FemaleRate()
{
    if ( getNumberOfStudents() != 0)
    {
        double rate = 0;
        for( auto student: Students)
        {
            if (student->getgender() == "F" && !student->getdeletion_State())
                rate++;
        }
        return (rate / getNumberOfStudents());
    }
    return 0;
}
// Setters
void Group::setGroupNumber(int groupNumber) {
    group_Number = groupNumber;
}

void Group::setGroupStudents(const QList<Student*>& students) {
    Students = students;
    updateNumberOfStudents();
}
void Group::setYear(int Year){ year = Year;}

void Group::setDelegate(Student * Delegate)
{
    delegate = Delegate;
}

void Group::setSubDelegate(Student * SubDelegate)
{
    this->SubDelegate = SubDelegate;
}

// Methods to manipulate students
void Group::addStudent(Student* student)
{

    Students.append(student);
    updateNumberOfStudents();

}

void Group::removeStudent(Student* student) {
    Students.removeOne(student);
    updateNumberOfStudents();
}

// Helper method to update the number of students
void Group::updateNumberOfStudents() {
    Number_of_students = Students.size();
}


bool Group::Alldeleted()
{
    for ( auto student : this->getGroupStudents())
    {
        if ( !student->getdeletion_State())
        {
            return false;
        }
    }
    return true;
}


//      /////////////////////////////////////this secion is for the Exam class\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


Exam::Exam()
    : examDay(QDate()), startTime(QTime()), endTime(QTime()) {}

// Parameterized constructor
Exam::Exam(const QDate &examDay, const QTime &startTime, const QTime &endTime)
    : examDay(examDay), startTime(startTime), endTime(endTime) {}

// Getters
QDate Exam::getExamDay() const {
    return examDay;
}

QTime Exam::getStartTime() const {
    return startTime;
}

QTime Exam::getEndTime() const {
    return endTime;
}

// Setters
void Exam::setExamDay(const QDate &examDay) {
    if (examDay.isValid()) {
        this->examDay = examDay;
    }
}

void Exam::setStartTime(const QTime &startTime) {
    if (startTime.isValid()) {
        this->startTime = startTime;
    }
}

void Exam::setEndTime(const QTime &endTime) {
    if (endTime.isValid()) {
        this->endTime = endTime;
    }
}

//  ----------------------------- this section is for the implementation of the Course class -------------------------------------------//



// Constructors
Course::Course() : ID(0) {}

Course::Course(QSqlDatabase* sqlbase, int id, const QString &courseName, int coef, int tutosessions, int labsessions,QList<Teacher *>  Teachers, int NumberOfLectureSessions, int Semester, QString HASsessions, int Year)
    : ID(id)
    ,Course_name(courseName)
    ,Sqlbase(sqlbase)
    ,coefficient(coef)
    ,Number_Of_Lab_Sessions(labsessions)
    ,Number_Of_Tutorial_Sessions(tutosessions)
    ,Number_Of_Lecture_Sessions(NumberOfLectureSessions)
    ,semester(Semester)
    ,year(Year)
{
    extractNumbers(HASsessions);
    loadTeacherRole(Teachers);
    loadExamFromDatabase();

}
Course::~Course()
{
    delete Midterm_Exam;
    delete Final_Exam;
}
void Course::loadTeacherRole(const QList<Teacher *>&  Teachers)
{
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    qDebug() << "Preparing query: 'SELECT TeacherID, Role, Groups FROM TeacherRoles WHERE Courseid = :id'";
    query.prepare("SELECT TeacherID, Role, `Group` FROM TeacherRoles WHERE Courseid = :id");
    query.bindValue(":id", getID());

    if (!query.exec()) {
        qWarning() << "Failed to query TeacherRoles:" << query.lastError().text();
        return;
    }

    while (query.next())
    {
        int teacherid = query.value(0).toInt();
        QString teacherrole = query.value(1).toString();
        int Group = query.value(2).toInt();
        for (auto teacher : Teachers)
        {
            if (teacher == nullptr) {
                qWarning() << "Null teacher pointer in Teachers list.";
                continue;
            }

            if (teacher->getID() == teacherid)
            {
                if (teacherrole == "Lecture" && this->hasLecture())
                {
                    addLectureTeacher(Group, teacher);
                    qDebug() << "*                * * ** * *** *******************************************************Lecture teacher " << teacher->getName() << " has been added to the Course " << this->Course_name<<"\n **************";
                }
                else if (teacherrole == "Lab" && this->hasLab())
                {
                    addLabTeacher(Group, teacher);
                    qDebug() << "* * *****************************                    ************************************************Lab teacher " << teacher->getName() << " has been added to the Course " << this->Course_name<<"\n **************";
                }
                else if (teacherrole == "Tutorial" && this->hasTutorial())
                {
                    addTutorialTeacher(Group, teacher);
                    qDebug() << "**********************  **************************               ********************                ******Tutorial teacher " << teacher->getName() << " has been added to the Course " << this->Course_name<<"\n **************";
                }
            }
        }
    }
    qDebug() << "SELECT TeacherID FROM CourseTeachers WHERE CourseID = :id";
    query.prepare("SELECT TeacherID FROM CourseTeachers WHERE CourseID = :id");
    query.bindValue(":id", getID());

    if (!query.exec())
    {
        qWarning() << "Failed to query CourseTeachers:" << query.lastError().text();
        return;
    }
    while (query.next())
    {
        int teacherid = query.value(0).toInt();
        for (auto teacher : Teachers)
        {
            if (teacher == nullptr) {
                qWarning() << "Null teacher pointer in Teachers list.";
                continue;
            }

            if (teacher->getID() == teacherid)
            {
                CourseTeachers.append(teacher);
            }
        }
    }
}


void Course::extractNumbers(const QString& str)
{
    for ( auto boolean : str.split(""))
    {
         // here since it's a boolean if the int is 1 then it's true, else if the int is 0 then it's false
        if ( boolean == "0")
        hasSession.append(false);
        else if( boolean == "1")
        hasSession.append(true);
    }
    qDebug()<<"the sessions are as follows : "<<hasLecture()<<" "<<hasLab()<< " "<<hasTutorial();
}


//these are helper functions to lad attendance

void Course::addTutorialTeacherAttendance(Teacher* teacher, int group)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }
    QSqlQuery query(*Sqlbase);
    QVector<bool> attendance;
    query.prepare("SELECT state FROM TeacherAttendance WHERE TeacherID = :teacher AND sessiontype = 'Tutorial' AND CourseID = :courseid AND `Group` = :group");
    query.bindValue(":teacher", teacher->getID());
    query.bindValue(":courseid",this->getID());
    query.bindValue(":group",group);

    if (!query.exec()) {
        qWarning() << "Failed to query Tutorial Teachers Attendance:" << query.lastError().text();
        return;
    }
    if (query.next())
    {
        QString state = query.value(0).toString();
        QString_To_Attendance(state,attendance);
        teacher->addTutorialAttendance(group,attendance);

        qDebug()<<"teacher : "<<teacher->getName()<<"'s Attendance In tutorial for Course : "<<this->getCourseName()<<" and Group : "<<group<<" has been loaded";
        for (auto attend : teacher->getTutorialAttendance())
            qDebug()<<attend;

    }
}
void Course::addLectureTeacherAttendance(Teacher* teacher, int group)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }
    QSqlQuery query(*Sqlbase);
    QVector<bool> attendance;
    query.prepare("SELECT state FROM TeacherAttendance WHERE TeacherID = :teacher AND sessiontype = :session AND CourseID = :courseid AND `Group` = :group");
    query.bindValue(":teacher", teacher->getID());
    query.bindValue(":courseid",this->getID());
    query.bindValue(":group",group);
    query.bindValue(":session","Lecture");
    if (!query.exec()) {
        qWarning() << "Failed to query Lecture Teachers Attendance:" << query.lastError().text();
        return;
    }
    if (query.next())
    {
        QString state = query.value(0).toString();
        QString_To_Attendance(state,attendance);
        teacher->addLectureAttendance(group,attendance);

        qDebug()<<"teacher : "<<teacher->getName()<<"'s Attendance In Lecture for Course : "<<this->getCourseName()<<" and Group : "<<group<<" has been loaded";
        for (auto attend : teacher->getLectureAttendance())
            qDebug()<<attend;

    }
}
void Course::addLabTeacherAttendance(Teacher* teacher, int group)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }
    QSqlQuery query(*Sqlbase);
    QVector<bool> attendance;
    query.prepare("SELECT state FROM TeacherAttendance WHERE TeacherID = :teacher AND sessiontype = 'Lab' AND CourseID = :courseid AND `Group` = :group");
    query.bindValue(":teacher", teacher->getID());
    query.bindValue(":courseid",getID());
    query.bindValue(":group",group);

    if (!query.exec()) {
        qWarning() << "Failed to query Lab Teachers Attendance:" << query.lastError().text();
        return;
    }
    if (query.next())
    {
        QString state = query.value(0).toString();
        QString_To_Attendance(state,attendance);
        teacher->addLabAttendance(group,attendance);

        qDebug()<<"teacher : "<<teacher->getName()<<"'s Attendance In Lab for Course : "<<getCourseName()<<" and Group : "<<group<<" has been loaded";
        for (auto attend : teacher->getLabAttendance())
            qDebug()<<attend;
    }
}



// helper function which is not a member
void QString_To_Attendance(QString AttendanceString, QVector<bool>& Attendance)
{
    QStringList bitList =AttendanceString.split("");
    for ( auto boolean : bitList)
    {
        if ( boolean == "0")
            Attendance.append(false);
        else if( boolean == "1")
            Attendance.append(true);
    }
}
//


// Getters
int Course::getID() const {
    return ID;
}

QString Course::getCourseName() const {
    return Course_name;
}

QMap<int, Teacher*> Course::getLabTeachers() const {
    return Lab_teachers;
}

QMap<int, Teacher*> Course::getTutorialTeachers() const {
    return Tutorial_teachers;
}
QMap<int, Teacher*> Course::getLectureTeachers() const{
    return Lecture_teachers;
}

int Course::getyear()const
{
    return year;
}

QList<Teacher*> Course::getCourseTeacher() const
{
    return this->CourseTeachers;
}

bool Course::UpdateLLTinDatabase()
{
    // Construct the LLT state string
    QString state;
    state += hasLecture() ? "1" : "0";
    state += hasLab() ? "1" : "0";
    state += hasTutorial() ? "1" : "0";

    // Check for valid database pointer and open status
    if (!Sqlbase) {
        qWarning() << "Database pointer is nullptr. Cannot update teacher.";
        return false;
    }

    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open.";
        return false;
    }

    // Prepare and execute the query
    QSqlQuery query(*Sqlbase);
    query.prepare("UPDATE Courses SET Lecture_Lab_Tutorial = :llt WHERE CourseID = :id");
    query.bindValue(":id", getID()); // Note the colon added here
    query.bindValue(":llt", state);

    // Debugging output
    qDebug() << "Executing query: UPDATE Courses SET Lecture_Lab_Tutorial = " << state << " WHERE CourseID = " << getID();

    // Execute the query and check for errors
    if (!query.exec()) {
        qWarning() << "Failed to update LLT:" << query.lastError().text();
        return false;
    }

    // Success
    return true;
}
void Course::loadExamFromDatabase()
{
    if (!Sqlbase) {
        qWarning() << "Database pointer is nullptr. Cannot update teacher.";
        return ;
    }

    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open.";
        return ;
    }

    // Prepare and execute the query
    QSqlQuery query(*Sqlbase);

    query.prepare("SELECT MidtermDay, FinalDay, `Midterm Time Slot`, `Final Time Slot` FROM ExamSchedule WHERE CourseID = :courseid");
    query.bindValue(":courseid", getID());

    if (!query.exec()) {
        qWarning() << "Failed to query the Exam Details from the Database:" << query.lastError().text();
        return;
    }
    if (query.next()) {
        // Process Midterm Exam Data
        QString MidtermDay = query.value(0).toString();
        qDebug()<<"Midterm Day is : "<<MidtermDay;
        QDate midtermDate = QDate::fromString(MidtermDay, "dd/MM/yyyy");
        qDebug()<<" now it is Before Validation : "<<midtermDate.day()<< "/"<<midtermDate.month()<<"/"<<midtermDate.year();
        if (!midtermDate.isValid()) {
            midtermDate.setDate(2024, 1, 1);
        }
        qDebug()<<" now it is After Validation : "<<midtermDate.day()<< "/"<<midtermDate.month()<<"/"<<midtermDate.year();
        QString MidtermTimeSlot = query.value(2).toString();
        QTime midtermStartTime(11, 0, 0, 0);
        QTime midtermEndTime(13, 0, 0, 0);

        if (!MidtermTimeSlot.isEmpty())
        {
            QStringList midtermTimeParts = MidtermTimeSlot.split("-");
            if (midtermTimeParts.size() == 2)
            {
                midtermStartTime = QTime::fromString(midtermTimeParts[0], "HH:mm");
                midtermEndTime = QTime::fromString(midtermTimeParts[1], "HH:mm");

                if (!midtermStartTime.isValid()) {
                    midtermStartTime.setHMS(11, 0, 0, 0);
                }
                if (!midtermEndTime.isValid()) {
                    midtermEndTime.setHMS(13, 0, 0, 0);
                }
            }
        }
        qDebug()<<" Midterm Start Time : "<<midtermStartTime.hour()<<":"<<midtermStartTime.minute();
        qDebug()<<" Midterm End Time : "<<midtermEndTime.hour()<<":"<<midtermEndTime.minute();
        Midterm_Exam = new Exam(midtermDate, midtermStartTime, midtermEndTime);

        // Process Final Exam Data
        QString FinalDay = query.value(1).toString();
        QDate finalDate = QDate::fromString(FinalDay, "dd/MM/yyyy");
        if (!finalDate.isValid()) {
            finalDate.setDate(2024, 1, 1);
        }

        QString FinalTimeSlot = query.value(3).toString();
        QTime finalStartTime(11, 0, 0, 0);
        QTime finalEndTime(13, 0, 0, 0);

        if (!FinalTimeSlot.isEmpty())
        {
            QStringList finalTimeParts = FinalTimeSlot.split("-");
            if (finalTimeParts.size() == 2)
            {
                finalStartTime = QTime::fromString(finalTimeParts[0], "HH:mm");
                finalEndTime = QTime::fromString(finalTimeParts[1], "HH:mm");

                if (!finalStartTime.isValid())
                {
                    finalStartTime.setHMS(11, 0, 0, 0);
                }
                if (!finalEndTime.isValid()) {
                    finalEndTime.setHMS(13, 0, 0, 0);
                }
            }
        }

        Final_Exam = new Exam(finalDate, finalStartTime, finalEndTime);
    }

}
void Course::WriteExamsToDatabase()
{
    if (!Sqlbase) {
        qWarning() << "Database pointer is nullptr. Cannot update teacher.";
        return ;
    }

    if (!Sqlbase->open()) {
        qWarning() << "Database is not open.";
        return ;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open.";
        return ;
    }

    // Prepare and execute the query
    QSqlQuery query(*Sqlbase);

    query.prepare("UPDATE ExamSchedule SET MidtermDay = :midday, FinalDay = :finday, `Midterm Time Slot` = :midslot, `Final Time Slot` = :finslot WHERE CourseID = :courseid");
    query.bindValue(":courseid",getID());
    QString MidtermDay = this->Midterm_Exam->getExamDay().toString("dd/MM/yyyy");
    query.bindValue(":midday",MidtermDay);
    QString FinalDay = this->Final_Exam->getExamDay().toString("dd/MM/yyyy");
    query.bindValue(":finday",FinalDay);
    QString Midterm_Exam_Time = this->Midterm_Exam->getStartTime().toString("HH:mm") + "-" + this->Midterm_Exam->getEndTime().toString("HH:mm");
    query.bindValue(":midslot",Midterm_Exam_Time);
    QString Final_Exam_Time = this->Final_Exam->getStartTime().toString("HH:mm") + "-" + this->Final_Exam->getEndTime().toString("HH:mm");
    query.bindValue(":finslot",Final_Exam_Time);

    if (!query.exec()) {
        qWarning() << "Failed to save the Exam Details to the Database:" << query.lastError().text();
        return;
    }
}

void Course::ADDCourseTeacherToList(Teacher* teacher)
{
    this->CourseTeachers.append(teacher);
}
void Course::removeCourseTeacherFromList(Teacher* teacher)
{
    this->CourseTeachers.removeOne(teacher);
}

// Setters
void Course::setID(int id) {
    this->ID = id;
}

void Course::setCourseName(const QString &courseName) {
    Course_name = courseName;
}
void Course::setCoef(int coef)
{
    this->coefficient = coef;
}

void Course::sethasLecture(bool lecture)
{
    hasSession[0] = lecture;
}

void Course::sethasLab(bool lab)
{
    hasSession[1] = lab;
}

void Course::sethasTutorial(bool tutorial)
{
    hasSession[2] = tutorial;
}

void Course::removeLectureTeachers()
{
    this->Lecture_teachers.clear();
}
void Course::removeLabTeachers()
{
    this->Lab_teachers.clear();
}
void Course::removeTutorialTeachers()
{
    this->Tutorial_teachers.clear();
}

void Course::removeteacherFromCourse(Teacher* teacher)
{

    this->removeCourseTeacherFromList(teacher);
    QMap<int, Teacher*>::iterator it = Lecture_teachers.begin();
    while (it != this->Lecture_teachers.end()) {

        if (it.value() == teacher)
        {
            it = this->Lecture_teachers.erase(it);
        } else {
            ++it;
        }
    }
    it = Tutorial_teachers.begin();
    while (it != this->Tutorial_teachers.end()) {

        if (it.value() == teacher)
        {
            it = this->Tutorial_teachers.erase(it);
        } else {
            ++it;
        }
    }
    it = Lab_teachers.begin();
    while (it != this->Lab_teachers.end()) {

        if (it.value() == teacher)
        {
            it = this->Lab_teachers.erase(it);
        } else {
            ++it;
        }
    }
}

// Methods to manipulate Lab_teachers and Tutorial_teachers
void Course::addLabTeacher(int groupID,Teacher *teacher) {
    Lab_teachers.insert(groupID, teacher);


}

void Course::addTutorialTeacher(int groupID,  Teacher *teacher) {
    Tutorial_teachers.insert(groupID, teacher);


}

void Course::addLectureTeacher(int groupID, Teacher *teacher)
{
    Lecture_teachers.insert(groupID, teacher);
}

void Course::removeLabTeacher(int groupID)
{
    Lab_teachers.remove(groupID);

}

void Course::removeTutorialTeacher(int groupID)
{
    Tutorial_teachers.remove(groupID);

}


void Course::removeLectureTeacher(int groupID)
{
    Lecture_teachers.remove(groupID);
}


void Course::addTutorialTeachertoDatabase(int groupID,  Teacher *teacher)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    // Delete from Student Schedules table
    query.prepare("INSERT INTO TeacherRoles (Courseid, TeacherID, `Role`, `Group`) VALUES (:courseid, :teacherid, :role, :group)");
    query.bindValue(":courseid", QString::number(this->getID()));
    query.bindValue(":teacherid", QString::number(teacher->getID()));
    query.bindValue(":role", "Tutorial");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to insert Tutorial Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }


    query.prepare("INSERT INTO TeacherAttendance (CourseID, TeacherID, sessiontype, `Group`, state) VALUES (:courseid, :teacherid, :role, :group, :StatE)");
    query.bindValue(":courseid", QString::number(this->getID()));
    query.bindValue(":teacherid", QString::number(teacher->getID()));
    query.bindValue(":role", "Tutorial");
    query.bindValue(":group", groupID);

    QString state = "";
    for( int i = 0; i < this->getNumberofTutorialSessions(); i++)
    {
        state+="0";
    }
    teacher->setTutorialAttendanceString(state,groupID);
    query.bindValue(":StatE",state);

    if (!query.exec()) {
        qWarning() << "Failed to insert Tutorial Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }


    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return ;
    }
}

void Course::addLabTeachertoDatabase(int groupID,  Teacher *teacher)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    // Delete from Student Schedules table
    query.prepare("INSERT INTO TeacherRoles (Courseid, TeacherID, `Role`, `Group`) VALUES (:courseid, :teacherid, :role, :group)");
    query.bindValue(":courseid", QString::number(this->getID()));
    query.bindValue(":teacherid", QString::number(teacher->getID()));
    query.bindValue(":role", "Lab");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to insert Lab Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    query.prepare("INSERT INTO TeacherAttendance (CourseID, TeacherID, sessiontype, `Group`, state) VALUES (:courseid, :teacherid, :role, :group, :StatE)");
    query.bindValue(":courseid", QString::number(this->getID()));
    query.bindValue(":teacherid", QString::number(teacher->getID()));
    query.bindValue(":role", "Lab");
    query.bindValue(":group", groupID);

    QString state = "";
    for( int i = 0; i < this->getNumberofLabSessions();i++)
    {
        state+="0";
    }
    teacher->setLabAttendanceString(state,groupID);
    query.bindValue(":StatE",state);

    if (!query.exec()) {
        qWarning() << "Failed to insert Lab Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return ;
    }
}

void Course::addLectureTeachertoDatabase(int groupID,  Teacher *teacher)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    // Delete from Student Schedules table
    query.prepare("INSERT INTO TeacherRoles (Courseid, TeacherID, `Role`, `Group`) VALUES (:courseid, :teacherid, :role, :group)");
    query.bindValue(":courseid", QString::number(this->getID()));
    query.bindValue(":teacherid", QString::number(teacher->getID()));
    query.bindValue(":role", "Lecture");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to insert Lecture Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }


///////////
    query.prepare("INSERT INTO TeacherAttendance (CourseID, TeacherID, sessiontype, `Group`, state) VALUES (:courseid, :teacherid, :role, :group, :StatE)");
    query.bindValue(":courseid", QString::number(this->getID()));
    query.bindValue(":teacherid", QString::number(teacher->getID()));
    query.bindValue(":role", "Lecture");
    query.bindValue(":group", groupID);

    QString state = "";
    for( int i = 0; i < this->getNumberofLectureSessions();i++)
    {
        state+="0";
    }
    teacher->setLectureAttendanceString(state,groupID);
    query.bindValue(":StatE",state);

    if (!query.exec()) {
        qWarning() << "Failed to insert Lecture Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

//////////////
    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return ;
    }
}

void Course::removeLabTeacherFromDatabase(int groupID)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    // Delete from Student Schedules table
    query.prepare("DELETE FROM TeacherRoles WHERE Courseid = :id AND `Role` = :role AND `Group` = :group");
    query.bindValue(":id", QString::number(this->getID()));
    query.bindValue(":role", "Lab");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to delete Lab Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    query.prepare("DELETE FROM TeacherAttendance WHERE CourseID = :id AND sessiontype = :role AND `Group` = :group");
    query.bindValue(":id", QString::number(this->getID()));
    query.bindValue(":role", "Lab");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherAttendance:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }


    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return ;
    }
}

void Course::removeTutorialTeacherFromDatabase(int groupID)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    // Delete from Student Schedules table
    query.prepare("DELETE FROM TeacherRoles WHERE Courseid = :id AND Role = :role AND `Group` = :group");
    query.bindValue(":id", QString::number(this->getID()));
    query.bindValue(":role", "Tutorial");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to delete Tutorial Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }
    query.prepare("DELETE FROM TeacherAttendance WHERE CourseID = :id AND sessiontype = :role AND `Group` = :group");
    query.bindValue(":id", QString::number(this->getID()));
    query.bindValue(":role", "Tutorial");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherAttendance:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return ;
    }
}

void Course::removeLectureTeacherFromDatabase(int groupID)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    query.prepare("DELETE FROM TeacherRoles WHERE Courseid = :id AND `Role` = :role AND `Group` = :group");
    query.bindValue(":id", QString::number(this->getID()));
    query.bindValue(":role", "Lecture");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherRoles:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    query.prepare("DELETE FROM TeacherAttendance WHERE CourseID = :id AND sessiontype = :role AND `Group` = :group");
    query.bindValue(":id", QString::number(this->getID()));
    query.bindValue(":role", "Lecture");
    query.bindValue(":group", groupID);

    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherAttendance:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return ;
    }
}


void Course::removeCourseTeacherFromDatabase(Teacher* teacher)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction())
    {
        qWarning() << "Failed to start transaction";
        return ;
    }

    query.prepare("DELETE FROM CourseTeachers WHERE CourseID = :courseid AND TeacherID = :teacherid");
    query.bindValue(":courseid", QString::number(this->getID()));
    query.bindValue(":teacherid", QString::number(teacher->getID()));

    if (!query.exec()) {
        qWarning() << "Failed to delete from CourseTeachers:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    query.prepare("DELETE FROM TeacherAttendance WHERE TeacherID = :id AND CourseID = :courseid");
    query.bindValue(":id", QString::number(teacher->getID()));
    query.bindValue(":courseid", QString::number(this->getID()));
    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherAttendance:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    query.prepare("DELETE FROM TeacherRoles WHERE TeacherID = :id AND Courseid = :courseid");
    query.bindValue(":id", QString::number(teacher->getID()));
    query.bindValue(":courseid", QString::number(this->getID()));
    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherRoles:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return ;
    }

}
void Course::AddCourseTeacherToDatabase(Teacher* teacher)
{
    if (!Sqlbase->open())
    {
        qWarning() << "Database is not open!";
        return ;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction())
    {
        qWarning() << "Failed to start transaction";
        return ;
    }


    query.prepare("INSERT INTO CourseTeachers (CourseID, teacherID) VALUES (:courseid, :teacherid)");
    query.bindValue(":courseid", QString::number(this->getID()));
    query.bindValue(":teacherid", QString::number(teacher->getID()));

    if (!query.exec())
    {
        qWarning() << "Failed to insert Course Teacher:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return ;
    }
}


int Course::getCoef()
{
    return this->coefficient;
}

int Course::getsemester()
{
    return semester;
}

int Course::getNumberofLectureSessions()
{
    return Number_Of_Lecture_Sessions;
}

int Course::getNumberofLabSessions()
{
    return Number_Of_Lab_Sessions;
}

int Course::getNumberofTutorialSessions()
{
    return Number_Of_Tutorial_Sessions;
}

bool Course::hasLecture()
{
    return hasSession[0];
}

bool Course::hasLab()
{
    return hasSession[1];
}

bool Course::hasTutorial()
{
    return hasSession[2];
}



void Course::setSemester(int sem )
{
    semester = sem;
}

void Course::setNumberofLectureSessions( int num )
{
    Number_Of_Lecture_Sessions = num;
}

void Course::setNumberofLabSessions( int num )
{
    Number_Of_Lab_Sessions = num;
}

void Course::setNumberofTutorialSessions( int num )
{
    Number_Of_Tutorial_Sessions = num;
}



bool Course::UpdateCourseinDatabase()
{
    if (!Sqlbase ) {
        qWarning() << "pointer is nullptr. Cannot update Course.";
        return false;
    }
    if (!Sqlbase->open()){
        qWarning()<<"database is not open";
        return false;
    }
    QSqlQuery query(*Sqlbase);
    query.prepare("UPDATE Courses SET CourseName = :name, Year = :year, Semester = :semester, Coefficient = :coef, `number of Lecture sessions` = :lecture, `number of Lab sessions` = :lab, `number of Tutorial sessions` = :tutorial WHERE CourseID = :id");
    query.bindValue(":name", this->getCourseName());
    query.bindValue(":year",this->getyear());
    query.bindValue(":semester",this->getsemester() );
    query.bindValue(":coef", this->getCoef());
    query.bindValue(":lecture",this->getNumberofLectureSessions());
    query.bindValue(":lab", this->getNumberofLabSessions() );
    query.bindValue(":tutorial", this->getNumberofTutorialSessions());
    query.bindValue(":id", this->getID());
    if (!query.exec()) {
        qDebug() << "Failed to update Course in the Course:" << query.lastError().text();
    } else {
        qDebug() << "Course updated in the database successfully.";
    }
    // add transactions here, and add the updater for examSchedule Database
    return true;

}


Exam* Course::getMidterExam()
{
    return this->Midterm_Exam;
}

Exam* Course::getFinalExam()
{
    return this->Final_Exam;
}

void Course::setFinalExam(Exam* exam)
{
    delete Final_Exam;
    Final_Exam = exam;
}

void Course::setMidtermExam(Exam* exam)
{
    delete Midterm_Exam;
    Midterm_Exam = exam;
}




void Course::loadLectureStudentAttendance(Student * student)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return ;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    QSqlQuery query(*Sqlbase);
    QVector<bool> attendance;

    query.prepare("SELECT `State` FROM StudentAttendance WHERE `ID` = :id AND sessiontype = 'Lecture' AND CourseID = :courseid");
    qDebug()<<"SELECT `State` FROM StudentAttendance WHERE `ID` = :id AND sessiontype = 'Lecture' AND CourseID = :courseid";
    query.bindValue(":id",student->getID());
    query.bindValue(":courseid",getID());

    if (!query.exec()) {
        qWarning() << "Failed to query Student Attendance:" << query.lastError().text();
        return;
    }
    if (query.next()) {
        QString state = query.value(0).toString();
        QString_To_Attendance(state,attendance);
        student->setLectureAttendance(attendance);
        qDebug()<<"student with id : "<<student->getID()<<"'s Attendance In Lecture for Course with CourseID : "<<getID()<<" has been loaded";
        for (auto attend : attendance)
            qDebug()<<attend;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return ;
    }

}
void Course::loadTutorialStudentAttendance(Student * student)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return ;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    QSqlQuery query(*Sqlbase);
    QVector<bool> attendance;

    query.prepare("SELECT `State` FROM StudentAttendance WHERE `ID` = :id AND sessiontype = 'Tutorial' AND CourseID = :courseid");
    qDebug()<<"SELECT `State` FROM StudentAttendance WHERE `ID` = :id AND sessiontype = 'Tutorial' AND CourseID = :courseid";
    query.bindValue(":id",student->getID());
    query.bindValue(":courseid",getID());

    if (!query.exec()) {
        qWarning() << "Failed to query Student Attendance:" << query.lastError().text();
        return;
    }
    if (query.next()) {
        QString state = query.value(0).toString();
        QString_To_Attendance(state,attendance);
        student->setTutorialAttendance(attendance);
        qDebug()<<"student with id : "<<student->getID()<<"'s Attendance In Tutorial for Course with CourseID : "<<getID()<<" has been loaded";
        for (auto attend : attendance)
            qDebug()<<attend;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return ;
    }
}

void Course::loadLabStudentAttendance(Student * student)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return ;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    QSqlQuery query(*Sqlbase);
    QVector<bool> attendance;

    query.prepare("SELECT `State` FROM StudentAttendance WHERE `ID` = :id AND sessiontype = 'Lab' AND CourseID = :courseid");
    qDebug()<<"SELECT `State` FROM StudentAttendance WHERE `ID` = :id AND sessiontype = 'Lab' AND CourseID = :courseid";
    query.bindValue(":id",student->getID());
    query.bindValue(":courseid",getID());

    if (!query.exec()) {
        qWarning() << "Failed to query Student Attendance:" << query.lastError().text();
        return;
    }
    if (query.next()) {
        QString state = query.value(0).toString();
        QString_To_Attendance(state,attendance);
        student->setLabAttendance(attendance);
        qDebug()<<"student with id : "<<student->getID()<<"'s Attendance In Lab for Course with CourseID : "<<getID()<<" has been loaded";
        for (auto attend : attendance)
            qDebug()<<attend;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return ;
    }


}



bool Course::updateLectureTeachersAttendanceinDatabase()
{
    for (auto teacher : this->getLectureTeachers())
    {
        if( teacher->getdeletion_State() != true)
        {
            this->updateTeacherLectureAttendanceinDatabase(teacher);
        }
    }
    return true;
}

bool Course::updateTutorialTeachersAttendanceinDatabase()
{
    for (auto teacher : this->getTutorialTeachers())
    {
        if( teacher->getdeletion_State() != true)
        {
            this->updateTeacherTutorialAttendanceinDatabase(teacher);
        }
    }
    return true;
}

bool Course::updateLabTeachersAttendanceinDatabase()
{
    for (auto teacher : this->getLabTeachers())
    {
        if( teacher->getdeletion_State() != true)
        {
            this->updateTeacherLabAttendanceinDatabase(teacher);
        }
    }
    return true;
}

//these are for one Teacher
bool Course::updateTeacherLectureAttendanceinDatabase(Teacher* teacher)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }
    QSqlQuery query(*Sqlbase);
    QString state;
    QMap<int,QVector<bool>> Attendance = teacher->getLectureAttendance();
    for(auto group : Attendance.keys())
    {
        for ( auto bit : Attendance[group])
        {
            if( bit == true)
            {
                state+="1";
            }
            else if(bit == false)
            {
                state+="0";
            }
        }
        query.prepare("UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session AND `Group` = :group");
        query.bindValue(":id", getID());
        query.bindValue(":session","Lecture");
        query.bindValue(":State",state);
        query.bindValue(":group",group);
        if (!query.exec())
        {
            qWarning() << " failed to UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session" << query.lastError().text();
            Sqlbase->rollback();
            return false;
        }
        state="";
    }
    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    return true;
}

bool Course::updateTeacherTutorialAttendanceinDatabase(Teacher* teacher)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }
    QSqlQuery query(*Sqlbase);
    QString state;
    QMap<int,QVector<bool>> Attendance = teacher->getTutorialAttendance();
    for(auto group : Attendance.keys())
    {
        for ( auto bit : Attendance[group])
        {
            if( bit == true)
            {
                state+="1";
            }
            else if(bit == false)
            {
                state+="0";
            }
        }
        query.prepare("UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session AND `Group` = :group");
        query.bindValue(":id", getID());
        query.bindValue(":session","Tutorial");
        query.bindValue(":State",state);
        query.bindValue(":group",group);
        if (!query.exec())
        {
            qWarning() << " failed to UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session" << query.lastError().text();
            Sqlbase->rollback();
            return false;
        }
        state="";
    }
    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    return true;
}

bool Course::updateTeacherLabAttendanceinDatabase(Teacher* teacher)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }
    QSqlQuery query(*Sqlbase);
    QString state;
    QMap<int,QVector<bool>> Attendance = teacher->getLabAttendance();
    for(auto group : Attendance.keys())
    {
        for ( auto bit : Attendance[group])
        {
            if( bit == true)
            {
                state+="1";
            }
            else if(bit == false)
            {
                state+="0";
            }
        }
        query.prepare("UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session AND `Group` = :group");
        query.bindValue(":id", getID());
        query.bindValue(":session","Lab");
        query.bindValue(":State",state);
        query.bindValue(":group",group);
        if (!query.exec())
        {
            qWarning() << " failed to UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session" << query.lastError().text();
            Sqlbase->rollback();
            return false;
        }
        state="";
    }
    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    return true;
}

// upCourse::date the database Attendance for Students
bool Course::updateLectureStudentsAttendanceinDatabase(Group* group)
{
    for (auto student : group->getGroupStudents() )
    {
        if( student->getdeletion_State() != true)
        {
            this->updateStudentLectureAttendanceinDatabase(student);
        }
    }
    return true;
}

bool Course::updateTutorialStudentsAttendanceinDatabase(Group* group)
{
    for (auto student : group->getGroupStudents() )
    {
        if( student->getdeletion_State() != true)
        {
            this->updateStudentTutorialAttendanceinDatabase(student);
        }
    }
    return true;
}

bool Course::updateLabStudentsAttendanceinDatabase(Group * group)
{
    for (auto student : group->getGroupStudents() )
    {
        if( student->getdeletion_State() != true)
        {
            this->updateStudentLabAttendanceinDatabase(student);
        }
    }
    return true;
}

//these are for one student
bool Course::updateStudentLectureAttendanceinDatabase(Student* student)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    QSqlQuery query(*Sqlbase);
    QString state;
    QVector<bool> Attendance = student->getLectureAttendance() ;
    for(auto bit : Attendance)
    {
        if( bit == true)
        {
            state+="1";
        }
        else if(bit == false)
        {
            state+="0";
        }
    }
    query.prepare("UPDATE StudentAttendance SET State = :state WHERE CourseID = :courseid AND sessiontype = :session AND ID = :id");
    query.bindValue(":courseid", getID());
    query.bindValue(":session","Lecture");
    query.bindValue(":state",state);
    query.bindValue(":id",student->getID());
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Lecture:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }
    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    return true;
}

bool Course::updateStudentTutorialAttendanceinDatabase(Student* student)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    QSqlQuery query(*Sqlbase);
    QString state;
    QVector<bool> Attendance = student->getTutorialAttendance() ;

    for(auto bit : Attendance)
    {
        if( bit == true)
        {
            state+="1";
        }
        else if(bit == false)
        {
            state+="0";
        }
    }

    query.prepare("UPDATE StudentAttendance SET State = :state WHERE CourseID = :courseid AND sessiontype = :session AND ID = :id");
    query.bindValue(":courseid", getID());
    query.bindValue(":session","Tutorial");
    query.bindValue(":state",state);
    query.bindValue(":id",student->getID());
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Tutorial:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }
    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    return true;
}

bool Course::updateStudentLabAttendanceinDatabase(Student *student)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    QSqlQuery query(*Sqlbase);
    QString state;
    QVector<bool> Attendance = student->getLabAttendance() ;
    for(auto bit : Attendance)
    {
        if( bit == true)
        {
            state+="1";
        }
        else if(bit == false)
        {
            state+="0";
        }
    }
    query.prepare("UPDATE StudentAttendance SET State = :state WHERE CourseID = :courseid AND sessiontype = :session AND ID = :id");
    query.bindValue(":courseid", getID());
    query.bindValue(":session","Lab");
    query.bindValue(":state",state);
    query.bindValue(":id",student->getID());
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Lab:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }
    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    return true;
}

void Course::loadGroupGradesFromDatabase(Group* group)
{

    for( auto student : group->getGroupStudents())
    {
        this->loadStudentGradesFromDatabase(student);
    }
}

void Course::loadStudentGradesFromDatabase(Student* student)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return ;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    QSqlQuery query(*Sqlbase);
    query.prepare("SELECT Midterm, Tutorial_Lab_Average, `Final Exam`, Average FROM GradesTable WHERE CourseID = :courseid AND StudentID = :studentid");
    query.bindValue(":courseid",this->getID());
    query.bindValue(":studentid",student->getID());

    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Lab:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if( query.next())
    {
        float midterm = query.value(0).toFloat();
        float T_L_Avg = query.value(1).toFloat();
        float Final_Exam = query.value(2).toFloat();
        float Avg = query.value(3).toFloat();
        student->setMidterm(midterm);
        student->setT_L_Avg(T_L_Avg);
        student->setFinal_Exam(Final_Exam);
        student->setAvg(Avg);
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return ;
    }
}




void Course::WriteStudentGradesToDatabase(Student *student) {
    qDebug() << "NOW I AM HERE !!!!!!" << student->getName();

    if (!student) {
        qWarning() << "Student pointer is null!";
        return;
    }

    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction:" << Sqlbase->lastError().text();
        return;
    }

    QSqlQuery query(*Sqlbase);
    query.prepare("UPDATE GradesTable SET Midterm = :midterm, Tutorial_Lab_Average = :TL, `Final Exam` = :final, Average = :avg WHERE CourseID = :courseid AND StudentID = :studentid");
    query.bindValue(":courseid", this->getID());
    query.bindValue(":studentid", student->getID());
    query.bindValue(":midterm", student->getGrades()[0]);
    query.bindValue(":TL", student->getGrades()[1]);
    query.bindValue(":final", student->getGrades()[2]);
    query.bindValue(":avg", student->getGrades()[3]);

    if (!query.exec()) {
        qWarning() << "Failed to update GradesTable:" << query.lastError().text();
        Sqlbase->rollback();
        return;
    }

    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return;
    }

    qDebug() << "Grades for student" << student->getName() << "updated successfully.";
}





//  ----------------------------- this section is for the implementation of the Year class -------------------------------------------//





Year::Year(QSqlDatabase* sqlbase,QList<Teacher*>* Teachers,int year)
    : year(year)
    ,Sqlbase(sqlbase)
    ,Number_of_groups(0)
    ,NumberOfCourses(0)
{
    if (sqlbase != nullptr)
    {
    if (!sqlbase->isOpen() ) {
        qWarning() << "Database is not open!";
        return;
    }
    loadGroupsFromDatabase();
    loadCoursesFromDatabase(Teachers);
    }
    else         qWarning() << "sqlbase is nullptr ";
}

int Year::getYear() const {
    return year;
}
int Year::getnumberofgroups() const{
    return Number_of_groups;
}

QList<Group*> Year::getGroups() const {
    return groups;
}

QList<Course*> Year::getCourses() const {
    return courses;
}

void Year::addGroup(Group* group) {
    groups.append(group);
    Number_of_groups++;
}

void Year::removeGroup(Group* group) {
    groups.removeOne(group);
    Number_of_groups--;
}

void Year::addCourse(Course* course) {
    courses.append(course);
    NumberOfCourses++;
}

void Year::removeCourse(Course* course) {
    courses.removeOne(course);
    qDebug()<<" The course is removed completely here from everywhere ";
    NumberOfCourses--;
}


void Year::loadGroupsFromDatabase()
{
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    qDebug() << "Preparing query: 'SELECT `Group` FROM `Groups` WHERE Year = :year'";
    query.prepare("SELECT `Group` FROM `Groups` WHERE Year = :year");
    query.bindValue(":year", year);

    if (!query.exec()) {
        qWarning() << "Failed to query groups:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int groupNumber = query.value(0).toInt();
        Group* group = new Group(groupNumber, year, Sqlbase);
        groups.append(group);
        qDebug() << "Group number" << groupNumber << "has been created";
        ++Number_of_groups;
    }


}

void Year::loadCoursesFromDatabase(QList<Teacher*>* Teachers){
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    qDebug() << "SELECT CourseName, CourseID, Coefficient, `number of tutorial sessions`, `number of Lab sessions`, `number of Lecture sessions`, Lecture_Lab_tutorial, Semester FROM Courses WHERE Year = :year";
    query.prepare("SELECT CourseName, CourseID, Coefficient, `number of tutorial sessions`, `number of Lab sessions`, `number of Lecture sessions`, Lecture_Lab_tutorial, Semester FROM Courses WHERE Year = :year");
    query.bindValue(":year", year);

    if (!query.exec()) {
        qWarning() << "Failed to query Courses:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        int id = query.value(1).toInt();
        int coef = query.value(2).toInt();
        int numberoftutosessions = query.value(3).toInt();
        int numberoflabsessions = query.value(4).toInt();
        int numberoflecturesessions = query.value(5).toInt();
        QString hassessions = query.value(6).toString();
        int semester = query.value(7).toInt();

        Course* course = new Course(Sqlbase,id,name,coef,numberoftutosessions,numberoflabsessions,*Teachers,numberoflecturesessions,semester,hassessions, year);
        courses.append(course);
        qDebug() << "Course :" << course->getCourseName() << "has been created";
        ++NumberOfCourses;
    }
}


//  ----------------------------- this section is for the implementation of the School class -------------------------------------------//
School::School(QSqlDatabase* sqlbase) :
    Sqlbase(sqlbase)

{
    loadTeachersFromDatabase();
    years = new Year[5];
    for (int i = 0; i < numberOfYears; ++i)
    {
        years[i] = Year(sqlbase,&teachers,i+1); // Initialize each year with a specific year number

        for ( auto Group : years[i].getGroups())// initializes the removedStudentsQList
        {
            for ( auto student : Group->getGroupStudents())
            {
                if ( student->getdeletion_State())
                {
                    RemovedStudents.append(student);
                }
        }   }
    }
}

School::~School() {
    delete[] years;
}
void School::setaddedgroup(Group* group)
{
    AddedGroup=group;
}
Group* School::getaddedgroup()
{
    return AddedGroup;
}

Year* School::getYears() const
{
    return years;
}
int School::getNumberofyears()
{
    return numberOfYears;
}

void School::addCourse(int year, Course* course) {
    for (int i = 0; i < numberOfYears; ++i) {
        if (years[i].getYear() == year) {
            years[i].addCourse(course);
            return;
        }
    }
}


void School::removeCourse(int year, Course* course) {
    qDebug()<<" You are Here !!! 4 ";
    for (int i = 0; i < numberOfYears; ++i) {
        if (years[i].getYear() == year) {
            qDebug()<<" the name of the Course is : "<<course->getCourseName();
            years[i].removeCourse(course);
            return;
        }
    }
}
QList<Teacher*> School::getTeachers() const {
    return teachers;
}
QSqlDatabase* School::getdatabase()const
{
    return Sqlbase;
}
void School::addTeacher(Teacher* teacher) {
    teachers.append(teacher);
}

void School::removeTeacher(Teacher* teacher) {
    teachers.removeOne(teacher);
}

void School::loadTeachersFromDatabase() {
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    query.prepare("SELECT Name, TeacherID, `E-mail`, PhoneNumber, Password, DatabaseID, ImagePath, Gender, Deleted FROM Teachers");

    if (!query.exec()) {
        qWarning() << "Failed to query teachers:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        int id = query.value(1).toInt();
        QString email = query.value(2).toString();
        QString phonenumber = query.value(3).toString();
        QString password = query.value(4).toString();
        int Sqlbaseid = query.value(5).toInt();
        QString Imagepath = query.value(6).toString();
        QString gender = query.value(7).toString();
        bool Deleted = query.value(8).toBool();

        Teacher* teacher = new Teacher(Sqlbase, name, email, phonenumber, id,password,Sqlbaseid,Imagepath,gender,Deleted);
        teachers.append(teacher);
        qDebug() << "Loaded teacher:" << name << " " << id << " " << email << " " << phonenumber;
    }

}

Teacher* School::findTeacherByName(const QString& name) {
    qDebug() << "Finding teacher with name: " << name;

    auto teachers = getTeachers();
    for (const auto& teacher : teachers) {
        qDebug() << "Checking teacher: " << (teacher ? teacher->getName() : "nullptr");
        if (teacher && (teacher->getName()+" ("+QString::number(teacher->getID())+")") == name) {
            qDebug() << "Teacher found: " << teacher->getName();
            return teacher;
        }
    }

    qDebug() << "Teacher not found: " << name;
    return nullptr;
}

Student* School::findStudentByName(const QString& name)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return nullptr;
    }

    qDebug() << "Finding student with name:" << name;

    QStringList ID2 = name.split("(");
    if (ID2.size() != 2) {
        qWarning() << "Name format incorrect, expected 'Name (ID)':" << name;
        return nullptr;
    }

    QString idString = ID2[1].remove(")");
    bool ok;
    int id = idString.toInt(&ok);
    if (!ok) {
        qWarning() << "Failed to convert ID to integer:" << idString;
        return nullptr;
    }

    qDebug() << "Extracted ID:" << id;

    QSqlQuery query(*Sqlbase);
    query.prepare("SELECT Year, `Group` FROM Students WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Query for Year and Group info failed:" << query.lastError().text();
        return nullptr;
    }

    if (!query.next()) {
        qDebug() << "No such row for student ID:" << id;
        return nullptr;
    }

    int studentYear = query.value(0).toInt();
    int studentGroup = query.value(1).toInt();

    qDebug() << "Student found in year:" << studentYear << "group:" << studentGroup;

    auto groups = getYears()[studentYear - 1].getGroups();
    for (auto group : groups) {
        qDebug() << "Checking group:" << group->getGroupNumber();
        if (group->getGroupNumber() == studentGroup) {
            for (auto student : group->getGroupStudents()) {
                QString studentName = student->getName() + " (" + QString::number(student->getID()) + ")";
                qDebug() << "Comparing:" << studentName << "with" << name;
                if (studentName == name) {
                    qDebug() << "Found student:" << student->getName();
                    return student;
                }
            }
        }
    }
    qDebug()<<"there is no such group, you must've deleted it";
    for ( auto student : this->RemovedStudents)
    {
        if ( student ->getgroup() == studentGroup && student->getID() == id )
        {
            return student;
        }

    }

    qDebug() << "Student not found:" << name;
    return nullptr;
}
Group* School::findGroupByName(const QString & name )
{
    static QRegularExpression re("^Y[0-9][0-9]?G[0-9][0-9]?");// we will be using regular expressions to make comparing and checking if the QString is valid easier

    if (re.match(name).hasMatch())
    {
        QStringList filters = name.split("G");
        int groupnumber = filters[1].toInt();
        int year = filters[0].split("Y")[1].toInt();
        for ( auto group : years[year-1].getGroups())
        {
            if ( group->getGroupNumber() == groupnumber)
                return group;
        }
    }
    qDebug()<<"this group could not be found";
    return nullptr;
}



Course* School::findCoursebyName(const QString & name)
{
    for (int i = 0; i < 5; i++)
    {
        for(auto course : this->getYears()[i].getCourses())
        {
            if(course->getCourseName() == name)
                return course;
        }
    }

    qDebug()<<"this Course could not be found";
    return nullptr;
}



bool School::removeStudentDefinitively(Student* student) // this will handle removing the student utterly from the database and his respective records                      remember to remove him from the QMaps and all the other things
{
        if (!Sqlbase->open())
        {
            qWarning() << "Database is not open!";
            return false;
        }

        QSqlQuery query(*Sqlbase);

        // Begin a transaction to ensure all deletes are executed atomically
        if (!Sqlbase->transaction())
        {
            qWarning() << "Failed to start transaction:" << Sqlbase->lastError().text();
            return false;
        }

        // Delete from Students table
        query.prepare("DELETE FROM Students WHERE ID = :studentID");
        query.bindValue(":studentID", student->getID());
        if (!query.exec())
        {
            qWarning() << "Failed to delete from Students table:" << query.lastError().text();
            Sqlbase->rollback();
            return false;
        }

        // Delete from StudentsAttendance table
        query.prepare("DELETE FROM StudentAttendance WHERE ID = :studentID");
        query.bindValue(":studentID", student->getID());
        if (!query.exec())
        {
            qWarning() << "Failed to delete from StudentAttendance table:" << query.lastError().text();
            Sqlbase->rollback();
            return false;
        }

        // Delete from GradesTable
        query.prepare("DELETE FROM GradesTable WHERE StudentID = :studentID");
        query.bindValue(":studentID", student->getID());
        if (!query.exec())
        {
            qWarning() << "Failed to delete from GradesTable:" << query.lastError().text();
            Sqlbase->rollback();
            return false;
        }

        // Commit the transaction
        if (!Sqlbase->commit())
        {
            qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
            Sqlbase->rollback();
            return false;
        }

        qDebug() << "Successfully deleted student with ID:" << student->getID();

        // this will remove the pointer of the student from the group if the group still exists
        for(auto group : this->getYears()[student->getYear()-1].getGroups())
        {
            if ( group->getGroupNumber() == student->getgroup())
            {
                group->removeStudent(student);

                if ( student->getposition() == "Delegate" )
                {
                    group->setDelegate(nullptr);
                }
                else if ( student->getposition() == "Sub-Delegate")
                {
                    group->setSubDelegate(nullptr);
                }
            }
        }

        delete student;
        qDebug()<<" student delted definitively successfully ";
        return true;

}

bool School::removeTeacherDefinitively(Teacher* teacher)
{

    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    // Delete from Teachers table
    query.prepare("DELETE FROM Teachers WHERE TeacherID = :teacherID");
    query.bindValue(":teacherID", teacher->getID());
    if (!query.exec()) {
        qWarning() << "Failed to delete from Teachers:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    // Example for Courses table
    query.prepare("DELETE FROM TeacherRoles WHERE TeacherID = :teacherID");
    query.bindValue(":teacherID", teacher->getID());
    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherRoles:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    query.prepare("DELETE FROM TeacherAttendance WHERE TeacherID = :teacherID");
    query.bindValue(":teacherID", teacher->getID());
    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherAttendance:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    query.prepare("DELETE FROM TeacherSchedule WHERE ID = :teacherID");
    query.bindValue(":teacherID", teacher->getID());
    if (!query.exec()) {
        qWarning() << "Failed to delete teacher schedule:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    query.prepare("DELETE FROM CourseTeachers WHERE TeacherID = :id");
    query.bindValue(":id",QString::number(teacher->getID()) );
    if (!query.exec()) {
        qWarning() << "Failed to delete from CourseTeachers:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    // Commit the transaction
    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return false;
    }

    this->removeTeacher(teacher);
    for ( int i = 0; i < 5; i++)
    {
        for (auto course : this->getYears()[i].getCourses())
        {
            course->removeteacherFromCourse(teacher);
        }
    }
    delete teacher;
    return true;

}
void School::deletestudentsfromgroup(QList<Student*> Students)
{
    for ( auto student : Students)
    {
        if ( !student->getdeletion_State())
        {
            this->removeStudentDefinitively(student);
        }
    }
}

bool School::deleteGroupdefinitively(Group* group) // remember to delete the records of teachers from TeacherRoles table
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    // Delete from Student Schedules table
    query.prepare("DELETE FROM StudentSchedules WHERE Year = :year AND `Group` = :group");
    query.bindValue(":year", group->getyear());
    query.bindValue(":group",group->getGroupNumber());
    if (!query.exec()) {
        qWarning() << "Failed to delete schedules of Groups:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }


    query.prepare("DELETE FROM `Groups` WHERE Year = :year AND `Group` = :group");
    query.bindValue(":year", group->getyear());
    query.bindValue(":group",group->getGroupNumber());
    if (!query.exec()) {
        qWarning() << "Failed to delete from Groups:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }


    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return false;
    }
    this->getYears()[group->getyear()-1].removeGroup(group);
    return true;

}

void School::addremmovedstudent(Student* student)
{
    this->RemovedStudents.append(student);
}

void School::recoverRemovedstudent(Student* student)
{
    this->RemovedStudents.removeOne(student);
}

QList<Student*> School::getRemovedStudents()const
{
    return RemovedStudents;
}

bool School::groupExists(int year, int groupNumber)
{
    for ( auto group : this->getYears()[year-1].getGroups())
    {
        if ( group->getGroupNumber() == groupNumber )
            return true;
    }
    return false;
}

bool School::AddStudentTodatabase(Student* student)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }

    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);

    // Begin transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction:" << Sqlbase->lastError().text();
        return false;
    }

    // Prepare the SQL query to insert a new student
    query.prepare("INSERT INTO Students (Name, ID, Year, `Group`, `E-mail`, PhoneNumber, Position, Gender, Deleted) VALUES (:name, :id, :year, :group, :email, :phone, :position, :gender, 0)");
    query.bindValue(":name", student->getName());
    query.bindValue(":group", student->getgroup());
    query.bindValue(":year", student->getYear());
    query.bindValue(":id", student->getID());
    query.bindValue(":email", student->getEmail());
    query.bindValue(":phone", student->getPhone_Number());
    query.bindValue(":position", "Student"); // Bind the position as "Student"
    query.bindValue(":gender", student->getgender());

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Failed to insert student:" << query.lastError().text();
        Sqlbase->rollback(); // Rollback transaction on error
        return false;
    }


    query.prepare("SELECT DatabaseID FROM Students WHERE ID = :id");
    query.bindValue(":id", student->getID());
    if (!query.exec()) {
        qWarning() << "Failed to extract dbid:" << query.lastError().text();
        Sqlbase->rollback(); // Rollback transaction on error
        return false;
    }
    if (query.next()) {
        int dbid = query.value(0).toInt();
        student->setDatabaseID(dbid);
    } else {
        Sqlbase->rollback(); // Rollback transaction if no ID found
        return false;
    }


   // here we will insert in the StudentAttendance entries for the attendance at the lab, the Lecture and the Tutorial.
    QStringList sessions {"Lecture","Lab","Tutorial"};
    QString state = "";
    for ( auto course : this->getYears()[student->getYear()-1].getCourses())
    {
        query.prepare("INSERT INTO StudentAttendance (ID, CourseID, sessiontype, State) VALUES (:id, :courseid, :session, :state)");
        for ( auto session : sessions)
        {
            query.bindValue(":id",student->getID());
            query.bindValue(":courseid",course->getID());
            query.bindValue(":session",session);
            if( session == "Tutorial" )
            {
                for(int i = 0; i < course->getNumberofTutorialSessions(); i++)
                    state+="0";
            }
            else if( session == "Lecture" )
            {
                for(int i = 0; i < course->getNumberofLectureSessions(); i++)
                    state+="0";
            }
            else if( session == "Tutorial" )
            {
                for(int i = 0; i < course->getNumberofLabSessions(); i++)
                    state+="0";
            }
            query.bindValue(":state",state);


            if (!query.exec()) {
                qWarning() << "Failed to insert the Student Attendance:" << query.lastError().text();
                Sqlbase->rollback(); // Rollback transaction on error
                return false;
            }
        }
        query.prepare("INSERT INTO GradesTable (StudentID, CourseID, Midterm, Tutorial_Lab_Average, `Final Exam`, Average) VALUES (:id, :Courseid, :midterm, :TLAverage, :final, :average)");
        query.bindValue(":id",student->getID());
        query.bindValue(":Courseid",course->getID());
        query.bindValue(":midterm",10);
        query.bindValue(":TLAverage",10);
        query.bindValue(":final",10);
        query.bindValue(":average",10);

        if (!query.exec()) {
            qWarning() << "Failed to create the Grades entry for student : "<<student->getName() <<" ended up with the following error : " << query.lastError().text();
            Sqlbase->rollback(); // Rollback transaction on error
            return false;
        }
    }

    // Commit transaction
    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        return false;
    }
    Group* Studentgroup = findGroupByName("Y"+QString::number(student->getYear())+"G"+QString::number(student->getgroup()));
    Studentgroup->addStudent(student);
    return true;
}

bool School::AddTeacherTodatabase(Teacher* teacher)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);

    // Begin transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction:" << Sqlbase->lastError().text();
        return false;
    }

    // Prepare the SQL query to insert a new teacher
        query.prepare("INSERT INTO Teachers (Name, TeacherID, Password, `E-mail`, PhoneNumber, Gender, Deleted) VALUES (:name, :id, :password, :email, :phone, :gender, 0)");
        query.bindValue(":name", teacher->getName());
        query.bindValue(":password", teacher->getpassword());
        query.bindValue(":id", teacher->getID());
        query.bindValue(":email", teacher->getEmail());
        query.bindValue(":phone", teacher->getPhone_Number());
        query.bindValue(":gender", teacher->getgender());

        // Execute the query
        if (!query.exec()) {
            qWarning() << "Failed to insert teacher:" << query.lastError().text();
            Sqlbase->rollback(); // Rollback transaction on error
            return false;
        }

        query.prepare("SELECT DatabaseID FROM Teachers WHERE TeacherID = :id");
        query.bindValue(":id", teacher->getID());
        if (!query.exec()) {
            qWarning() << "Failed to extract dbid:" << query.lastError().text();
            Sqlbase->rollback(); // Rollback transaction on error
            return false;
        }
        if (query.next()) {
            int dbid = query.value(0).toInt();
            teacher->setDatabaseID(dbid);
        } else {
            Sqlbase->rollback(); // Rollback transaction if no ID found
            return false;
        }
    //inserting the teacher
        query.prepare("INSERT INTO TeacherSchedule (ID, ScheduleItems) VALUES (:id, :schedule) ");
        query.bindValue(":id",teacher->getID());
        query.bindValue(":schedule","");
        if (!query.exec()) {
            qWarning() << "Failed to insert Teacher Schedule Items:" << query.lastError().text();
            Sqlbase->rollback(); // Rollback transaction on error
            return false;
        }

    // Commit transaction
        if (!Sqlbase->commit())
        {
            qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
            return false;
        }
        addTeacher(teacher);

    return true;
}

bool School::AddCourseTodatabase(Course* course)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);

    // Begin transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction:" << Sqlbase->lastError().text();
        return false;
    }

    QString Lecture_Lab_Tutorial("");

    if( course->hasLecture())
    {
        Lecture_Lab_Tutorial+="1";
    }
    else
    {
        Lecture_Lab_Tutorial+="0";
    }



    if( course->hasLab())
    {
        Lecture_Lab_Tutorial+="1";
    }
    else
    {
        Lecture_Lab_Tutorial+="0";
    }


    if( course->hasTutorial())
    {
        Lecture_Lab_Tutorial+="1";
    }
    else
    {
        Lecture_Lab_Tutorial+="0";
    }

    if( course->getID() == -1)
    {
    query.prepare("INSERT INTO Courses (CourseName, Year, Semester, Coefficient, `number of Lecture sessions`, `number of tutorial sessions`, `number of Lab sessions`, `Lecture_Lab_Tutorial`) VALUES (:name, :year, :semester, :coef, :lecture, :tuto, :lab, :LLT)");
    }
    else
    {
        query.prepare("INSERT INTO Courses (CourseName, CourseID, Year, Semester, Coefficient, `number of Lecture sessions`, `number of tutorial sessions`, `number of Lab sessions`, `Lecture_Lab_Tutorial`) VALUES (:name, :id, :year, :semester, :coef, :lecture, :tuto, :lab, :LLT)");
        query.bindValue(":id",course->getID());
    }

    query.bindValue(":name",course->getCourseName()); // :lecture, :tuto, :lab
    query.bindValue(":year",course->getyear());
    query.bindValue(":semester",course->getsemester());
    query.bindValue(":coef",course->getCoef());
    query.bindValue(":lecture",course->getNumberofLectureSessions());
    query.bindValue(":tuto",course->getNumberofTutorialSessions());
    query.bindValue(":lab",course->getNumberofLabSessions());
    query.bindValue(":LLT",Lecture_Lab_Tutorial);
    // Execute the query
    if (!query.exec()) {
        qWarning() << "Failed to insert Course:" << query.lastError().text();
        Sqlbase->rollback(); // Rollback transaction on error
        return false;
    }

    if( course->getID() == -1)
    {
        query.prepare("SELECT CourseID FROM Courses WHERE CourseName = :name");
        query.bindValue(":name",course->getCourseName());
        if (!query.exec()) {
            qWarning() << "Failed to extract new ID" << query.lastError().text();
            Sqlbase->rollback(); // Rollback transaction on error
            return false;
        }
        if(query.next())
        {
            int newID = query.value(0).toInt();
            course->setID(newID);
        }
    }

    query.prepare("INSERT INTO ExamSchedule (CourseID, MidtermDay, FinalDay, `Midterm Time Slot`, `Final Time Slot`) VALUES (:id, '01/01/2024', '01/01/2024', '11:00-13:00', '11:00-13:00')");
    query.bindValue(":id",course->getID());

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Failed to insert Exam Schedule:" << query.lastError().text();
        Sqlbase->rollback(); // Rollback transaction on error
        return false;
    }

    QStringList sessions;
    if( course->hasLecture())
    {
        sessions.append("Lecture");

    }
    if( course->hasLab())
    {
        sessions.append("Lab");

    }
    if( course->hasTutorial())
    {
        sessions.append("Tutorial");
    }


    QString state = "";
    for ( auto group : this->getYears()[course->getyear()-1].getGroups())
    {
        for( auto student : group->getGroupStudents())
        {
            query.prepare("INSERT INTO StudentAttendance (ID, CourseID, sessiontype, State) VALUES (:id, :courseid, :session, :state)");
            for ( auto session : sessions)
            {
                query.bindValue(":id",student->getID());
                query.bindValue(":courseid",course->getID());
                query.bindValue(":session",session);
                if( session == "Tutorial" )
                {
                    for(int i = 0; i < course->getNumberofTutorialSessions(); i++)
                        state+="0";
                }
                else if( session == "Lecture" )
                {
                    for(int i = 0; i < course->getNumberofLectureSessions(); i++)
                        state+="0";
                }
                else if( session == "Lab" )
                {
                    for(int i = 0; i < course->getNumberofLabSessions(); i++)
                        state+="0";
                }
                query.bindValue(":state",state);


                if (!query.exec()) {
                    qWarning() << "Failed to insert the Student Attendance:" << query.lastError().text();
                    Sqlbase->rollback(); // Rollback transaction on error
                    return false;
                }
            }
            query.prepare("INSERT INTO GradesTable (StudentID, CourseID, Midterm, Tutorial_Lab_Average, `Final Exam`, Average) VALUES (:id, :Courseid, :midterm, :TLAverage, :final, :average)");
            query.bindValue(":id",student->getID());
            query.bindValue(":Courseid",course->getID());
            query.bindValue(":midterm",10);
            query.bindValue(":TLAverage",10);
            query.bindValue(":final",10);
            query.bindValue(":average",10);

            if (!query.exec()) {
                qWarning() << "Failed to create the Grades entry for student : "<<student->getName() <<" ended up with the following error : " << query.lastError().text();
                Sqlbase->rollback(); // Rollback transaction on error
                return false;
            }
        }
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        return false;
    }
    this->getYears()[course->getyear()-1].addCourse(course);// or we could have used the school class addcourse
    course->loadExamFromDatabase();
    return true;
}

Course* School::getAddedCourse()
{
    return AddedCourse;
}
void School::setAddedCourse(Course* course)
{
    AddedCourse = course;
}


bool School::deleteCourseDefinitively(Course * course )
{
    qDebug()<<" You are Here !!! 2 ";
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    // Delete from Student Schedules table
    query.prepare("DELETE FROM Courses WHERE CourseID = :id");
    query.bindValue(":id", QString::number(course->getID()));
    if (!query.exec()) {
        qWarning() << "Failed to delete Course from Courses:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }


    query.prepare("DELETE FROM TeacherRoles WHERE Courseid = :id");
    query.bindValue(":id",QString::number(course->getID()) );
    if (!query.exec()) {
        qWarning() << "Failed to delete from TeacherRoles:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    query.prepare("DELETE FROM CourseTeachers WHERE CourseID = :id");
    query.bindValue(":id",QString::number(course->getID()) );
    if (!query.exec()) {
        qWarning() << "Failed to delete from CourseTeachers:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    query.prepare("DELETE FROM ExamSchedule WHERE CourseID = :id");
    query.bindValue(":id",QString::number(course->getID()) );
    if (!query.exec()) {
        qWarning() << "Failed to delete from ExamSchedule:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    query.prepare("DELETE FROM StudentAttendance WHERE CourseID = :id");
    query.bindValue(":id",QString::number(course->getID()) );
    if (!query.exec()) {
        qWarning() << "Failed to delete from StudentAttendance:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }


    query.prepare("DELETE FROM GradesTable WHERE CourseID = :id");
    query.bindValue(":id",QString::number(course->getID()) );
    if (!query.exec()) {
        qWarning() << "Failed to delete from GradesTable:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }


    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction";
        Sqlbase->rollback();
        return false;
    }
    qDebug()<<" You are Here !!! 3 ";
    this->removeCourse(course->getyear(),course);
    return true;
}




bool School::AddLectureSessiontoDatabase(Course* course) // this function will add the lecture sessiontype specifier to the Student Attendance Table
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);
    QString state("");
    for ( auto group : this->getYears()[course->getyear()-1].getGroups())
    {
        for( auto student : group->getGroupStudents())
        {
            query.prepare("INSERT INTO StudentAttendance (ID, CourseID, sessiontype, State) VALUES (:id, :courseid, :session, :state)");
            query.bindValue(":id",student->getID());
            query.bindValue(":courseid",course->getID());
            query.bindValue(":session","Lecture");

            for(int i = 0; i < course->getNumberofLectureSessions(); i++)
                state+="0";

            query.bindValue(":state",state);

            if (!query.exec())
            {
                qWarning() << "Failed to insert the Student Attendance for the NEW Lecture:" << query.lastError().text();
                Sqlbase->rollback(); // Rollback transaction on error
                return false;
            }
        }
    }
    course->sethasLecture(true);
    course->UpdateLLTinDatabase();
    return true;
}



bool School::AddTutorialSessionstoDatabase(Course* course)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);
    QString state("");
    for ( auto group : this->getYears()[course->getyear()-1].getGroups())
    {
        for( auto student : group->getGroupStudents())
        {
            query.prepare("INSERT INTO StudentAttendance (ID, CourseID, sessiontype, State) VALUES (:id, :courseid, :session, :state)");
            query.bindValue(":id",student->getID());
            query.bindValue(":courseid",course->getID());
            query.bindValue(":session","Tutorial");

            for(int i = 0; i < course->getNumberofTutorialSessions(); i++)
                state+="0";

            query.bindValue(":state",state);

            if (!query.exec())
            {
                qWarning() << "Failed to insert the Student Attendance for the NEW Tutorial:" << query.lastError().text();
                Sqlbase->rollback(); // Rollback transaction on error
                return false;
            }
        }
    }
    course->sethasTutorial(true);
    course->UpdateLLTinDatabase();
    return true;
}


bool School::AddLabSessionstoDatabase(Course* course) //
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }

    QSqlQuery query(*Sqlbase);
    QString state("");
    for ( auto group : this->getYears()[course->getyear()-1].getGroups())
    {
        for( auto student : group->getGroupStudents())
        {
            query.prepare("INSERT INTO StudentAttendance (ID, CourseID, sessiontype, State) VALUES (:id, :courseid, :session, :state)");
            query.bindValue(":id",student->getID());
            query.bindValue(":courseid",course->getID());
            query.bindValue(":session","Lab");

            for(int i = 0; i < course->getNumberofLabSessions(); i++)
                state+="0";

            query.bindValue(":state",state);

            if (!query.exec())
            {
                qWarning() << "Failed to insert the Student Attendance for the NEW Lab:" << query.lastError().text();
                Sqlbase->rollback(); // Rollback transaction on error
                return false;
            }
        }
    }
    course->sethasLab(true);
    course->UpdateLLTinDatabase();
    return true;
}



bool School::removeLectureSessionfromDatabase(Course* course) // remove the lab teachers too
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    QSqlQuery query(*Sqlbase);


    query.prepare("DELETE FROM StudentAttendance WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue("session","Lecture");
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Lectures:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }
    query.prepare("DELETE FROM TeacherAttendance WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue("session","Lecture");
    if (!query.exec())
    {
        qWarning() << "Failed to delete from TeacherAttendance table the Lectures:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    course->removeLectureTeachers();
    course->sethasLecture(false);
    course->setNumberofLectureSessions(0);
    course->UpdateLLTinDatabase();
    course->UpdateCourseinDatabase();
    return true;
}



bool School::removeLabSessionsfromDatabase(Course* course)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    QSqlQuery query(*Sqlbase);

    qDebug()<<"DELETE FROM StudentAttendance WHERE CourseID = :id AND sessiontype = :session";
    query.prepare("DELETE FROM StudentAttendance WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Lab");
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Lab:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }
    qDebug()<<"DELETE FROM TeacherAttendance WHERE CourseID = :id AND sessiontype = :session";
    query.prepare("DELETE FROM TeacherAttendance WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Lab");
    if (!query.exec())
    {
        qWarning() << "Failed to delete from TeacherAttendance table the Lab:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    course->removeLabTeachers();
    course->sethasLab(false);
    course->setNumberofLabSessions(0);

    course->UpdateLLTinDatabase();
    course->UpdateCourseinDatabase();
    return true;
}

bool School::removeTutorialSessionfromDatabase(Course* course)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return false;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return false;
    }

    QSqlQuery query(*Sqlbase);

    query.prepare("DELETE FROM StudentAttendance WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Tutorial");
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Tutorial:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }
    query.prepare("DELETE FROM TeacherAttendance WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Tutorial");
    if (!query.exec())
    {
        qWarning() << "Failed to delete from TeacherAttendance table the Tutorial:" << query.lastError().text();
        Sqlbase->rollback();
        return false;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return false;
    }
    course->removeTutorialTeachers();
    course->sethasTutorial(false);
    course->setNumberofTutorialSessions(0);
    course->UpdateLLTinDatabase();
    course->UpdateCourseinDatabase();
    return true;
}



void School::EmptyCourseTeachersAttendance(Course* course)
{
    QMap<int, QVector<bool>> attendance;
    for (auto teacher : course->getCourseTeacher())
    {
        teacher->removeLectureAttendance();
        teacher->removeLabAttendance();
        teacher->removeTutorialAttendance();
    }
}



void School::updateLectureSessionNumberinDatabase(Course * course) // this will update the student attendance and the teacherattendance
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return ;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    QString state = "";
    for( int i = 0; i < course->getNumberofLectureSessions(); i++)
    {
        state+="0";
    }
    qDebug()<<"all ok here 1";
    for (auto teacher : course->getLectureTeachers())
    for (const int& group : teacher->getLectureAttendance().keys()) {
            teacher->setLectureAttendanceString(state, group);
    }
    qDebug()<<" ALL ok Here 2";
    QSqlQuery query(*Sqlbase);

    query.prepare("UPDATE StudentAttendance SET State = :state WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Lecture");
    query.bindValue(":state",state);
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Lecture:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }
    query.prepare("UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Lecture");
    query.bindValue(":State",state);
    if (!query.exec())
    {
        qWarning() << "Failed to delete from TeacherAttendance table the Lecture:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return ;
    }
    qDebug()<<" ALL ok Here 3";
}



void School::updateLabSessionNumberinDatabase(Course * course)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return ;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    QString state = "";
    for( int i = 0; i < course->getNumberofLabSessions(); i++)
    {
        state+="0";
    }
    for (auto teacher : course->getLabTeachers())
        for (const int& group : teacher->getLabAttendance().keys()) {
            teacher->setLabAttendanceString(state, group);
        }
    QSqlQuery query(*Sqlbase);

    query.prepare("UPDATE StudentAttendance SET State = :state WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Lab");
    query.bindValue(":state",state);
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Lab:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }
    query.prepare("UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Lab");
    query.bindValue(":State",state);
    if (!query.exec())
    {
        qWarning() << "Failed to delete from TeacherAttendance table the Lab:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return ;
    }
}


void School::updateTutorialSessionNumberinDatabase(Course * course)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return ;
    }
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return ;
    }
    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction";
        return ;
    }

    QString state = "";
    for( int i = 0; i < course->getNumberofTutorialSessions(); i++)
    {
        state+="0";
    }
    for (auto teacher : course->getTutorialTeachers())
        for (const int& group : teacher->getTutorialAttendance().keys()) {
            teacher->setTutorialAttendanceString(state, group);
        }

    QSqlQuery query(*Sqlbase);

    query.prepare("UPDATE StudentAttendance SET State = :state WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Tutorial");
    query.bindValue(":state",state);
    if (!query.exec())
    {
        qWarning() << "Failed to delete from StudentAttendance table the Tutorial:" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }
    query.prepare("UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session");
    query.bindValue(":id", course->getID());
    query.bindValue(":session","Tutorial");
    query.bindValue(":State",state);
    if (!query.exec())
    {
        qWarning() << " failed to UPDATE TeacherAttendance SET state = :State WHERE CourseID = :id AND sessiontype = :session" << query.lastError().text();
        Sqlbase->rollback();
        return ;
    }

    if (!Sqlbase->commit())
    {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        Sqlbase->rollback();
        return ;
    }
}


void School::loadStudentAttendance( Course* course)
{

    for(auto group : this->getYears()[course->getyear() -1].getGroups() )
    {
        this->loadCourseGroupAttendance(course,group);
    }
}

void School::loadCourseGroupAttendance( Course* course, Group * group)
{

    if( course->hasTutorial())
    {
        for( auto student : group->getGroupStudents())
        {
            course->loadTutorialStudentAttendance(student);
        }
    }

    if( course->hasLecture())
    {
        for( auto student : group->getGroupStudents())
        {
            course->loadLectureStudentAttendance(student);
        }
    }

    if( course->hasLab())
    {
        for( auto student : group->getGroupStudents())
        {
            course->loadLabStudentAttendance(student);
        }

    }
}



void School::loadTeacherScheduleItems(Teacher * teacher)
{
    if (!Sqlbase->isOpen()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    query.prepare("SELECT ScheduleItems FROM TeacherSchedule WHERE ID = :id");
    query.bindValue(":id", teacher->getID());

    if (!query.exec())
    {
        qWarning() << "Failed to query studentsSchedules" << query.lastError().text();
        return;
    }

    if (query.next()) {
        QString Schedule = query.value(0).toString();
        if (!Schedule.isEmpty()) {
            QStringList Items = Schedule.split("/", Qt::SkipEmptyParts);
            for (const QString &item : Items)
            {
                QString Time_Slot;
                QStringList Courses;

                if (!item.isEmpty())
                {
                    int indexOfBracket = item.indexOf('[');
                    if (indexOfBracket != -1) {
                        Time_Slot = item.left(indexOfBracket);
                        QString coursesStr = item.mid(indexOfBracket + 1, item.length() - indexOfBracket - 2); // Remove last ']' character
                        Courses = coursesStr.split('.', Qt::SkipEmptyParts);
                    }
                    else
                    {
                        Time_Slot = item;
                    }
                }

                if (!Time_Slot.isEmpty()) {
                    teacher->AddTimeSlotToSchedule(Time_Slot,Courses);
                }
            }
        }
    }

}

void School::WriteTeacherScheduleItemsToDatabase(Teacher * teacher)
{
    qDebug() << "Saving the Items";

    QString items;
    qDebug()<<"keys inside the saving function";
    for ( auto key : teacher->getTeacherScheduleItems().keys())
    {
        qDebug()<<key;
    }
    qDebug()<<teacher->getTeacherScheduleItems();

    for (auto key : teacher->getTeacherScheduleItems().keys()) {
        items += key;

        if (teacher->getTeacherScheduleItems()[key].isEmpty()) {
            items += "/";
            continue;
        }

        items += "[";
        for (auto cell : teacher->getTeacherScheduleItems()[key]) {
            items += cell + ".";
            qDebug()<<cell;
        }
        items.chop(1);
        items += "]/";
    }
    items.chop(1);

    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return;
    }

    QSqlQuery query(*Sqlbase);
    query.prepare("UPDATE TeacherSchedule SET ScheduleItems = :items WHERE ID = :id");
    query.bindValue(":items", items);
    query.bindValue(":id", teacher->getID());

    if (!query.exec()) {
        qWarning() << "Failed to write to TeacherSchedule: " << query.lastError().text();
    } else {
        qDebug() << "Items saved successfully";
    }
}

void School::UpdateExamDetailsInDatabase()
{
    for( int i = 0; i < 5 ; i++)
    {
        for (auto course : this->getYears()[i].getCourses())
        {
            course->WriteExamsToDatabase();
        }
    }
}
bool School::updateGroupNumber(Group* group,int newGroupNumber)
{
    if (!Sqlbase->open()) {
        qWarning() << "Database is not open!";
        return false;
    }

    // Start a transaction
    if (!Sqlbase->transaction()) {
        qWarning() << "Failed to start transaction:" << Sqlbase->lastError().text();
        return false;
    }

    // List of tables to update

    QSqlQuery query(*Sqlbase);
    query.prepare("UPDATE Students SET `Group` = :newGroupNumber WHERE `Group` = :oldGroupNumber AND Year = :year");
    query.bindValue(":newGroupNumber", newGroupNumber);
    query.bindValue(":oldGroupNumber", group->getGroupNumber());
    query.bindValue(":year",group->getyear());

    if (!query.exec())
    {
        qWarning() << "Failed to update table Students :" << query.lastError().text();
        Sqlbase->rollback(); // Rollback the transaction in case of error
        return false;
    }

    query.prepare("UPDATE Groups SET `Group` = :newGroupNumber WHERE `Group` = :oldGroupNumber AND Year = :year");
    query.bindValue(":newGroupNumber", newGroupNumber);
    query.bindValue(":oldGroupNumber", group->getGroupNumber());
    query.bindValue(":year",group->getyear());


    if (!query.exec())
    {
        qWarning() << "Failed to update table Students :" << query.lastError().text();
        Sqlbase->rollback(); // Rollback the transaction in case of error
        return false;
    }


    query.prepare("UPDATE StudentSchedules SET `Group` = :newGroupNumber WHERE `Group` = :oldGroupNumber AND Year = :year");
    query.bindValue(":newGroupNumber", newGroupNumber);
    query.bindValue(":oldGroupNumber", group->getGroupNumber());
    query.bindValue(":year",group->getyear());


    if (!query.exec())
    {
        qWarning() << "Failed to update table Students :" << query.lastError().text();
        Sqlbase->rollback(); // Rollback the transaction in case of error
        return false;
    }


    for(auto course : this->getYears()[group->getyear()-1].getCourses())
    {
        query.prepare("UPDATE TeacherRoles SET `Group` = :newGroupNumber WHERE `Group` = :oldGroupNumber AND courseid = :id");
        query.bindValue(":newGroupNumber", newGroupNumber);
        query.bindValue(":oldGroupNumber", group->getGroupNumber());
        query.bindValue(":id",course->getID());
        if (!query.exec())
        {
            qWarning() << "Failed to update table TeacherRoles :" << query.lastError().text();
            Sqlbase->rollback(); // Rollback the transaction in case of error
            return false;
        }
    }
    // Commit the transaction
    if (!Sqlbase->commit()) {
        qWarning() << "Failed to commit transaction:" << Sqlbase->lastError().text();
        return false;
    }
    return true;
}
void School::loadTeacherAttendance( Course* course)
{

    for(auto group : this->getYears()[course->getyear() -1].getGroups() )
    {
        if( course->hasTutorial())
        {
            if (course->getTutorialTeachers().contains(group->getGroupNumber()))
            {
                for( auto teacher : course->getTutorialTeachers())
                {
                    course->addTutorialTeacherAttendance(teacher,group->getGroupNumber());
                }

            }

        }


        if( course->hasLecture())
        {
            if (course->getLectureTeachers().contains(group->getGroupNumber()))
            {
                for( auto teacher : course->getLectureTeachers())
                {
                    course->addLectureTeacherAttendance(teacher,group->getGroupNumber());

                }
            }
        }

        if( course->hasLab())
        {
            if (course->getLabTeachers().contains(group->getGroupNumber()))
            {
                for( auto teacher : course->getLabTeachers())
                {
                    course->addLabTeacherAttendance(teacher,group->getGroupNumber());
                }
            }

        }
    }
}
