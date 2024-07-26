# School Management System
First QT C++ Project: A Simulation of a University Management Environment

## Description
The School Management System is a comprehensive application designed to streamline the administration of school activities. It provides functionalities for managing students, teachers, courses, schedules, and exams. The system offers an intuitive user interface and advanced features to ensure efficient and effective school management.

## Logging In
1. The application provides interfaces for both teachers and administrators to log into their accounts.
2. Initially, only the admin account is accessible using the username `Admin` and the password `Admin123`. It is recommended to change the password upon first login. Changing the admin name is not advisable for security reasons. Users must search for the admin account initially.
3. The login interface includes a ComboBox to help users select their ID in case of name duplicates.

## Features

### 1. Advanced Real-Time Search Mechanisms
- The system supports advanced search functionalities, allowing admins to search for teachers, students, and courses using various parameters and specifiers. The search mechanism comprises two methods: regular search and command search. The regular search uses names, while the command search, inspired by Linux terminal commands, uses regular expressions. Commands start with `#` followed by a specifier and parameter, separated by `:`. Example: `#id:2`.

#### Teachers:
- `id`
- `name`
- `email`
- Examples: `#id:0` (Admin), `#email:Admin@gmail.com`

#### Students:
- Same specifiers as teachers, with additional:
- `Y`: Selects students of a specified year
- `G`: Selects students of a specified group (must be used after `Y`)
- Examples: `#Y1`, `#Y5:G1`

#### Courses:
- `name`
- `id`
- `S`: Semester, can be used with `Y`
- `ss`: Session type (`Le`: Lectures, `La`: Labs, `T`: Tutorials), can be layered for intersections
- Examples: `#S1`, `#Y1:S1`, `#ss:Le`, `#ss:Le.La.T`

**Note:** Specifiers are case-insensitive.

### 2. Student and Teacher Profile Management
- Add images to student and teacher profiles for easy identification.
- Change students' groups seamlessly.
- View and change teacher passwords (note: not hashed, less secure).
- Validate and update entity details.
- Option to delete entities with prompts for placing them in the recycle bin or deleting permanently.

### 3. Recycle Bin Management
- Delete entities without removing them permanently, temporarily hiding their records.
- Recover entities and reassign them to new groups if necessary.
- Same search system for teachers and students as the main system.

### 4. Group Management
- Refresh and add groups.
- Group profiles provide details like the number of students, gender percentage, delegate and vice-delegate assignments.
- Option to delete groups with choices to reassign or delete students.

### 5. Teacher Management
- Admin has full control over teacher details, including names, emails, and passwords.
- Add and delete teachers as needed.
- Creating a teacher account is equivalent to adding a teacher.

### 6. Course Management
- Advanced search functionality by semester, year, and group.
- Course editor includes:
  1. Course teacher management
  2. Session handler
  3. Editing functionalities
  4. Session manager (assigns groups to course teachers and manages teacher attendance)

### 7. Schedule Management
- Manual assignment of courses, teachers, and rooms.
- Exam scheduling for midterms and finals by day and time, sortable by year.

### 8. Grades Management
- Grades section with a table for student grades and averages, modifiable by teachers and read-only for admins.
- Sortable by course and group.

### 9. Teacher Interface
- Teachers can choose courses, view groups, select session types, and manage attendance.
- Access to student profiles (read-only).
- Exam interface for loading student results.
- Customizable schedule for teachers.

## Database Structure
The system uses an SQLite database with 12 tables:

1. **Teachers**: Details of teachers
2. **Teacher Schedule**: Teacher schedule items
3. **TeacherRoles**: Assigns teachers to groups in courses
4. **TeacherAttendance**: Attendance records of teachers
5. **Students**: Details of students
6. **StudentSchedules**: Group schedules
7. **StudentAttendance**: Attendance records of students
8. **Groups**: Details of groups
9. **Courses**: Details of courses
10. **GradesTable**: Grades of students
11. **ExamSchedule**: Exam schedules for each course
12. **CourseTeachers**: General information on course teachers

## How to Add the Database File
Since the SQLite database file cannot be added to the resource files directly, follow these steps:

1. Place the database folder at the root of the project.
2. The application will locate the database using the following path:
   ```cpp
   sqlbase->setDatabaseName(QDir::currentPath() + "/Database/SchoolManagmentDatabase.db");
## Gained Experience and Knowledge
1. **Planning**: Planning is paramount to ensure the project's success. This project significantly enhanced my planning skills, as careful planning ensured every piece fell into place perfectly.
2. **Patience**: Patience is key to creating anything substantial. This project taught me the importance of taking time to ensure each step is completed correctly.
3. **Creativity and Learning**: Projects like this foster creativity and push boundaries. For example, I learned SQLite to implement the database tables and how to interact with the database using code, which seemed challenging at first.
4. **UI/UX Skills**: I improved my UI/UX skills and perception, learning how to create a user-friendly and intuitive interface for the application.
5. **Problem-Solving**: Tackling various challenges throughout the project enhanced my problem-solving abilities.
6. **Database Interaction**: I gained extensive experience in interacting with databases, applying OOP concepts, and managing data efficiently.
7. **Enhanced Technical Skills**: Working on this project honed my technical skills, particularly in QT and C++ development.

This project was a valuable learning experience that significantly improved my technical abilities and project management skills.
# Screenshots:
![Login Screen](assets/login_screen.png)
![Admin Dashboard](assets/admin_dashboard.png)
![Teacher Profile](assets/teacher_profile.png)
![Student List](assets/student_list.png)
![Course Management](assets/course_management.png)
![Schedule Management](assets/schedule_management.png)
![Grades Management](assets/grades_management.png)
