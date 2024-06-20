#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int maxRow = 10;

string EmpName[maxRow] = {};
string EmpID[maxRow] = {};


void OpenFile(){
        string line;

        ifstream employeesFile;
        employeesFile.open("employees.txt");

        if(employeesFile.is_open()){
            int x=0;
            while (getline(employeesFile,line))
            {
               int l = line.length(); 
               EmpID[x] = line.substr(0,3);
               EmpName[x] = line.substr(4,1-4);
               x++;
            }
            
        }
        else{
             cout<<"Unable to open the file!"<<endl;
        }
        
}

void AddRecord()
{
    char name[50];
    char empNo[5];

    cin.ignore();

    cout << "Enter Employee ID: " << endl;
    cin.getline(empNo, 5);

    cout << "Enter Employee Name: " << endl;
    cin.getline(name, 50);

    for (int x = 0; x < maxRow; x++)
    {
        if (EmpID[x] == "\0")
        {
            EmpID[x] = empNo;
            EmpName[x] = name;
            break;
        }
    }
}

void DisplayAllRecords()
{

    int counter = 0;
    cout << "Current Record(s)" << endl;
    cout << "=================" << endl;
    cout << "NO. |  ID |    NAME    " << endl;
    cout << endl;

    for (int x = 0; x < maxRow; x++)
    {
        if (EmpID[x] != "\0")
        {
            counter++;
            cout << " " << counter << "    " << EmpID[x] << "       " << EmpName[x] << endl;
        }
    }

    if (counter == 0)
    {
        cout << "No record found" << endl;
    }

    cout << "=====================" << endl;
}

void SearchRecord(string empID)

{
    int counter = 0;
    cout << "Current Record(s) " << endl;
    cout << "================" << endl;

    cout << "NO. |  ID |    NAME    " << endl;
    cout << endl;

    for (int x = 0; x < maxRow; x++)
    {
        if (EmpID[x] == empID)
        {
            counter++;
            cout << " " << counter << "    " << EmpID[x] << "       " << EmpName[x] << endl;
        }
    }

    if (counter == 0)
    {
        cout << "RECORD BY ID NOT FOUND " << endl;
    }

    cout << "===========================" << endl;
}

void UpdateRecord(string empID, string newEmpName)
{
    int counter = 0;

    for (int x = 0; x < maxRow; x++)
    {
        if (EmpID[x] == empID)
        {
            counter++;
            EmpName[x] = newEmpName;

            cout<<"Employee Successfully Updated"<<endl;
        }
    }

    if (counter == 0)
    {
        cout << "RECORD BY ID NOT FOUND " << endl;
    }

    cout << "===================" << endl;
}

void DeleteRecord(string empID){

    int counter =0;

    for (int x = 0; x < maxRow; x++)
    {
        if (EmpID[x] == empID)
        {
            counter++;
            EmpID[x] = "";
            EmpName[x] = "";
            cout<<"Employee Successfully Deleted!"<<endl;
        }
    }

    if (counter == 0)
    {
        cout << "RECORD BY ID NOT FOUND " << endl;
    }
}

void saveToFIle(){
    ofstream employeesFile;
    employeesFile.open("employees.txt");

    for(int x = 0; x<maxRow; x++){
        if(EmpID[x] == "\0"){
            break;
        }
        else{
            employeesFile<<EmpID[x] + "," + EmpName[x]<<endl;
        }
    }
}

int main()
{
    int option;
    string empID;
    string newEmpName;
    cout << "MENU" << endl;
     
     OpenFile();

    do
    {

        cout << "1.CREATE RECORD" << endl;
        cout << "2.UPDATE RECORD" << endl;
        cout << "3.DELETE RECORD" << endl;
        cout << "4.SEARCH RECORD" << endl;
        cout << "5.DISPLAY ALL RECORDS" << endl;
        cout << "6.EXIT" << endl;

        cout << "Select option " << endl;
        cin >> option;

        switch (option)
        {
        case 1:
            AddRecord();
            break;
        case 2:
            cin.ignore();
            cout << "Enter ID of the record " << endl;
            getline(cin,empID);
            cout<< "Enter new Employee Name " <<endl;
            getline(cin,newEmpName);
            UpdateRecord(empID,newEmpName);
            break;
        case 3:
            cin.ignore();
            cout<<"Enter ID to delete"<<endl;
            getline(cin,empID);
            DeleteRecord(empID);
            break;
        case 4:
            cout << " Search By ID " << endl;
            cin >> empID;
            SearchRecord(empID);
            break;
        case 5:
            DisplayAllRecords();
            break;
        default:
            break;
        }

    } while (option != 6);

    saveToFIle();

    cout<<"Contents saved to the file "<<endl;
    return 0;
}
