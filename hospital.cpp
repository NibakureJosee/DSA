#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// Structure for Hospital data
struct Hospital {
    string H_name;
    string location;
    int available_beds;
    float rating;
    string contact;
    string doctor_name;
    int price;

    Hospital(const string& name, const string& loc, int beds, float rtg, const string& cont, const string& doc, int prc)
        : H_name(name), location(loc), available_beds(beds), rating(rtg), contact(cont), doctor_name(doc), price(prc) {}
};

// Structure for Patient data
struct Patient {
    string P_name;
    int P_id;
    string contact;
    int price;

    Patient(const string& name, int id, const string& cont, int prc)
        : P_name(name), P_id(id), contact(cont), price(prc) {}
};

// Node structure for Hospital linked list
struct HospitalNode {
    Hospital data;
    HospitalNode* next;

    HospitalNode(const Hospital& h) : data(h), next(nullptr) {}
};

// Node structure for Patient linked list
struct PatientNode {
    Patient data;
    PatientNode* next;

    PatientNode(const Patient& p) : data(p), next(nullptr) {}
};

// Class for Hospital Management System
class HospitalManagementSystem {
private:
    HospitalNode* hospitals_head;
    PatientNode* patients_head;

public:
    HospitalManagementSystem() : hospitals_head(nullptr), patients_head(nullptr) {}

    // Function to add a hospital
    void addHospital(const Hospital& h) {
        HospitalNode* newNode = new HospitalNode(h);
        newNode->next = hospitals_head;
        hospitals_head = newNode;
    }

    // Function to add a patient
    void addPatient(const Patient& p) {
        PatientNode* newNode = new PatientNode(p);
        newNode->next = patients_head;
        patients_head = newNode;
    }

    // Function to print hospital data
    void printHospitals() const {
        cout << "PRINT hospitals DATA:" << endl;
        HospitalNode* current = hospitals_head;
        while (current) {
            cout << "HospitalName: " << current->data.H_name << ", Location: " << current->data.location << ", Beds Available: "
                << current->data.available_beds << ", Rating: " << current->data.rating << ", Contact: " << current->data.contact
                << ", Doctor Name: " << current->data.doctor_name << ", Price Per Bed: " << current->data.price << endl;
            current = current->next;
        }
        cout << endl;
    }

    // Function to print patient data
    void printPatients() const {
        cout << "PRINT patients DATA:" << endl;
        PatientNode* current = patients_head;
        while (current) {
            cout << "Patient Name: " << current->data.P_name << ", Patient Id: " << current->data.P_id << ", Contact: "
                << current->data.contact << ", Expenditure: " << current->data.price << endl;
            current = current->next;
        }
        cout << endl;
    }

    // Function to search and print hospitals by name
    void searchHospitalByName(const string& name) const {
        bool found = false;
        HospitalNode* current = hospitals_head;
        while (current) {
            if (current->data.H_name == name) {
                cout << "HospitalName: " << current->data.H_name << ", Location: " << current->data.location << ", Beds Available: "
                    << current->data.available_beds << ", Rating: " << current->data.rating << ", Contact: " << current->data.contact
                    << ", Doctor Name: " << current->data.doctor_name << ", Price Per Bed: " << current->data.price << endl;
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            cout << "Hospital with name '" << name << "' not found." << endl;
        }

        cout << endl;
    }

    // Function to search and print patients by ID
    void searchPatientById(int id) const {
        bool found = false;
        PatientNode* current = patients_head;
        while (current) {
            if (current->data.P_id == id) {
                cout << "Patient Name: " << current->data.P_name << ", Patient Id: " << current->data.P_id << ", Contact: "
                    << current->data.contact << ", Expenditure: " << current->data.price << endl;
                found = true;
            }
            current = current->next;
        }

        if (!found) {
            cout << "Patient with ID '" << id << "' not found." << endl;
        }

        cout << endl;
    }

    // Function to sort hospitals by available beds
    void sortByBedsAvailable() {
        HospitalNode* current = hospitals_head;
        HospitalNode* nextNode = nullptr;
        Hospital temp("", "", 0, 0.0, "", "", 0);

        if (current == nullptr) {
            return;
        }

        while (current != nullptr) {
            nextNode = current->next;

            while (nextNode != nullptr) {
                if (current->data.available_beds < nextNode->data.available_beds) {
                    temp = current->data;
                    current->data = nextNode->data;
                    nextNode->data = temp;
                }

                nextNode = nextNode->next;
            }

            current = current->next;
        }
    }
};

int main() {
    HospitalManagementSystem hms;

    // Example data initialization
    hms.addHospital(Hospital("H1", "Bangalore", 4, 5.2, "657534XXX7", "D1", 100));
    hms.addHospital(Hospital("H2", "Bangalore", 5, 4.1, "298766XXX2", "D4", 200));
    hms.addHospital(Hospital("H4", "Mumbai", 6, 3.4, "324565XXX9", "D3", 100));
    hms.addHospital(Hospital("H3", "Prayagraj", 9, 5.9, "343456XXX4", "D2", 290));

    hms.addPatient(Patient("P1", 2, "234534XXX7", 1000));
    hms.addPatient(Patient("P2", 3, "234576XXX2", 1200));
    hms.addPatient(Patient("P3", 4, "857465XXX9", 1100));
    hms.addPatient(Patient("P4", 1, "567657XXX0", 600));

    // Perform operations
    hms.printHospitals();
    hms.printPatients();

    hms.searchHospitalByName("H2");
    hms.searchPatientById(3);

    hms.sortByBedsAvailable();
    cout << "Hospitals sorted by available beds:" << endl;
    hms.printHospitals();

    return 0;
}

