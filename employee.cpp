#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Employee {
    int id;
    string name;
    double hourlyRate;
    int hoursWorked;
    double salary;
    double tax;
    double deductions;
    double netSalary;
};

void getDetails(Employee& emp) {
    cout << "Enter Employee ID: ";
    cin >> emp.id;
    cout << "Enter Employee Name: ";
    cin.ignore();
    getline(cin, emp.name);
    cout << "Enter Hourly Rate: ";
    cin >> emp.hourlyRate;
    cout << "Enter Hours Worked: ";
    cin >> emp.hoursWorked;
    cout << "Enter Deductions: ";
    cin >> emp.deductions;
    // Calculate salary, tax, and net salary
    emp.salary = emp.hourlyRate * emp.hoursWorked;
    emp.tax = emp.salary * 0.15; // Assuming a flat tax rate of 15%
    emp.netSalary = emp.salary - emp.tax - emp.deductions;
}

void displayDetails(const Employee& emp) {
    cout << "Employee ID: " << emp.id << endl;
    cout << "Employee Name: " << emp.name << endl;
    cout << "Hourly Rate: " << emp.hourlyRate << endl;
    cout << "Hours Worked: " << emp.hoursWorked << endl;
    cout << "Gross Salary: " << emp.salary << endl;
    cout << "Tax: " << emp.tax << endl;
    cout << "Deductions: " << emp.deductions << endl;
    cout << "Net Salary: " << emp.netSalary << endl;
}

void saveToFile(const Employee& emp) {
    ofstream file;
    file.open("employees.txt", ios::app);
    if (file.is_open()) {
        file << emp.id << "\n" << emp.name << "\n" << emp.hourlyRate << "\n" << emp.hoursWorked << "\n"
             << emp.deductions << "\n" << emp.salary << "\n" << emp.tax << "\n" << emp.netSalary << "\n";
        file.close();
    } else {
        cout << "Unable to open file!" << endl;
    }
}

void displayFromFile() {
    ifstream file;
    file.open("employees.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Unable to open file!" << endl;
    }
}

void displayMenu() {
    cout << "Payroll Management System" << endl;
    cout << "1. Add Employee" << endl;
    cout << "2. Display Employees" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    vector<Employee> employees;
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                Employee emp;
                getDetails(emp);
                employees.push_back(emp);
                saveToFile(emp);
                break;
            }
            case 2:
                displayFromFile();
                break;

            case 3:
                cout << "Exiting..." << endl;
                return 0;

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}

