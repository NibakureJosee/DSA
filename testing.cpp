
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include <bits/stdc++.h>
#include <vector>
#include <set>
using namespace std;

void returnInvalidCommandWarning(string command)
{
    cout << "Oops, command '" << command << "' is not supported!, type 'help' for support" << endl;
}
void returnList(string type)
{
    ifstream file("database.csv");
    if (!file)
    {
        cout << "Failed to open the file." << endl;
    }
    string line;

    if (type.compare("locations") == 0)
    {
        set<string> locations;
        while (getline(file, line))
        {
            stringstream ss(line);
            string item;
            getline(ss, item, ',');
            locations.insert(item);
        }
        for (const string &location : locations)
        {
            cout << location << endl;
        }
    }
    else
    {
        set<string> diseases;
        while (getline(file, line))
        {
            stringstream ss(line);
            string item;
            getline(ss, item, ',');
            getline(ss, item, ',');

            diseases.insert(item);
        }
        for (const string &disease : diseases)
        {
            cout << disease << endl;
        }
    }
    file.close();
}

void addLocation(string location)
{
    ofstream file("database.csv", ios::app);
    if (!file)
    {
        cout << "Failed to open the file." << endl;
    }
    file << location.append(",null,null") << endl;

    file.close();

    cout << "Location added successfully." << endl;
}
void recordDiseaseAndCases(string location, string disease, string cases)
{

    ifstream inputFile("database.csv");
    if (!inputFile)
    {
        cout << "Failed to open the file." << endl;
    }
    string line;
    vector<string> remainingLines;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        string item;
        getline(ss, item, ',');
        transform(item.begin(), item.end(), item.begin(), ::tolower);
        if (item == location)
        {
            getline(ss, item, ',');
            transform(item.begin(), item.end(), item.begin(), ::tolower);
            if (item == "null" || item == disease)
            {
                continue;
            }
            remainingLines.push_back(line);
        }
        else
        {
            remainingLines.push_back(line);
        }
    }

    ofstream outputFile("database.csv");
    if (!outputFile)
    {
        cout << "Failed to open the file." << endl;
    }
    remainingLines.push_back(location.append(",").append(disease).append(",").append(cases));
    for (const string &line : remainingLines)
    {
        outputFile << line << endl;
    }

    outputFile.close();

    cout << disease << " and cases added successfully." << endl;
}

void casesOfDisease(string disease)
{
    ifstream file("database.csv");
    if (!file)
    {
        cout << "Failed to open the file." << endl;
    }

    int totalCases = 0;
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string item;
        getline(ss, item, ',');
        getline(ss, item, ',');
        if (disease == item)
        {
            getline(ss, item, ',');
            totalCases = totalCases + stoi(item);
        }
    }

    cout << "There are " << totalCases << " total cases of " << disease << endl;

    file.close();
}

void casesOfDiseaseByLocation(string location, string disease)
{
    ifstream file("database.csv");
    if (!file)
    {
        cout << "Failed to open the file." << endl;
    }

    int totalCases = 0;
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string item;
        getline(ss, item, ',');
        transform(item.begin(), item.end(), item.begin(), ::tolower);
        if (item == location)
        {
            getline(ss, item, ',');
            transform(item.begin(), item.end(), item.begin(), ::tolower);
            if (disease == item)
            {
                getline(ss, item, ',');
                totalCases = totalCases + stoi(item);
            }
        }
    }
    cout << "There are " << totalCases << " total cases of " << disease << " in " << location << endl;
    file.close();
}

void deleteLocation(string location)
{
    ifstream inputFile("database.csv");
    if (!inputFile)
    {
        cout << "Failed to open the file." << endl;
    }
    vector<string> remainingLines;
    string line;
    while (getline(inputFile, line))
    {
        stringstream ss(line);
        string item;
        getline(ss, item, ',');
        if (item != location)
        {
            remainingLines.push_back(line);
        }
    }

    inputFile.close();

    ofstream outputFile("database.csv");
    if (!outputFile)
    {
        cout << "Failed to open the file." << endl;
    }
    for (const string &line : remainingLines)
    {
        outputFile << line << endl;
    }
    outputFile.close();
    cout << "Location deleted successfully." << endl;
}

void whereDiseaseExists(string disease)
{
    ifstream file("database.csv");
    if (!file)
    {
        cout << "Failed to open the file." << endl;
    }
    string line;
    set<string> locations;
    while (getline(file, line))
    {
        stringstream ss(line);
        string currentLocation;
        string item;
        getline(ss, item, ',');
        currentLocation = item;
        getline(ss, item, ',');
        transform(item.begin(), item.end(), item.begin(), ::tolower);
        if (disease == item)
        {
            locations.insert(currentLocation);
        }
    }

    if (locations.size() != 0)
    {
        for (const string &location : locations)
        {
            cout << location << endl;
        }
    }
    else
    {
        cout << "There are no locations with " << disease << endl;
    }
}

bool locationExists(string location)
{
    ifstream file("database.csv");
    if (!file)
    {
        cout << "Failed to open the file." << endl;
    }
    bool flag = false;
    string line;
    while (getline(file, line) && flag == false)
    {
        stringstream ss(line);
        string item;
        getline(ss, item, ',');
        // cout << "Location: " << item << endl;

        transform(item.begin(), item.end(), item.begin(), ::tolower);
        if (location == item)
        {
            flag = true;
            break;
        }
    }
    return flag;
}
void help()
{
    cout << "Need help? Type 'help' command then press Enter key." << endl;
    string command;
    do
    {
        cout << "\nConsole > ";
        getline(cin, command);
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        // Create a stringstream from the input string
        stringstream ss(command);
        // Vector to store the split tokens
        vector<string> tokens;
        // Temporary string to store each token
        string token;
        // Split the string by space and store the tokens in the vector
        while (ss >> token)
        {
            tokens.push_back(token);
        }
        if (tokens.size() == 1 && tokens.at(0).compare("help") == 0)
        {
            cout << "================================================================" << endl;
            cout << "*\t\t\t"
                 << "HELP MENU"
                 << "\t\t\t" << endl;
            cout << "================================================================" << endl;
            cout << "add <Location>"
                 << "\t\t\t\t"
                 << ":Add a new location" << endl;
            cout << "delete <Location>"
                 << "\t\t\t"
                 << ":Delete an existing location" << endl;
            cout << "record <Location> <disease> <cases>"
                 << "\t"
                 << ":Record a disease and its cases" << endl;
            cout << "list locations"
                 << "\t\t\t\t"
                 << ":List all existing locations" << endl;
            cout << "list diseases"
                 << "\t\t\t\t"
                 << ":List existing Diseases in locations" << endl;
            cout << "where <disease>"
                 << "\t\t\t\t"
                 << ":Find where disease exists" << endl;
            cout << "cases <location><disease>"
                 << "\t\t"
                 << ":Find cases of a disease in location" << endl;
            cout << "cases <disease>"
                 << "\t\t\t\t"
                 << ":Find total cases of a given disease" << endl;
            cout << "help"
                 << "\t\t\t\t\t"
                 << ":Prints user manual" << endl;
            cout << "Exit"
                 << "\t\t\t\t\t"
                 << ":Exit the program" << endl;
        }
        else if (tokens.size() == 2 && tokens.at(0).compare("list") == 0)
        {
            if (tokens.at(1).compare("locations") == 0)
            {
                returnList("locations");
            }
            else if (tokens.at(1).compare("diseases") == 0)
            {
                returnList("diseases");
            }
            else
            {
                returnInvalidCommandWarning(command);
            }
        }
        else if (tokens.size() == 2 && tokens.at(0).compare("add") == 0)
        {
            if (!locationExists(tokens.at(1)))
            {
                addLocation(tokens.at(1));
            }
            else
            {
                cout << "The location entered already exit" << endl;
            }
        }

        else if (tokens.size() == 2 && tokens.at(0).compare("cases") == 0)
        {
            casesOfDisease(tokens.at(1));
        }
        else if (tokens.size() == 3 && tokens.at(0).compare("cases") == 0)
        {
            if (locationExists(tokens.at(1)))
            {
                casesOfDiseaseByLocation(tokens.at(1), tokens.at(2));
            }
            else
            {
                cout << "The location entered doesn't exit" << endl;
            }
        }
        else if (tokens.size() == 2 && tokens.at(0).compare("where") == 0)
        {
            whereDiseaseExists(tokens.at(1));
        }
        else if (tokens.size() == 4 && tokens.at(0).compare("record") == 0)
        {
            recordDiseaseAndCases(tokens.at(1), tokens.at(2), tokens.at(3));
        }
        else if (tokens.size() == 2 && tokens.at(0).compare("delete") == 0)
        {
            if (locationExists(tokens.at(1)))
            {
                deleteLocation(tokens.at(1));
            }
            else
            {
                cout << "The location entered doesn't exit" << endl;
            }
        }
        else if (command.compare("exit") == 0)
        {
            cout << "BYE!" << endl;
            break;
        }
        else
        {
            returnInvalidCommandWarning(command);
        }
    } while (command.compare("exit") != 0);
}
int main()
{
    time_t currentTime = time(NULL);
    string time = ctime(&currentTime);
    cout << "=============================================" << endl;
    cout << "*\t"
         << "Welcome to Disease Cases Reporting System!"
         << "\t*" << endl;
    cout << "* "
         << "*****************************************"
         << " *" << endl;
    cout << "\t\t\t\t" << endl;
    cout << "* "
         << "It is developed by Josephine Nibakure as practical"
         << " *" << endl;
    cout << "* "
         << "evaluation for the end of Year 3."
         << "\t\t*" << endl;
    cout << "=============================================" << endl;
    cout << "Starting time: " << time << endl;
    help();
    return 0;
}
