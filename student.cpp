#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// Course class to store course details and marks
class Course {
public:
    string code;
    string name;

    Course(string code, string name) {
        this->code = code;
        this->name = name;
    }

    // Function to convert Course data to CSV string
    string toCSVString() const {
        stringstream ss;
        ss << code << "," << name;
        return ss.str();
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

    // Function to add a course with marks to student's list
    void addCourse(string code, float marks) {
        courses.push_back(make_pair(code, marks));
    }

    // Function to update marks for a specific course
    void updateMarks(string code, float marks) {
        for (auto& course : courses) {
            if (course.first == code) {
                course.second = marks;
                return;
            }
        }
    }

    // Function to convert Student data to CSV string
    string toCSVString() const {
        stringstream ss;
        ss << id << "," << name;
        for (const auto& course : courses) {
            ss << "," << course.first << "," << course.second;
        }
        return ss.str();
    }

    // Function to check if student has already enrolled in a course
    bool hasCourse(string code) const {
        for (const auto& course : courses) {
            if (course.first == code) {
                return true;
            }
        }
        return false;
    }
};

// StudentManager class to manage students and their operations
class StudentManager {
private:
    vector<Student> students;
    vector<Course> courses;
    const string studentsFile = "students.csv"; // CSV File to store student data
    const string coursesFile = "courses.csv"; // CSV File to store course data

public:
    // Constructor to load students and courses from CSV files
    StudentManager() {
        loadStudents();
        loadCourses();
    }

    // Function to load students from CSV file
    void loadStudents() {
        ifstream infile(studentsFile);
        if (!infile) {
            cout << "File not found. Creating new file." << endl;
            return;
        }

        string line;
        while (getline(infile, line)) {
            stringstream ss(line);
            int id;
            string name;
            ss >> id;
            ss.ignore(); // Ignore comma
            getline(ss, name, ',');

            Student student(id, name);

            string courseCode;
            float marks;
            while (getline(ss, courseCode, ',')) {
                ss >> marks;
                student.addCourse(courseCode, marks);
            }

            students.push_back(student);
        }
        infile.close();
    }

    // Function to load courses from CSV file
    void loadCourses() {
        ifstream infile(coursesFile);
        if (!infile) {
            cout << "File not found. Creating new file." << endl;
            return;
        }

        string line;
        while (getline(infile, line)) {
            stringstream ss(line);
            string code, name;
            getline(ss, code, ',');
            getline(ss, name, ',');
            Course course(code, name);
            courses.push_back(course);
        }
        infile.close();
    }

    // Function to save students to CSV file
    void saveStudents() {
        ofstream outfile(studentsFile);
        if (!outfile) {
            cerr << "Error: Unable to open file for writing." << endl;
            return;
        }

        for (const auto& student : students) {
            outfile << student.toCSVString() << endl;
        }
        outfile.close();
    }

    // Function to save courses to CSV file
    void saveCourses() {
        ofstream outfile(coursesFile);
        if (!outfile) {
            cerr << "Error: Unable to open file for writing." << endl;
            return;
        }

        for (const auto& course : courses) {
            outfile << course.toCSVString() << endl;
        }
        outfile.close();
    }

    // Function to add a new student
    void addStudent(int id, string name) {
        Student newStudent(id, name);
        students.push_back(newStudent);
        saveStudents(); // Save updated list to file
    }

    // Function to display all students
    void displayStudents() {
        for (const auto& student : students) {
            cout << "ID: " << student.id << ", Name: " << student.name << endl;
            cout << "Courses and Marks:" << endl;
            for (const auto& course : student.courses) {
                cout << course.first << " (" << getCourseName(course.first) << "): " << course.second << endl;
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
        saveCourses(); // Save updated list to file
    }

    // Function to enroll student in a course
    void enrollStudentInCourse(int studentId, string courseCode, float marks) {
        for (auto& student : students) {
            if (student.id == studentId) {
                if (findCourse(courseCode)) {
                    if (!student.hasCourse(courseCode)) {
                        student.addCourse(courseCode, marks);
                        saveStudents(); // Save updated list to file
                        cout << "Student " << student.name << " enrolled in course " << courseCode << " - " << getCourseName(courseCode) << endl;
                        return;
                    } else {
                        cout << "Student " << student.name << " is already enrolled in course " << courseCode << endl;
                    }
                } else {
                    cout << "Course " << courseCode << " not found." << endl;
                }
                return;
            }
        }
        cout << "Student with ID " << studentId << " not found." << endl;
    }

    // Function to update marks for a course
    void updateMarks(int studentId, string courseCode, float marks) {
        for (auto& student : students) {
            if (student.id == studentId) {
                student.updateMarks(courseCode, marks);
                saveStudents(); // Save updated list to file
                cout << "Marks updated for student " << student.name << " in course " << courseCode << " - " << getCourseName(courseCode) << endl;
                return;
            }
        }
        cout << "Student with ID " << studentId << " not found." << endl;
    }

    // Function to check if course exists
    bool findCourse(string code) const {
        for (const auto& course : courses) {
            if (course.code == code) {
                return true;
            }
        }
        return false;
    }

    // Function to get course name from course code
    string getCourseName(string code) const {
        for (const auto& course : courses) {
            if (course.code == code) {
                return course.name;
            }
        }
        return "Course not found";
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
    string password;
    cout << "Enter admin password: ";
    cin >> password;

    if (password == "admin123") {
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
                    manager.addStudent(id, name);
                    break;
                }
                case 3:
                    manager.saveStudents();
                    manager.saveCourses();
                    cout << "Data saved." << endl;
                    break;
                case 4:
                    cout << "Logging out." << endl;
                    return;
                default:
                    cout << "Invalid option! Please try again.\n";
            }
        }
    } else {
        cout << "Incorrect password. Access denied." << endl;
    }
}

int main() {
    StudentManager manager;

    while (true) {
        cout << "\nMain Menu\n";
        cout << "1. Student Login\n";
        cout << "2. Admin Login\n";
        cout << "3. Exit\n";
        cout << "Enter option: ";
        int option;
        cin >> option;

        switch (option) {
            case 1: {
                int studentId;
                cout << "Enter student ID: ";
                cin >> studentId;
                studentMenu(manager, studentId);
                break;
            }
            case 2:
                adminMenu(manager);
                break;
            case 3:
                cout << "Exiting program." << endl;
                return 0;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    }

    return 0;
}

