//
//  Student_prep.cpp
//  Ne_correction
//
//  Created by Axella Dorcus on 19/06/2024.
//

#include "Student_prep.hpp"

#include <iostream>

using namespace std;

class Course {
public:
    string courseName;
    bool available;
    Course(string name,bool avail):
    courseName(name), available(avail){}
    
};

class Student {
public:
    string studentName;
    vector<string> courses;
    Student* next;

    Student(string name) : studentName(name), next(nullptr) {}
};

class LinkedList{
private:
    Student* head;
    
public:
    LinkedList():head(nullptr){}
    
    void addStudent(string name){
        
        Student* newStudent= new Student(name);
        newStudent->next=head;
        head=newStudent;
        
    }
    
    Student* findStudent(string name){
        Student* currrent=head;
        while (currrent!=nullptr) {
            if (currrent->studentName==name ) {
                return currrent;
            }
            currrent=currrent->next;
        }
        return nullptr;
    }
    };

