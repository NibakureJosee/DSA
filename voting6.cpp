#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

// Struct to represent a user
struct User {
    std::string username;
    std::string password;
    bool isAdmin;
};

// Struct to represent a poll/candidate
struct Candidate {
    int id;
    std::string name;
    int votes;
};

// Class to represent the voting system
class VotingApp {
private:
    std::vector<User> users;
    std::vector<Candidate> candidates;
    std::unordered_map<std::string, int> userVotes; // Track user votes by username and candidate ID
    int candidateIdCounter = 0;

    const std::string usersFile = "users.txt";
    const std::string candidatesFile = "candidates.txt";

    bool isUsernameTaken(const std::string& username) {
        for (const auto& user : users) {
            if (user.username == username) {
                return true;
            }
        }
        return false;
    }

    User* findUser(const std::string& username, const std::string& password) {
        for (auto& user : users) {
            if (user.username == username && user.password == password) {
                return &user;
            }
        }
        return nullptr;
    }

    void loadUsers() {
        std::ifstream file(usersFile);
        if (!file) {
            std::cerr << "Error: Failed to open " << usersFile << "\n";
            return;
        }

        std::string username, password, isAdminStr;
        bool isAdmin;
        while (file >> username >> password >> isAdminStr) {
            isAdmin = (isAdminStr == "1");
            users.push_back({username, password, isAdmin});
        }
        file.close();
    }

    void saveUsers() {
        std::ofstream file(usersFile);
        if (!file) {
            std::cerr << "Error: Failed to open " << usersFile << " for writing\n";
            return;
        }

        for (const auto& user : users) {
            file << user.username << " " << user.password << " " << (user.isAdmin ? "1" : "0") << "\n";
        }
        file.close();
    }

    void loadCandidates() {
        std::ifstream file(candidatesFile);
        if (!file) {
            std::cerr << "Error: Failed to open " << candidatesFile << "\n";
            return;
        }

        std::string name;
        int id, votes;
        while (file >> id >> name >> votes) {
            candidates.push_back({id, name, votes});
            if (id >= candidateIdCounter) {
                candidateIdCounter = id + 1; // Set counter to next available ID
            }
        }
        file.close();
    }

    void saveCandidates() {
        std::ofstream file(candidatesFile);
        if (!file) {
            std::cerr << "Error: Failed to open " << candidatesFile << " for writing\n";
            return;
        }

        for (const auto& candidate : candidates) {
            file << candidate.id << " " << candidate.name << " " << candidate.votes << "\n";
        }
        file.close();
    }

    void displayMenu() {
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "3. Exit\n";
    }

    void handleRegister() {
        std::string username, password;
        bool isAdmin;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;
        std::cout << "Is admin (1 for yes, 0 for no): ";
        std::cin >> isAdmin;

        if (isUsernameTaken(username)) {
            std::cout << "Username already exists.\n";
            return;
        }

        users.push_back({username, password, isAdmin});
        saveUsers();
        std::cout << "Registration successful.\n";
    }

    void handleLogin() {
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        loadUsers(); // Reload users from file (in case changes were made)
        User* user = findUser(username, password);
        if (user) {
            std::cout << "Login successful.\n";
            if (user->isAdmin) {
                handleAdminFunctions(user);
            } else {
                handleUserFunctions(user);
            }
        } else {
            std::cout << "Invalid username or password.\n";
        }
    }

    void handleAdminFunctions(User* user) {
        while (true) {
            std::cout << "Admin Menu:\n";
            std::cout << "1. Create Candidate\n";
            std::cout << "2. List Candidates\n";
            std::cout << "3. View Results\n";
            std::cout << "4. Logout\n";
            int choice;
            std::cin >> choice;
            switch (choice) {
                case 1:
                    createCandidate();
                    break;
                case 2:
                    listCandidates();
                    break;
                case 3:
                    viewResults();
                    break;
                case 4:
                    return;
                default:
                    std::cout << "Invalid choice.\n";
            }
        }
    }

    void handleUserFunctions(User* user) {
        while (true) {
            std::cout << "User Menu:\n";
            std::cout << "1. List Candidates\n";
            std::cout << "2. Vote\n";
            std::cout << "3. Logout\n";
            int choice;
            std::cin >> choice;
            switch (choice) {
                case 1:
                    listCandidates();
                    break;
                case 2:
                    castVote(user);
                    break;
                case 3:
                    return;
                default:
                    std::cout << "Invalid choice.\n";
            }
        }
    }

    void createCandidate() {
        std::string name;
        std::cout << "Enter candidate name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        candidates.push_back({candidateIdCounter++, name, 0});
        saveCandidates();
        std::cout << "Candidate created successfully.\n";
    }

    void listCandidates() {
        std::cout << "Candidates:\n";
        for (const auto& candidate : candidates) {
            std::cout << "ID: " << candidate.id << " Name: " << candidate.name << " Votes: " << candidate.votes << "\n";
        }
    }

    void castVote(User* user) {
        int candidateId;
        std::cout << "Enter candidate ID to vote for: ";
        std::cin >> candidateId;

        if (userVotes.find(user->username) != userVotes.end()) {
            std::cout << "You have already voted.\n";
            return;
        }

        for (auto& candidate : candidates) {
            if (candidate.id == candidateId) {
                candidate.votes++;
                userVotes[user->username] = candidateId;
                saveCandidates(); // Update votes count in file
                std::cout << "Vote cast successfully.\n";
                return;
            }
        }

        std::cout << "Invalid candidate ID.\n";
    }

    void viewResults() {
        std::cout << "Results:\n";
        for (const auto& candidate : candidates) {
            std::cout << "Name: " << candidate.name << " Votes: " << candidate.votes << "\n";
        }
    }

public:
    VotingApp() {
        loadUsers();
        loadCandidates();
    }

    void run() {
        while (true) {
            displayMenu();
            int choice;
            std::cin >> choice;
            switch (choice) {
                case 1:
                    handleRegister();
                    break;
                case 2:
                    handleLogin();
                    break;
                case 3:
                    return;
                default:
                    std::cout << "Invalid choice.\n";
            }
        }
    }
};

int main() {
    VotingApp app;
    app.run();
    return 0;
}

