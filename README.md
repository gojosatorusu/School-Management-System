# School-Management-System
First QT C++ Project, a Simulation of a University Management Environment
# School Management System

## Description
The School Management System is a comprehensive application designed to streamline the administration of school activities. It provides functionalities for managing students, teachers, courses, schedules, and exams. The system offers an intuitive user interface and advanced features to ensure efficient and effective school management.

## Features

1. **Advanced Real-Time Search Mechanisms**
   - The system supports advanced search functionalities, allowing the admin to search for courses by various parameters like semester, year, group, and session types (lectures, labs, tutorials).
   - Commands start with `#` followed by a specifier (e.g., `#id:1`, `#ss:Le`, `#Y1:S1`).

2. **Student and Teacher Profile Management**
   - Allows adding images to student and teacher profiles for easy identification.
   - Enables changing students' groups with a seamless transition process.

3. **Recycle Bin Management**
   - Manages cases where a student is in the recycle bin and the group has been deleted, with the ability to recover the student and assign them to a new group.

4. **Group Management**
   - Features for refreshing and adding groups.
   - Group profiles provide details like the number of students, gender percentage, delegate and vice-delegate assignment, and the ability to delete groups with options to reassign or delete students.

5. **Teacher Management**
   - The admin has full control over teacher details, including names, emails, and even passwords (not hashed for easy recovery).
   - Teachers can be added and deleted as needed.

6. **Course Management**
   - Advanced search functionality by semester, year, and group.
   - Course editor includes a session handler for managing lectures, labs, and tutorials, and assigning teachers to courses and sessions.
   - Attendance management integrated into the course sessions.

7. **Schedule Management**
   - Manual assignment of courses, teachers, and rooms.
   - Exam scheduling to plan midterm and final exams by day and time, sortable by year.

8. **Grades Management**
   - Grades section with a table for student grades and averages (midterm, tutorial, lab, final exam, and overall average).
   - Sortable by course and group for easy access.

9. **Teacher Interface**
   - Teachers can choose from courses they teach, view groups, select session types, and manage attendance.
   - Access to student profiles without modification privileges.
   - Exam interface for loading student results.
   - Customizable schedule for teachers.

## Database Structure
The system uses an SQLite database with 12 tables:

1. **Teachers**: Details of teachers.
2. **Teacher Schedule**: Items of the teacher schedule.
3. **TeacherRoles**: Assigns teachers to groups in courses.
4. **TeacherAttendance**: Attendance records of teachers.
5. **Students**: Details of students.
6. **StudentSchedules**: Group schedules.
7. **StudentAttendance**: Attendance records of students.
8. **Groups**: Details of groups.
9. **Courses**: Details of courses.
10. **GradesTable**: Grades of students.
11. **ExamSchedule**: Schedule of exams for each course.
12. **CourseTeachers**: Teachers of the course (general information).

## How to Add the Database File
Since the SQLite database file cannot be added to the resource files directly, please follow these steps:

1. Place the database file in the `Database` directory at the root of the project.
2. The application will find the database using the following path:
   ```cpp
   sqlbase->setDatabaseName(QDir::currentPath() + "/Database/SchoolManagmentDatabase.db");
