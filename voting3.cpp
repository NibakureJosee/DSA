#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stack>

#include <unordered_map>

using namespace std;

// Caesar cipher function
string caesarCipher(string text, int shift)
{
    string result = "";
    for (int i = 0; i < text.length(); i++)
    {
        if (isupper(text[i]))
            result += char(int(text[i] + shift - 65) % 26 + 65);
        else
            result += char(int(text[i] + shift - 97) % 26 + 97);
    }
    return result;
}
void line(char ch)
{
    for (int i = 0; i < 65; i++)
    {
        cout << ch;
    }
    cout << endl;
}
void header()
{
    system("cls");
    line('=');
    cout << "\t\t      SMART VOTING SYSTEM\n";
    line('=');
}

struct QueueNode
{
    string voterID;
    QueueNode *next;

    QueueNode(string id) : voterID(id), next(nullptr) {}
};

class Voter
{
public:
    string voterID;
    string name;
    string password;
    string location;
    int locationStatus;
    bool voteStatus;

    Voter() : voterID(""), name(""), password(""), location(""), locationStatus(0), voteStatus(0) {}
    Voter(string id, string name, string password, string location, int locationStatus, bool voteStatus) : voterID(id), name(name), password(password), location(location), locationStatus(locationStatus), voteStatus(voteStatus) {}
};

class Candidate
{
public:
    string candidateID;
    string c_name;
    string c_location;
    Candidate() : candidateID(""), c_name(""), c_location("") {}
    Candidate(string id, string name, string location) : candidateID(id), c_name(name), c_location(location) {}
};

class VotingQueue
{
private:
    QueueNode *front;
    QueueNode *rear;

public:
    VotingQueue() : front(NULL), rear(NULL) {}

    bool isEmpty() const
    {
        return front == NULL;
    }

    void enqueue(string id)
    {
        QueueNode *newNode = new QueueNode(id);
        if (isEmpty())
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }

    string dequeue()
    {
        if (isEmpty())
        {
            cerr << "Error: Queue is empty." << endl;
            return "";
        }

        QueueNode *temp = front;
        string id = temp->voterID;

        if (front == rear)
        {
            front = rear = NULL;
        }
        else
        {
            front = front->next;
        }

        delete temp;
        return id;
    }
};
int votingAllowed = 0;
class AdminInterface
{
private:
    unordered_map<string, Voter> voters;

public:
    bool voterExists(string id)
    {
        return voters.find(id) != voters.end();
    }
    void loadVoters()
    {
        ifstream file("eligible_voters.csv");
        string line, id, name, password, location;
        string strLocationStatus = "0";
        string strVoteStatus = "0";
        int locationStatus;
        bool voteStatus;
        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, password, ',');
            getline(ss, location, ',');
            getline(ss, strLocationStatus, ',');
            getline(ss, strVoteStatus, ',');
            locationStatus = stoi(strLocationStatus);
            voteStatus = (strVoteStatus == "0" ? 0 : 1);

            voters[id] = Voter(id, name, password, location, locationStatus, voteStatus);
        }
        file.close();
    }
    void addVoter(string id)
    {
        string name, password, location;
        cout << "Voter's name: ";
        cin >> name;
        cout << "Voter password: ";
        cin >> password;
        cout << "Voter location: ";
        location = this->location();
        // Encrypt the password using Caesar cipher
        string encryptedPassword = caesarCipher(password, 3);
        int locationStatus = 0;
        bool voteStatus = 0;
        voters[id] = Voter(id, name, encryptedPassword, location, locationStatus, voteStatus);

        // Save the new voter to file
        ofstream file;
        file.open("eligible_voters.csv", ios::app); // Open the file in append mode
        file << id << "," << name << "," << encryptedPassword << "," << location << ","
             << locationStatus << "," << voteStatus << "\n";
        file.close();
    }

    string location()
    {
        string loc;
        int choice;
        do
        {
            cout << "\nChoose City: \n\n\t[1] Karachi \n\t[2] Islamabad \n\t[3] Lahore \n\t[4] Multan \n\t[5] Hyderabad \n\nChoice: ";
            cin >> choice;
            if (choice == 1)
            {
                return "Karachi";
            }

            else if (choice == 2)
            {
                return "Islamabad";
            }

            else if (choice == 3)
            {
                return "Lahore";
            }

            else if (choice == 4)
            {
                return "Multan";
            }

            else if (choice == 5)
            {
                return "Hyderabad";
            }

            else
            {
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice < 1 || choice > 5);
    }
    void editInfo()
    {
        string id;
        cout << "Enter voter ID: ";
        cin >> id;

        ifstream file("eligible_voters.csv");
        ofstream temp("temp.csv");
        string line, voterID, name, password, location;
        string strLocationStatus = "0";
        string strVoteStatus = "0";
        bool found = false;
        int locationStatus;
        bool voteStatus;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, voterID, ',');
            getline(ss, name, ',');
            getline(ss, password, ',');
            getline(ss, location, ',');
            getline(ss, strLocationStatus, ',');
            getline(ss, strVoteStatus, ',');
            locationStatus = stoi(strLocationStatus);
            voteStatus = (strVoteStatus == "1" ? true : false);
            if (voterID == id)
            {
                found = true;
                cout << "What do you want to edit? \n\t[1] Name \n\t[2] Password \n\t[3] Location \nChoice: ";
                int choice;
                cin >> choice;

                if (choice == 1)
                {
                    cout << "Enter new name: ";
                    cin >> name;
                }
                else if (choice == 2)
                {
                    cout << "Enter new password: ";
                    cin >> password;
                    string encryptedPassword = caesarCipher(password, 3);

                    // Update the password
                    voters[id].password = encryptedPassword;
                    ofstream outFile("eligible_voters.csv");

                    for (auto &pair : voters)
                    {
                        Voter &voter = pair.second;
                        outFile << pair.first << "," << voter.name << "," << voter.password << "," << voter.location << "\n";
                    }

                    outFile.close();
                }
                else if (choice == 3)
                {
                    location = this->location();
                }
                else
                {
                    cout << "Invalid choice.\n";
                    return;
                }
            }

            temp << voterID << "," << name << "," << password << "," << location << "\n";
        }

        file.close();
        temp.close();

        if (found)
        {
            remove("eligible_voters.csv");
            rename("temp.csv", "eligible_voters.csv");
        }
        else
        {
            cout << "User not found.\n";
            remove("temp.csv");
        }
    }
    void saveVoters()
    {
        ofstream file("eligible_voters.csv");
        for (const auto &pair : voters)
        {
            const Voter &voter = pair.second;

            file << voter.voterID << "," << voter.name << "," << voter.password << "," << voter.location << "," << voter.locationStatus << "," << voter.voteStatus << "\n";
        }
        file.close();
    }
    void checkNotifications()
    {
        this->loadVoters();
        for (auto &voter : voters)
        {
            if (voter.second.locationStatus != 0)
            {
                cout << "Voter with ID " << voter.second.voterID << " has requested for change in location.\n";
                switch (voter.second.locationStatus)
                {
                case 1:
                    cout << "Requested location: Karachi\n";
                    break;
                case 2:
                    cout << "Requested location: Islamabad\n";
                    break;
                case 3:
                    cout << "Requested location: Lahore\n";
                    break;
                case 4:
                    cout << "Requested location: Multan\n";
                    break;
                case 5:
                    cout << "Requested location: Hyderabad\n";
                    break;
                default:
                    cout << "Invalid location status.\n";
                    break;
                }
                cout << "\nDo you want to approve this request? \n\t[1] Yes \n\t[2] No \nChoice: ";
                int choice;
                do
                {
                    cout << "Enter choice: ";
                    cin >> choice;
                    if (choice == 1)
                    {
                        switch (voter.second.locationStatus)
                        {
                        case 1:
                            voter.second.location = "Karachi";
                            voter.second.locationStatus = 0;
                            this->saveVoters();
                        case 2:
                            voter.second.location = "Islamabad";
                            voter.second.locationStatus = 0;
                            this->saveVoters();
                            break;
                        case 3:
                            voter.second.location = "Lahore";
                            voter.second.locationStatus = 0;
                            this->saveVoters();
                            break;
                        case 4:
                            voter.second.location = "Multan";
                            voter.second.locationStatus = 0;
                            this->saveVoters();
                            break;
                        case 5:
                            voter.second.location = "Hyderabad";
                            voter.second.locationStatus = 0;
                            this->saveVoters();
                            break;
                        default:
                            cout << "Invalid location status.\n";
                            this->saveVoters();
                            break;
                        }
                    }
                    else if (choice == 2)
                    {
                        cout << "Location change request rejected.\n";
                        voter.second.locationStatus = 0;
                    }
                    else
                    {
                        cout << "Invalid choice. Please try again.\n";
                    }
                } while (choice != 1 && choice != 2);
            }
            else
            {
                cout << "No notifications.\n";
            }
        }
    }
    void releaseVotes(VotingQueue &voteQueue)
    {
        cout << "Processing votes...\n";

        // Process the votes from the queue
        while (!voteQueue.isEmpty())
        {
            string voterID = voteQueue.dequeue();

            // Ensure the voter still exists
            if (voters.find(voterID) != voters.end())
            {
                // Update the vote status
                voters[voterID].voteStatus = true;
            }
        }

        cout << "Votes have been processed.\n";
    }
    unordered_map<string, int> countVotes()
    {
        unordered_map<string, int> candidateVotes;

        ifstream file("voter_votes.csv");
        string line, voterID, candidateID;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, voterID, ',');
            getline(ss, candidateID, ',');

            // Count the vote for the candidate
            candidateVotes[candidateID]++;
        }

        file.close();
        return candidateVotes;
    }

    void displayResults()
    {
        unordered_map<string, int> candidateVotes = countVotes();

        cout << "Election Results:\n";

        for (const auto &candidate : candidateVotes)
        {
            cout << "Candidate ID: " << candidate.first << "\tVotes: " << candidate.second << endl;
        }
    }
};
class VotersInterface
{
private:
    unordered_map<string, Voter> voters;

    unordered_map<string, Candidate> candidates;

public:
    bool voterExists(string id)
    {
        return voters.find(id) != voters.end();
    }
    string getPassword(string id)
    {
        if (voterExists(id))
        {
            return voters[id].password;
        }
        else
        {
            return "0";
        }
    }
    void loadVoters()
    {
        ifstream file("eligible_voters.csv");
        string line, id, name, password, location;
        string strLocationStatus = "0";
        string strVoteStatus = "0";
        int locationStatus;
        bool voteStatus;
        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, password, ',');
            getline(ss, location, ',');
            getline(ss, strLocationStatus, ',');
            getline(ss, strVoteStatus, ',');

            int locationStatus = stoi(strLocationStatus);
            bool voteStatus = (strVoteStatus == "1" ? true : false);
            voters[id] = Voter(id, name, password, location, locationStatus, voteStatus);
        }
        file.close();
    }
    void saveVoters()
    {
        ofstream file("eligible_voters.csv");
        for (const auto &pair : voters)
        {
            const Voter &voter = pair.second;

            file << voter.voterID << "," << voter.name << "," << voter.password << "," << voter.location << "," << voter.locationStatus << "," << voter.voteStatus << "\n";
        }
        file.close();
    }

    void changePassword(string id)
    {
        if (voterExists(id))
        {
            cout << "Enter current password: ";
            string currentPassword;
            cin >> currentPassword;

            // Encrypt the entered password for comparison
            string encryptedCurrentPassword = caesarCipher(currentPassword, 3);

            if (voters[id].password == encryptedCurrentPassword)
            {
                cout << "Enter new password: ";
                string newPassword;
                cin >> newPassword;

                // Encrypt the new password before updating
                string encryptedNewPassword = caesarCipher(newPassword, 3);

                // Update the password
                voters[id].password = encryptedNewPassword;

                cout << "Password changed successfully!\n";
            }
            else
            {
                cout << "Incorrect current password.\n";
            }
        }
        else
        {
            cout << "Invalid voter ID.\n";
        }
    }

    void seeLocation(string id)
    {
        // this->loadVoters();
        if (voters.find(id) != voters.end())
        {
            cout << "\nThe location of the voter with ID " << id << " is " << voters[id].location << ".\n";
            cout << endl
                 << "Do you want to request for change in location? \n\n\t[1] Yes \n\t[2] No \n";
            int choice;

            do
            {
                cout << "Choice: ";
                cin >> choice;
                if (choice == 1)
                {
                    cout << "Where do you want to change your location? \n\t[1] Karachi \n\t[2] Islamabad \n\t[3] Lahore \n\t[4] Multan \n\t[5] Hyderabad \n\t[6] Do not change \n";
                    int choice;
                    do
                    {
                        cout << "Choice: ";
                        cin >> choice;
                        if (choice == 1)
                        {
                            voters[id].locationStatus = 1;
                            this->saveVoters();
                        }
                        else if (choice == 2)
                        {
                            voters[id].locationStatus = 2;
                            this->saveVoters();
                        }
                        else if (choice == 3)
                        {
                            voters[id].locationStatus = 3;
                            this->saveVoters();
                        }
                        else if (choice == 4)
                        {
                            voters[id].locationStatus = 4;
                            this->saveVoters();
                        }
                        else if (choice == 5)
                        {
                            voters[id].locationStatus = 5;
                            this->saveVoters();
                        }
                        else if (choice == 6)
                        {
                            cout << "Location change request cancelled.\n";
                            break;
                        }
                        else
                        {
                            cout << "Invalid choice. Please try again.\n";
                        }
                    } while (choice < 1 || choice > 6);
                }
                else if (choice == 2)
                {
                    break;
                }

                else
                {
                    cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 1 && choice != 2);
        }
        else
        {
            cout << "No voter found with ID " << id << ".\n";
        }
    }
    void loadCandidates()
    {
        ifstream file("eligible_candidates.csv");
        string line, id, name, location;

        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, location, ',');

            candidates[id] = Candidate(id, name, location);
        }

        file.close();
    }
    void displayCandidateDetails()
    {
        loadCandidates();

        cout << "Candidate Details:\n";

        for (const auto &candidate : candidates)
        {
            cout << "ID: " << candidate.second.candidateID << "\nName: " << candidate.second.c_name << "\nLocation: " << candidate.second.c_location << "\n\n";
        }
    }
    void vote(VotingQueue &voteQueue)
    {
        if (votingAllowed == 1)
        {
            string voterID;
            cout << "Enter your voter ID: ";
            cin >> voterID;

            if (voterExists(voterID))
            {
                if (!voters[voterID].voteStatus)
                {
                    cout << "Candidates available for voting:\n";
                    displayCandidateDetails();

                    cout << "Enter the ID of the candidate you want to vote for: ";
                    string candidateID;
                    cin >> candidateID;

                    // Check if the candidate exists
                    if (candidates.find(candidateID) != candidates.end())
                    {
                        // Enqueue the vote for processing later
                        voteQueue.enqueue(voterID);

                        cout << "Vote recorded. It will be processed later.\n";
                        // Record the vote in the CSV file
                        ofstream outFile("voter_votes.csv", ios::app);
                        outFile << voterID << "," << candidateID << "\n";
                        outFile.close();
                    }
                    else
                    {
                        cout << "Invalid candidate ID.\n";
                    }
                }
                else
                {
                    cout << "You have already voted.\n";
                }
            }
            else
            {
                cout << "Invalid voter ID.\n";
            }
        }
        else
        {
            cout << votingAllowed << endl;
            cout << "Voting is not allowed at the moment.\n";
        }
    }
};

class VotingSystem
{
private:
    AdminInterface adminInterface;
    VotersInterface votersInterface;
    VotingQueue voteQueue;

public:
    void welcomeAdmin()
    {
        system("cls");
        header();
        cout << "\t\t        WELCOME, ADMIN!\n";
        line('-');
    }
    void adminChoice()
    {
        int choice;
        do
        {
            cout << endl
                 << "Press\n\t[0] to go to Admin's Menu!" << endl
                 << "\tChoice: ";

            cin >> choice;
            if (choice == 0)
            {
                adminMethod();
            }

            else if (choice == 1)
            {
                break;
            }

            else
            {
                cout << "Invalid choice.\n";
            }

        } while (choice != 0 && choice != 1);
    }

    void adminMethod()
    {
        while (true)
        {
            welcomeAdmin();
            cout << "What do you what to do: \n\n\t[1] Allow Voting \n\t[2] Add Voter \n\t[3] Edit Voter's Info\n\t[4] Check Notifications \n\t[5] Release Votes \n\t[6] Go Back \n\nChoice: ";
            int choice;
            cin >> choice;
            if (choice == 1)
            {
                if (votingAllowed)
                {
                    cout << "Voting is already allowed.\n";
                    adminChoice();
                }
                else
                {
                    votingAllowed = 1;

                    cout << "Voting is now allowed.\n";
                    adminChoice();
                }
            }

            else if (choice == 2)
            {
                welcomeAdmin();
                string id;
                cout << "Enter voter ID: ";
                cin >> id;

                // Load voters from file
                adminInterface.loadVoters();

                // Check if voter with entered ID exists
                if (adminInterface.voterExists(id))
                {
                    cout << "This ID already exists.\n";
                    adminChoice();
                }
                else
                {
                    adminInterface.addVoter(id);
                    cout << "Voter added successfully.\n";
                    adminChoice();
                }
            }
            else if (choice == 3)
            {
                adminInterface.editInfo();
                cout << "Voter's info edited successfully.\n";
                adminChoice();
            }
            else if (choice == 4)
            {
                adminInterface.checkNotifications();
                adminChoice();
            }
            else if (choice == 5)
            {
                // ------------RELEASE VOTE FUNCTIONALITY---------------

                if (votingAllowed)
                {
                    adminInterface.releaseVotes(voteQueue);
                    adminChoice();
                }
                else
                {
                    cout << "Since voting is not allowed, you cannot release results.\n";
                    adminChoice();
                }
            }
            else if (choice == 6)
            {
                start();
                break;
            }
            else
            {
                cout << "Invalid choice.\n";
                adminChoice();
            }
        }
    }

    void voterChoice(string id)
    {
        int choice;
        do
        {
            cout << endl
                 << "Press\n\t[0] to go to Voters's Menu\n "
                 << "\tChoice: ";
            cin >> choice;
            if (choice == 0)
            {
                voterMethod(id);
            }

            else
            {
                cout << "Invalid choice.\n";
            }

        } while (choice != 0 && choice != 1);
    }

    void welcomeVoter()
    {
        system("cls");
        header();
        cout << "\t\t         WELCOME, VOTER!\n";
        line('-');
    }

    void voterMethod(string id)
    {
        int choice;
        do
        {
            welcomeVoter();
            cout << "What do you want to do?\n\t[1] See location\n\t[2] Change password\n\t[3] Vote\n\t[4] Go back\nChoice: ";
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                welcomeVoter();
                votersInterface.seeLocation(id);
                cout << "Location change request sent.\nLocation will be changed after approval from admin.\n";
                voterChoice(id);
                break;
            case 2:
                welcomeVoter();
                votersInterface.changePassword(id);
                voterChoice(id);
                break;
            case 3:
                welcomeVoter();
                // Code to vote...
                if (votingAllowed)
                {
                    votersInterface.vote(voteQueue);
                    voterChoice(id);
                }
                else
                {
                    cout << "Voting is not allowed at the moment.\n";
                    voterChoice(id);
                }
                break;
            case 4:
                start();
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        } while (choice < 1 || choice > 4);
    }

    void start()
    {
        while (true)
        {
            header();
            cout << "\nWhat is your status? \n\n\t[1] Admin \n\t[2] Voter \n\nStatus: ";
            int status;
            cin >> status;
            line('-');

            if (status == 1)
            {
                welcomeAdmin();

                cout << endl;
                cout << "User: ADMIN" << endl;

                string password;
                cout << "Password: ";
                cin >> password;

                if (password == "admin")
                {
                    adminMethod();
                }

                else
                {
                    cout << "Invalid password.\n";
                }
            }
            else if (status == 2)
            {
                cout << "Enter voter ID: ";
                string id;
                cin >> id;
                votersInterface.loadVoters();

                if (votersInterface.voterExists(id))
                {
                    int attempts = 3;
                    while (attempts > 0)
                    {
                        string password;
                        cout << "Enter Password (Attempts left: " << attempts << "): ";
                        cin >> password;

                        // Encrypt the password
                        string encryptedPassword = caesarCipher(password, 3);

                        // Get the voter's password from the hash table using a public getter function
                        string storedPassword = votersInterface.getPassword(id);

                        // Compare the encrypted password with the stored password
                        if (encryptedPassword == storedPassword)
                        {
                            cout << "Password is correct.\n";
                            voterMethod(id);
                            break;
                        }
                        else
                        {
                            cout << "Incorrect password. Please try again.\n";
                            attempts--;
                        }
                    }

                    if (attempts == 0)
                    {
                        cout << "You have exceeded the maximum number of attempts. Exiting...\n";
                        break;
                        return;
                    }
                }
                else
                {
                    cout << "Invalid status.\n";
                }
            }
        }
    }
};

int main()
{
    VotingSystem votingSystem;
    votingSystem.start();
    return 0;
}
