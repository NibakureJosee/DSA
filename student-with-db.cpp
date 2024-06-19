#include <iostream>
#include <pqxx/pqxx> // Include libpqxx headers
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace pqxx;

// Course class to store course details and marks
class Course {
public:
    string code;
    string name;

    Course(string code, string name) {
        this->code = code;
        this->name = name;
    }
};

// Student class to store student details and courses
class Student {
public:
    int id;
    string name;
    vector<pair<string, float>> courses; // Pair of course code and marks

    Student(int id, string name) {
        this->id = id;
        this->name = name;
    }
};

// StudentManager class to manage students and their operations
class StudentManager {
private:
    vector<Student> students;
    vector<Course> courses;
    connection conn; // PostgreSQL connection

public:
    // Constructor to initialize PostgreSQL connection
    StudentManager() : conn("dbname=mydatabase user=myuser password=mypassword hostaddr=127.0.0.1 port=5432") {
        if (!conn.is_open()) {
            cerr << "Failed to connect to database." << endl;
            exit(1);
        } else {
            cout << "Connected to database." << endl;
        }
    }

    // Function to load students from PostgreSQL database
    void loadStudents() {
        work txn(conn);
        result studentsResult = txn.exec("SELECT id, name FROM students");

        for (auto row : studentsResult) {
            int id = row["id"].as<int>();
            string name = row["name"].as<string>();
            Student student(id, name);
            students.push_back(student);
        }

        for (auto& student : students) {
            result coursesResult = txn.exec("SELECT course_code, marks FROM student_courses WHERE student_id = " + to_string(student.id));

            for (auto row : coursesResult) {
                string courseCode = row["course_code"].as<string>();
                float marks = row["marks"].as<float>();
                student.courses.push_back(make_pair(courseCode, marks));
            }
        }
    }

    // Function to load courses from PostgreSQL database
    void loadCourses() {
        work txn(conn);
        result coursesResult = txn.exec("SELECT code, name FROM courses");

        for (auto row : coursesResult) {
            string code = row["code"].as<string>();
            string name = row["name"].as<string>();
            Course course(code, name);
            courses.push_back(course);
        }
    }

    // Function to save students to PostgreSQL database
    void saveStudents() {
        work txn(conn);
        txn.exec("DELETE FROM student_courses");
        txn.exec("DELETE FROM students");

        for (const auto& student : students) {
            stringstream ss;
            ss << "INSERT INTO students (id, name) VALUES (" << student.id << ", '" << student.name << "')";
            txn.exec(ss.str());

            for (const auto& course : student.courses) {
                stringstream ss;
                ss << "INSERT INTO student_courses (student_id, course_code, marks) VALUES ("
                   << student.id << ", '" << course.first << "', " << course.second << ")";
                txn.exec(ss.str());
            }
        }

        txn.commit();
    }

    // Function to save courses to PostgreSQL database
    void saveCourses() {
        work txn(conn);
        txn.exec("DELETE FROM courses");

        for (const auto& course : courses) {
            stringstream ss;
            ss << "INSERT INTO courses (code, name) VALUES ('" << course.code << "', '" << course.name << "')";
            txn.exec(ss.str());
        }

        txn.commit();
    }

    // Function to add a new student
    void addStudent(int id, string name) {
        Student newStudent(id, name);
        students.push_back(newStudent);
    }

    // Function to display all students
    void displayStudents() {
        for (const auto& student : students) {
            cout << "ID: " << student.id << ", Name: " << student.name << endl;
            cout << "Courses and Marks:" << endl;
            for (const auto& course : student.courses) {
                cout << course.first << ": " << course.second << endl;
            }
            cout << endl;
        }
    }

    // Function to display available courses
    void displayCourses() {
        cout << "Available Courses:" << endl;
        for (const auto& course : courses) {
            cout << course.code << " - " << course.name << endl;
        }
        cout << endl;
    }

    // Function to add a course
    void addCourse(string code, string name) {
        Course newCourse(code, name);
        courses.push_back(newCourse);
    }

    // Function to enroll student in a course
    void enrollStudentInCourse(int studentId, string courseCode, float marks) {
        for (auto& student : students) {
            if (student.id == studentId) {
                student.courses.push_back(make_pair(courseCode, marks));
                cout << "Student " << student.name << " enrolled in course " << courseCode << endl;
                return;
            }
        }
        cout << "Student with ID " << studentId << " not found." << endl;
    }

    // Function to update marks for a course
    void updateMarks(int studentId, string courseCode, float marks) {
        for (auto& student : students) {
            if (student.id == studentId) {
                for (auto& course : student.courses) {
                    if (course.first == courseCode) {
                        course.second = marks;
                        cout << "Marks updated for student " << student.name << " in course " << courseCode << endl;
                        return;
                    }
                }
                cout << "Course " << courseCode << " not found for student " << student.name << endl;
                return;
            }
        }
        cout << "Student with ID " << studentId << " not found." << endl;
    }
};

void studentMenu(StudentManager& manager, int studentId) {
    while (true) {
        cout << "\nStudent Menu\n";
        cout << "1. Display Available Courses\n";
        cout << "2. Enroll in Course\n";
        cout << "3. Display My Courses\n";
        cout << "4. Update Marks\n";
        cout << "5. Exit\n";
        cout << "Enter option: ";
        int option;
        cin >> option;

        switch (option) {
            case 1:
                manager.displayCourses();
                break;
            case 2: {
                string courseCode;
                float marks;
                cout << "Enter course code to enroll: ";
                cin >> courseCode;
                cout << "Enter marks for the course: ";
                cin >> marks;
                manager.enrollStudentInCourse(studentId, courseCode, marks);
                break;
            }
            case 3:
                manager.displayStudents();
                break;
            case 4: {
                string courseCode;
                float marks;
                cout << "Enter course code to update marks: ";
                cin >> courseCode;
                cout << "Enter new marks: ";
                cin >> marks;
                manager.updateMarks(studentId, courseCode, marks);
                break;
            }
            case 5:
                cout << "Logging out." << endl;
                return;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    }
}

void adminMenu(StudentManager& manager) {
    while (true) {
        cout << "\nAdmin Menu\n";
        cout << "1. Add Course\n";
        cout << "2. Add Student\n";
        cout << "3. Save Data\n";
        cout << "4. Exit\n";
        cout << "Enter option: ";
        int option;
        cin >> option;

        switch (option) {
            case 1: {
                string code, name;
                cout << "Enter course code: ";
                cin >> code;
                cout << "Enter course name: ";
                cin.ignore(); // Ignore newline character
                getline(cin, name);
                manager.addCourse(code, name);
                break;
            }
            case 2: {
                int id;
                string name;
                cout << "Enter student ID: ";
                cin >> id;
                cout << "Enter student name: ";
                cin.ignore(); // Ignore newline character
                getline(cin, name);
                manager.addStudent
                                manager.saveCourses();
                cout << "Data saved." << endl;
                break;
            case 4:
                cout << "Exiting program." << endl;
                return;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    }
}

int main() {
    StudentManager manager;

    // Load data from database
    manager.loadStudents();
    manager.loadCourses();

    int userType;
    cout << "Select User Type:\n";
    cout << "1. Student\n";
    cout << "2. Admin\n";
    cout << "Enter option: ";
    cin >> userType;

    switch (userType) {
        case 1: {
            int studentId;
            cout << "Enter Student ID: ";
            cin >> studentId;
            studentMenu(manager, studentId);
            break;
        }
        case 2:
            adminMenu(manager);
            break;
        default:
            cout << "Invalid option! Exiting program." << endl;
    }

    return 0;
}


