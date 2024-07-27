#ifndef ENTITY_H
#define ENTITY_H
#include<QString>
#include<QList>
#include<QMap>
#include<QVector>
#include<QtSql>
#include <QDate>
#include <QTime>



enum class EntityType
{
    Student,
    Teacher,
    Admin
};

enum class SessionType
{
    Lecture,
    Lab,
    Tutorial,
    NoSession
};

// the entity base class, Student, Teacher will inherit from this class
class Entity
{
public:
    Entity()=default;
    Entity(QSqlDatabase* ,QString = "",QString = "",QString = "",int = 0,int = 0, QString ="",QString="I",bool = false);
    virtual ~Entity() = 0;
public:
    QString getName() const;
    QString getPhone_Number()const;
    QString getEmail()const;
    int getID()const;
    int getDatabaseID();
    QString getImagepath();
    QString getgender();
    bool getdeletion_State();



    void setName(const QString &);
    void setID(int );
    void setPhone_Number(const QString &);
    void setEmail(const QString &);
    void setDatabaseID(int);
    void setImagepath(QString);
    void setgender(QString);
    void setdeletion_State(bool);

private:
    QString Name;
    int ID;
    int databaseID; // is used to identify the row when changing the profile of the entity. and with this the admin is graced with the privlige of chenging the ID of the teachers and students
    QString Email;
    QString phone_Number;
    QString imagepath;
    QString gender;
    bool deletion_State;
    QSqlDatabase* Sqlbase;

};



class Teacher: public Entity
{
public:
    ~Teacher();
    Teacher()=default;
    Teacher(QSqlDatabase* ,QString = "",QString = "",QString = "",int = 0,QString="",int = 0, QString ="", QString = "I",bool = false);//will initialize the attendance class place holder from the file and then proceed to use it for statistics

    //getters
    QMap<int, QVector<bool>> getLectureAttendance() const;
    QMap<int, QVector<bool>> getLabAttendance() const;
    QMap<int, QVector<bool>> getTutorialAttendance() const;
    double get_Attendance_rate();
    QString getpassword()const;
    QMap<QString,QStringList> getTeacherScheduleItems();
    //setters
    void setpassword(const QString &);
    void setLectureAttendance(int Group , QVector<bool>);
    void setLabAttendance(int Group , QVector<bool>);
    void setTutorialAttendance(int Group , QVector<bool> );
    // these function will be used to set the lecture, lab and tutorial through Strings because of simplicity, and they are very limited in utilization
    void setLectureAttendanceString(QString ,int );
    void setLabAttendanceString(QString ,int );
    void setTutorialAttendanceString(QString,int );
    //these are specific to sessions
    void addLectureAttendance(int, QVector<bool>&);
    void addLabAttendance(int, QVector<bool>& attendance);
    void addTutorialAttendance(int, QVector<bool>& attendance);

    // remove functions for sessions
    void removeLectureAttendance();
    void removeLabAttendance();
    void removeTutorialAttendance();
    void removeLectureTeacherToGroupAttendance(int);
    void removeTutorialTeacherToGroupAttendance(int);
    void removeLabTeacherToGroupAttendance(int);
    //updates the teacher
    void update_Teacher( QSqlDatabase* sqlbase);
    // updates the Time Slots
    void AddTimeSlotToSchedule(const QString &,QStringList);
    void removeTimeSlotFromSchedule(const QString &);

private:
    QMap<int, QVector<bool>> Lecture_Attendance;
    QMap<int, QVector<bool>> Lab_Attendance;
    QMap<int, QVector<bool>> Tutorial_Attendance;
    QString Password;
    QMap<QString,QStringList> TeacherScheduleItems;
};


class Student : public Entity // there is a constructor here
{
public:
    Student()=default;
    Student(QSqlDatabase* ,QString = "",QString = "",QString = "",int = 0, int = 1, int = 0,int = 0, QString ="", QString = "I",QString="Student", bool = false);
    ~Student();
    //getters
    QVector<bool> getLabAttendance() const;
    QVector<bool> getTutorialAttendance() const;
    QVector<bool> getLectureAttendance() const;
    int getYear() const;
    int getgroup()const;
    QString getposition();
    QVector<float> getGrades();
    //setters
    void setLabAttendance(QVector<bool>);
    void setTutorialAttendance(QVector<bool> );
    void setLectureAttendance(QVector<bool>);
    void setyear(int Year);
    void setgroup(int Group);
    void setposition(QString);
    void setMidterm(float);
    void setT_L_Avg(float);
    void setFinal_Exam(float);
    void setAvg(float);
    void setGradee(int i , float Grade);

    void emptyLabAttendance();
    void emptyLectureAttendance();
    void emptyTutorialAttendance();

    //will update the student details
    void update_Student(QSqlDatabase*);

private:
    int year;
    int group;
    QString position;
    QVector<bool> TutorialAttendance;
    QVector<bool> LabAttendance;
    QVector<bool> LectureAttendance;
    QList<float> Grades{10,10,10,10};
};





class Group {
public:
    // Constructors
    Group();
    Group(int groupNumber,int year,QSqlDatabase* );
    ~Group();

    // Getters
    int getGroupNumber() const;
    const QList<Student *> getGroupStudents() const;
    int getNumberOfStudents() const;
    int getyear();
    Student* getDelegate();
    Student* getSubDelegate();
    QVector<bool> getLabbAttendance();
    QVector<bool> getTutorialbAttendance();
    double MaleRate();
    double FemaleRate();

    // Setters
    void setGroupNumber(int groupNumber);
    void setGroupStudents(const QList<Student*>& students);
    void setYear(int);
    void setDelegate(Student*);
    void setSubDelegate(Student*);
    void setLabbAttendance(QVector<bool>);
    void setTutorialbAttendance(QVector<bool>);
    void setGroupScheduleItems(const QMap<QString,QStringList> &);
    QMap<QString,QStringList> getGroupScheduleItems();

    // Methods to manipulate students
    void addStudent(Student* student);
    void removeStudent(Student* student);
    void loadGroupStudentsFromDatabase();

    bool Alldeleted();
    void AddTimeSlotToSchedule(const QString &,QStringList);
    void removeTimeSlotFromSchedule(const QString &);
    void loadGroupScheduleItems();
    void WriteScheduleItemsToDatabase();

private:
    int group_Number;
    int year;
    QList<Student*> Students;
    Student* delegate;
    Student* SubDelegate;
    int Number_of_students;
    QSqlDatabase* Sqlbase;
    int schedule_rows;
    QMap<QString,QStringList> GroupScheduleItems;
private :
// Helper method to update the number of students
void updateNumberOfStudents();

};// wil contain a list of students because it makes removal and addition easier


class Exam
{
public:
    // Constructors
    Exam();
    Exam(const QDate &examDay, const QTime &startTime, const QTime &endTime);

    // Getters
    QDate getExamDay() const;
    QTime getStartTime() const;
    QTime getEndTime() const;

    // Setters
    void setExamDay(const QDate &examDay);
    void setStartTime(const QTime &startTime);
    void setEndTime(const QTime &endTime);

private:
    QDate examDay;
    QTime startTime;
    QTime endTime;
};


class Course {
public:
    // Constructors
    Course();
    Course(QSqlDatabase* ,int id, const QString &courseName,int, int, int,QList<Teacher*>, int , int, QString, int);
    ~Course();
    // Getters
    int getID() const;
    int getyear()const;
    QString getCourseName() const;
    QMap<int, Teacher*> getLabTeachers() const;
    QMap<int, Teacher*> getTutorialTeachers() const;
    QMap<int, Teacher*> getLectureTeachers() const;
    int getsemester();
    int getNumberofLectureSessions();
    int getNumberofLabSessions();
    int getNumberofTutorialSessions();
    int getCoef();
    Exam* getMidterExam();
    Exam* getFinalExam();
    bool hasLecture();
    bool hasLab();
    bool hasTutorial();
    //setters
    void setFinalExam(Exam*);
    void setMidtermExam(Exam*);
    QList<Teacher*>getCourseTeacher()const;
    void setID(int id);
    void setCourseName(const QString &courseName);
    void setyear(int);
    void setSemester(int );
    void setNumberofLectureSessions( int );
    void setNumberofLabSessions( int );
    void setNumberofTutorialSessions( int );
    void setCoef(int);
    void sethasLecture(bool);
    void sethasLab(bool);
    void sethasTutorial(bool);
    //session Manipulators
    void addLabTeacher(int groupID,Teacher *teacher);
    void addTutorialTeacher(int groupID,  Teacher* teacher);
    void addLectureTeacher(int groupID,Teacher *teacher);
    void removeLabTeacher(int groupID);
    void removeTutorialTeacher(int groupID);
    void removeLectureTeacher(int groupID);
    void removeteacherFromCourse(Teacher*);

    void ADDCourseTeacherToList(Teacher*);
    void removeCourseTeacherFromList(Teacher*);

    void loadTeacherRole(const QList<Teacher *>&  Teachers);

    void extractNumbers(const QString& str);// this function will extract from a string passed to it if the Course has Lecture Session, Lab Sessions and Tutorial Sessions
    bool UpdateLLTinDatabase();
    bool UpdateCourseinDatabase();
    void removeLectureTeachers();
    void removeLabTeachers();
    void removeTutorialTeachers();


    // this will work as a completion to add and remove the roles of teachers, this is specific to the database
    void addTutorialTeachertoDatabase(int groupID,  Teacher *teacher);
    void addLabTeachertoDatabase(int groupID,  Teacher *teacher);
    void addLectureTeachertoDatabase(int groupID,  Teacher *teacher);
    void removeLabTeacherFromDatabase(int groupID);
    void removeTutorialTeacherFromDatabase(int groupID);
    void removeLectureTeacherFromDatabase(int groupID);

    void removeCourseTeacherFromDatabase(Teacher*);
    void AddCourseTeacherToDatabase(Teacher*);

    // these will be used mainly for loading specific attendance from the database :
    void addLectureTeacherAttendance(Teacher*, int group);
    void addLabTeacherAttendance(Teacher*, int);
    void addTutorialTeacherAttendance(Teacher*, int);
    // these are like the one above but for students
    void loadLectureStudentAttendance(Student * student);
    void loadTutorialStudentAttendance(Student * student);
    void loadLabStudentAttendance(Student * student);
    // these will update the attendance in the Database after he changes something
    // update the database Attendance for teachers :
    bool updateLectureTeachersAttendanceinDatabase();
    bool updateTutorialTeachersAttendanceinDatabase();
    bool updateLabTeachersAttendanceinDatabase();
    //these are for one Teacher
    bool updateTeacherLectureAttendanceinDatabase(Teacher *);
    bool updateTeacherTutorialAttendanceinDatabase(Teacher *);
    bool updateTeacherLabAttendanceinDatabase(Teacher *);
    // update the database Attendance for Students
    bool updateLectureStudentsAttendanceinDatabase(Group *);
    bool updateTutorialStudentsAttendanceinDatabase(Group*);
    bool updateLabStudentsAttendanceinDatabase(Group*);
    //these are for one student
    bool updateStudentLectureAttendanceinDatabase(Student *);
    bool updateStudentTutorialAttendanceinDatabase(Student *);
    bool updateStudentLabAttendanceinDatabase(Student *);
    //
    void loadExamFromDatabase();
    void WriteExamsToDatabase();
    //these will load the Students Grades from the Database and Write To it
    void loadGroupGradesFromDatabase(Group*);
    void loadStudentGradesFromDatabase(Student*);
    void WriteGroupGradesToDatabase(Group*);
    void WriteStudentGradesToDatabase(Student *);
private:
    int ID;
    QString Course_name;
    int year;
    int coefficient;
    int Number_Of_Lecture_Sessions;
    int Number_Of_Tutorial_Sessions;
    int Number_Of_Lab_Sessions;
    int semester;
    QMap<int, Teacher*> Lab_teachers;
    QMap<int, Teacher*> Tutorial_teachers;
    QMap<int, Teacher*> Lecture_teachers;
    QList<Teacher *> CourseTeachers;
    QSqlDatabase* Sqlbase;
    QList<bool> hasSession;// this will indicate if the Course Contains Lectures or Labs or Tutorials and just in this order : index 0 is for lectures, 1 is for labs, 2 is for tutorials
    Exam* Midterm_Exam;
    Exam* Final_Exam;
};

class Year {
public:
    Year() = default;
    Year(QSqlDatabase*, QList<Teacher*>*,int = 0);

    int getYear() const;
    int getnumberofgroups() const;
    QList<Group*> getGroups() const;
    QList<Course*> getCourses() const;
    void addGroup(Group* group);
    void removeGroup(Group* group);
    void addCourse(Course* course);
    void removeCourse(Course* course);
    void loadGroupsFromDatabase();
    void loadCoursesFromDatabase(QList<Teacher*>*);

private:
    int year;
    int Number_of_groups;
    int NumberOfCourses;
    QList<Group*> groups;
    QList<Course*> courses;
    QSqlDatabase* Sqlbase;

};


class School {
public:
    School(QSqlDatabase* );
    ~School();
    //getters
    Year* getYears() const;
    QList<Teacher*> getTeachers() const;
    QSqlDatabase* getdatabase()const;
    static int getNumberofyears();
    Group* getaddedgroup();
    Course* getAddedCourse();
    void addCourse(int year, Course* course);
    void removeCourse(int year, Course* course);

    void addTeacher(Teacher* teacher);
    void removeTeacher(Teacher* teacher);
    void loadTeachersFromDatabase();
    //helper functions for UI :
    Student* findStudentByName(const QString& );
    Teacher* findTeacherByName( const QString& name);
    Group *  findGroupByName(const QString& name);
    Course* findCoursebyName(const QString & name);

    //setters
    void setaddedgroup(Group*);
    void setAddedCourse(Course*);

    bool removeStudentDefinitively(Student*);
    bool removeTeacherDefinitively(Teacher*);
    void deletestudentsfromgroup(QList<Student*>);//will empty students from group
    bool deleteGroupdefinitively(Group*);
    bool deleteCourseDefinitively(Course *);
    // will be used to manipulate the RemovedStudents structure
    void addremmovedstudent(Student* student);
    void recoverRemovedstudent(Student* student);
    QList<Student*> getRemovedStudents()const;
    //this function will be used to update the group's number in the database after changing it
    bool updateGroupNumber(Group* group,int newGroupNumber);
    bool groupExists(int year, int groupNumber);
    bool AddStudentTodatabase(Student*);
    bool AddTeacherTodatabase(Teacher*);
    bool AddCourseTodatabase(Course* );
    // these are database manipulators
    bool AddLectureSessiontoDatabase(Course *);
    bool AddLabSessionstoDatabase(Course *);
    bool AddTutorialSessionstoDatabase(Course *);
    bool removeLectureSessionfromDatabase(Course *);
    bool removeLabSessionsfromDatabase(Course *);
    bool removeTutorialSessionfromDatabase(Course *);
    void loadTeacherAttendance(Course*);// this will be used to load the teachers attendance for lectures, tutorials, labs when the Admin (user) clicks on the course item to examine it
    void loadStudentAttendance( Course* course);
    void loadCourseGroupAttendance( Course* course, Group * group); //this wil be more specific as it will cover only a passed Group , so that it is more dynamic
    void EmptyCourseTeachersAttendance(Course* course);
    // will update the course number of sessions in the database ( not in the Courses Table but rather in the StudentAttendance and TeacherAttendance) and also will update the current in real time attendance of teachers and students
    void updateLectureSessionNumberinDatabase(Course *);
    void updateLabSessionNumberinDatabase(Course *);
    void updateTutorialSessionNumberinDatabase(Course *);
    // will update the exam table
    void UpdateExamDetailsInDatabase();
    //these will be used to load the Teachers' Schedule :
    void loadTeacherScheduleItems(Teacher *);
    void WriteTeacherScheduleItemsToDatabase(Teacher *);

private:
    Year* years;
    QList<Teacher*> teachers;
    QSqlDatabase* Sqlbase;
    static constexpr int numberOfYears = 5;
    Group* AddedGroup;
    Course* AddedCourse;
    //this will be used to store the deleted students no matter what their group or year is, this will be added for easier manipulation, and there is no need for such a structure for teachers
    QList<Student*> RemovedStudents;
};

void QString_To_Attendance(QString AttendanceString, QVector<bool>& Attendance);// a helper function to load the attendance of each teacher
// this will be used as an identifier to both teacher / student used dialogs to have multiple forms


#endif // ENTITY_H
