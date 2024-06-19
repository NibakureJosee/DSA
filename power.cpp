#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct PreviousData {
    float prevPower;
    float prevAmount;
    int set;
    int month;
};

struct Customer {
    string customerName;
    string customerCategory;
    int cashPowerNumber;
    PreviousData customerData;
};

vector<Customer> customers;

// Load customer data from file
void loadCustomers(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        Customer customer;
        size_t pos = 0;
        
        pos = line.find(",");
        customer.customerName = line.substr(0, pos);
        line.erase(0, pos + 1);
        
        pos = line.find(",");
        customer.customerCategory = line.substr(0, pos);
        line.erase(0, pos + 1);
        
        customer.cashPowerNumber = stoi(line);
        customers.push_back(customer);
    }
    file.close();
}

// Check category of the customer
string checkCategory(int cashPower) {
    for (const auto& customer : customers) {
        if (customer.cashPowerNumber == cashPower) {
            return customer.customerCategory;
        }
    }
    return "no";
}

// Calculate power consumption for residential category
float residential(int amount) {
    if (amount <= 1335) {
        return amount / 89.0;
    } else if (amount <= 8755) {
        return 15 + (amount - 1335) / 212.0;
    } else {
        return 50 + (amount - 8755) / 249.0;
    }
}

// Generate token for the transaction
string generateToken(int cashPower, int amount) {
    srand(time(0));
    int token1 = rand() % 1000000;
    int token2 = rand() % 100000;
    return to_string(token1) + " " + to_string(token2) + " " + to_string(cashPower) + " " + to_string(amount);
}

// Get current month
int getCurrentMonth() {
    time_t t = time(nullptr);
    tm* timePtr = localtime(&t);
    return timePtr->tm_mon + 1;
}

// Update previous data for a customer
void updatePrevData(Customer& customer, int amount, float power) {
    customer.customerData.prevAmount = amount;
    customer.customerData.prevPower = power;
    customer.customerData.set = 1;
    customer.customerData.month = getCurrentMonth();
}

// Get previous data for a customer
PreviousData getPreviousData(const Customer& customer) {
    return customer.customerData;
}

// Main application logic
void runApp() {
    cout << "--------------- Welcome to E-Pay -----------------\n\n";

    while (true) {
        int cashPower;
        float providedPower;
        string category;
        int amount;

        cout << "Enter amount: ";
        cin >> amount;
        cout << "Enter cash power number: ";
        cin >> cashPower;

        category = checkCategory(cashPower);
        if (category == "no") {
            cout << "The customer does not exist.\n";
            continue;
        }

        auto it = find_if(customers.begin(), customers.end(), [cashPower](const Customer& c) { return c.cashPowerNumber == cashPower; });
        if (it == customers.end()) {
            cout << "Customer not found.\n";
            continue;
        }

        Customer& customer = *it;

        if (category == "residential") {
            float currentPower = residential(amount);
            PreviousData prevData = getPreviousData(customer);

            if (prevData.set == 1) {
                amount += prevData.prevAmount;
                currentPower = residential(amount);
                updatePrevData(customer, amount, currentPower);
                float givenPower = currentPower - prevData.prevPower;
                cout << "The power is: " << givenPower << " kWh\n";
                cout << "Enter this number: " << generateToken(cashPower, amount) << "\n";
            } else {
                updatePrevData(customer, amount, currentPower);
                cout << currentPower << " kWh\n";
                cout << "Enter this number: " << generateToken(cashPower, amount) << "\n";
            }
        } else if (category == "non-residential") {
            // Add your non-residential logic here
        } else if (category == "Hotels") {
            providedPower = amount / 157.0;
            cout << providedPower << " kWh\n";
            cout << "Enter this number: " << generateToken(cashPower, amount) << "\n";
        } else if (category == "telecom-towers") {
            providedPower = amount / 201.0;
            cout << providedPower << " kWh\n";
            cout << "Enter this number: " << generateToken(cashPower, amount) << "\n";
        } else if (category == "Water Treatment plants and Water pumping stations") {
            providedPower = amount / 126.0;
            cout << providedPower << " kWh\n";
            cout << "Enter this number: " << generateToken(cashPower, amount) << "\n";
        } else if (category == "Health Facilities") {
            providedPower = amount / 186.0;
            cout << providedPower << " kWh\n";
            cout << "Enter this number: " << generateToken(cashPower, amount) << "\n";
        } else if (category == "Broadcasters") {
            providedPower = amount / 192.0;
            cout << providedPower << " kWh\n";
            cout << "Enter this number: " << generateToken(cashPower, amount) << "\n";
        } else if (category == "Commercial Data Centers") {
            providedPower = amount / 179.0;
            cout << providedPower << " kWh\n";
            cout << "Enter this number: " << generateToken(cashPower, amount) << "\n";
        } else {
            cout << "Unknown category.\n";
            continue;
        }

        int cont;
        cout << "Do you want to continue (1 for yes, 0 for no): ";
        cin >> cont;
        if (cont != 1) {
            break;
        }
    }
}

int main() {
    loadCustomers("customers.txt");
    runApp();
    return 0;
}

