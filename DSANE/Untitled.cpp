#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cctype>

struct Item {
    int item_id;
    std::string item_name;
    int item_quantity;
    std::string item_registration_date;
};

std::string toLowercase(const std::string& str) {
    std::string result;
    for (char c : str) {
        result += std::tolower(c);
    }
    return result;
}

bool compareItems(const Item& item1, const Item& item2) {
    return toLowercase(item1.item_name) < toLowercase(item2.item_name);
}

bool isInteger(const std::string& input) {
    for (char c : input) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool isString(const std::string& input) {
    for (char c : input) {
        if (!std::isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

bool isDate(const std::string& input) {
    std::istringstream iss(input);
    int year, month, day;
    char delimiter;
    if (iss >> year >> delimiter >> month >> delimiter >> day) {
        return (iss.eof() && (delimiter == '-') && (month >= 1 && month <= 12) && (day >= 1 && day <= 31));
    }
    return false;
}

void addItem(int item_id, const std::string& item_name, int item_quantity, const std::string& item_registration_date) {
    std::ifstream file("items.csv");
    if (!file.is_open()) {
        std::ofstream createFile("items.csv");
        createFile.close();
        file.open("items.csv");
    }

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;
            std::getline(ss, token, ',');
            int existing_item_id;
            if (std::istringstream(token) >> existing_item_id && existing_item_id == item_id) {
                std::cout << "Item ID already exists!\n";
                return;
            }
        }
        file.close();

        std::ofstream outputFile("items.csv", std::ios::app);
        if (outputFile.is_open()) {
            outputFile << item_id << "," << item_name << "," << item_quantity << "," << item_registration_date << "\n";
            std::cout << "Item added successfully!\n";
            outputFile.close();
        } else {
            std::cout << "Unable to open file for writing!\n";
        }
    } else {
        std::cout << "Unable to open file for reading!\n";
    }
}

void listItems() {
    std::ifstream file("items.csv");
    if (!file.is_open()) {
        std::cout << "No items found!\n";
        return;
    }

    std::vector<Item> items;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Item item;
        std::getline(ss, line, ',');
        std::istringstream(line) >> item.item_id;
        std::getline(ss, item.item_name, ',');
        std::getline(ss, line, ',');
        std::istringstream(line) >> item.item_quantity;
        std::getline(ss, item.item_registration_date, ',');
        items.push_back(item);
    }
    file.close();

    std::sort(items.begin(), items.end(), compareItems);
    for (const auto& item : items) {
        std::cout << "Item ID: " << item.item_id << "\n";
        std::cout << "Item Name: " << item.item_name << "\n";
        std::cout << "Quantity: " << item.item_quantity << "\n";
        std::cout << "Reg Date: " << item.item_registration_date << "\n";
        std::cout << "----------------------\n";
    }
}

int main() {
    std::cout << "-----------------------------------------\n";
    std::cout << "     Welcome to RCA Inventory system!    \n";
    std::cout << "  Type 'help' and press enter for help.  \n";
    
    help();
 
    std::cout << "-----------------------------------------\n";
    std::cout << "    Thank you for using the system!      \n";
    std::cout << "-----------------------------------------\n";
    return 0;
}
