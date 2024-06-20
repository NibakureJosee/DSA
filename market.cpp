#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Product class to represent a product
class Product {
private:
    int id;
    std::string name;
    double price;
    int quantity;

public:
    // Constructor to initialize a product
    Product(int id, const std::string& name, double price, int quantity)
        : id(id), name(name), price(price), quantity(quantity) {}

    // Getter methods for private member variables
    int getId() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    void setQuantity(int newQuantity) { quantity = newQuantity; } // Setter method for quantity
};

// ProductList class to manage a list of products using vector
class ProductList {
private:
    std::vector<Product> products; // Vector to store products

public:
    // Function to add a product to the list
    void addProduct(const Product& product) {
        products.push_back(product);
    }

    // Function to remove a product by ID
    void removeProduct(int productId) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->getId() == productId) {
                products.erase(it);
                std::cout << "Product removed successfully." << std::endl;
                return;
            }
        }
        std::cout << "Product not found." << std::endl;
    }

    // Function to find a product by ID
    Product* findProduct(int productId) {
        for (auto& product : products) {
            if (product.getId() == productId) {
                return &product;
            }
        }
        return nullptr; // Product not found
    }

    // Function to display all products in the list
    void displayProducts() const {
        std::cout << "Product List:" << std::endl;
        for (const auto& product : products) {
            std::cout << product.getId() << ". "
                      << product.getName() << " - $"
                      << product.getPrice() << " (Quantity: "
                      << product.getQuantity() << ")" << std::endl;
        }
    }

    // Function to get reference to all products in the list
    std::vector<Product>& getAllProducts() {
        return products;
    }
};

// User class to represent a user
class User {
private:
    std::string username;
    std::string password;
    std::string role; // 'admin' or 'buyer'

public:
    // Constructor to initialize a user
    User(const std::string& username, const std::string& password, const std::string& role)
        : username(username), password(password), role(role) {}

    // Getter methods for private member variables
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getRole() const { return role; }
};

// FileHandler class for reading/writing users and products to/from files
class FileHandler {
private:
    std::string usersFile = "users.csv";       // File to store users
    std::string productsFile = "products.csv"; // File to store products

public:
    // Function to read users from file and return a vector of users
    std::vector<User> readUsers() {
        std::vector<User> users;
        std::ifstream file(usersFile);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << usersFile << std::endl;
            return users;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string username, password, role;
            // Extract username, password, and role from each line
            if (std::getline(iss, username, ',') &&
                std::getline(iss, password, ',') &&
                std::getline(iss, role, ',')) {
                users.emplace_back(username, password, role); // Add user to vector
            }
        }
        file.close();
        return users;
    }

    // Function to write users to file
    void writeUsers(const std::vector<User>& users) {
        std::ofstream file(usersFile);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << usersFile << std::endl;
            return;
        }

        // Write each user's data to file
        for (const auto& user : users) {
            file << user.getUsername() << ","
                 << user.getPassword() << ","
                 << user.getRole() << "\n";
        }
        file.close();
    }

    // Function to read products from file and return a vector of products
    std::vector<Product> readProducts() {
        std::vector<Product> products;
        std::ifstream file(productsFile);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << productsFile << std::endl;
            return products;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;
            // Tokenize each line by comma and store tokens in vector
            while (std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.size() == 4) {
                // Convert tokens to appropriate data types and create Product object
                int id = std::stoi(tokens[0]);
                std::string name = tokens[1];
                double price = std::stod(tokens[2]);
                int quantity = std::stoi(tokens[3]);
                products.emplace_back(id, name, price, quantity); // Add product to vector
            }
        }
        file.close();
        return products;
    }

    // Function to write products to file
    void writeProducts(const std::vector<Product>& products) {
        std::ofstream file(productsFile);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << productsFile << std::endl;
            return;
        }

        // Write each product's data to file
        for (const auto& product : products) {
            file << product.getId() << ","
                 << product.getName() << ","
                 << product.getPrice() << ","
                 << product.getQuantity() << "\n";
        }
        file.close();
    }
};

// AdminDashboard class for admin operations
class AdminDashboard {
private:
    ProductList productList; // Product list managed by admin
    FileHandler fileHandler; // File handler for reading/writing products

public:
    // Constructor to initialize admin dashboard with products
    AdminDashboard(const std::vector<Product>& products) {
        for (const auto& product : products) {
            productList.addProduct(product); // Add products to the admin's product list
        }
    }

    // Function to add a new product to the list and save to file
    void addProduct(int id, const std::string& name, double price, int quantity) {
        Product newProduct(id, name, price, quantity); // Create new product object
        productList.addProduct(newProduct); // Add product to list
        saveProductsToFile(); // Save updated products to file
    }

    // Function to remove a product by ID and save to file
    void removeProduct(int productId) {
        productList.removeProduct(productId); // Remove product from list
        saveProductsToFile(); // Save updated products to file
    }

    // Function to display all products in the list
    void displayProducts() {
        productList.displayProducts(); // Display all products in the list
    }

    // Function to save products to file
    void saveProductsToFile() {
        fileHandler.writeProducts(productList.getAllProducts()); // Write products to file
    }

    // Function to display admin menu and handle admin operations
    void adminMenu() {
        int choice;
        do {
            std::cout << "\nAdmin Menu:" << std::endl;
            std::cout << "1. Add Product" << std::endl;
            std::cout << "2. Remove Product" << std::endl;
            std::cout << "3. View Products" << std::endl;
            std::cout << "4. Logout" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            switch (choice) {
                case 1: {
                    int id, quantity;
                    double price;
                    std::string name;
                    std::cout << "Enter product ID: ";
                    std::cin >> id; // Get product ID
                    std::cin.ignore(); // Ignore newline character in buffer
                    std::cout << "Enter product name: ";
                    std::getline(std::cin, name); // Get product name
                    std::cout << "Enter product price: ";
                    std::cin >> price; // Get product price
                    std::cout << "Enter product quantity: ";
                    std::cin >> quantity; // Get product quantity
                    addProduct(id, name, price, quantity); // Add product using entered data
                    break;
                }
                case 2: {
                    int productId;
                    std::cout << "Enter product ID to remove: ";
                    std::cin >> productId; // Get product ID to remove
                    removeProduct(productId); // Remove product by ID
                    break;
                }
                case 3:
                    displayProducts(); // View all products available
                    break;
                case 4:
                    std::cout << "Logged out from Admin Dashboard." << std::endl; // Logout message
                    break;
                default:
                    std::cout << "Invalid choice. Please enter again." << std::endl;
                    break;
            }
        } while (choice != 4); // Repeat until admin chooses to logout
    }
};

// BuyerDashboard class for buyer operations
class BuyerDashboard {
private:
    ProductList productList; // Product list managed by buyer
    FileHandler fileHandler; // File handler for reading/writing products

public:
    // Constructor to initialize buyer dashboard with products
    BuyerDashboard(const std::vector<Product>& products) {
        for (const auto& product : products) {
            productList.addProduct(product); // Add products to the buyer's product list
        }
    }

    // Function to display all products available for purchase
    void displayProducts() {
        productList.displayProducts(); // Display all products in the list
    }

    // Function to purchase a product by ID and update quantity, then save to file
    void purchaseProduct(int productId, int quantity) {
        Product* product = productList.findProduct(productId); // Find product by ID
        if (!product) {
            std::cout << "Product not found." << std::endl; // Display message if product not found
            return;
        }

        if (product->getQuantity() < quantity) {
            std::cout << "Not enough quantity available." << std::endl; // Display message if quantity is insufficient
            return;
        }

        // Update product quantity after purchase
        product->setQuantity(product->getQuantity() - quantity);

        double amount = quantity * product->getPrice();
        std::cout << "Purchase successful. Amount to pay: $" << amount << std::endl; // Display success message

        saveProductsToFile(); // Save updated products to file
    }

    // Function to save products to file
    void saveProductsToFile() {
        fileHandler.writeProducts(productList.getAllProducts()); // Write products to file
    }

    // Function to display buyer menu and handle buyer operations
    void buyerMenu() {
        int choice;
        do {
            std::cout << "\nBuyer Menu:" << std::endl;
            std::cout << "1. View Products" << std::endl;
            std::cout << "2. Purchase Product" << std::endl;
            std::cout << "3. Logout" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            switch (choice) {
                case 1:
                    displayProducts(); // Display all products available
                    break;
                case 2: {
                    int productId, quantity;
                    std::cout << "Enter product ID to purchase: ";
                    std::cin >> productId; // Get product ID to purchase
                    std::cout << "Enter quantity to purchase: ";
                    std::cin >> quantity; // Get quantity to purchase
                    purchaseProduct(productId, quantity); // Purchase product using entered data
                    break;
                }
                case 3:
                    std::cout << "Logged out from Buyer Dashboard." << std::endl; // Logout message
                    break;
                default:
                    std::cout << "Invalid choice. Please enter again." << std::endl;
                    break;
            }
        } while (choice != 3); // Repeat until user chooses to logout
    }
};

// Main function to run the application
int main() {
    FileHandler fileHandler; // File handler for reading/writing products
    std::vector<Product> products = fileHandler.readProducts(); // Read products from file

    AdminDashboard adminDashboard(products); // Create AdminDashboard object with products
    BuyerDashboard buyerDashboard(products); // Create BuyerDashboard object with products

    std::vector<User> users = fileHandler.readUsers(); // Read users from file

    int userType;
    std::string username, password;
    bool loggedIn = false;

    do {
        std::cout << "\nUser Type:" << std::endl;
        std::cout << "1. Admin" << std::endl;
        std::cout << "2. Buyer" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> userType;

        switch (userType) {
            case 1:
                std::cout << "Enter username: ";
                std::cin >> username;
                std::cout << "Enter password: ";
                std::cin >> password;

                for (const auto& user : users) {
                    if (user.getUsername() == username && user.getPassword() == password && user.getRole() == "admin") {
                        adminDashboard.adminMenu();
                        loggedIn = true;
                        break;
                    }
                }

                if (!loggedIn) {
                    std::cout << "Invalid username or password." << std::endl;
                }
                break;
            case 2:
                buyerDashboard.buyerMenu();
                break;
            case 3:
                std::cout << "Exiting the program." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please enter again." << std::endl;
                break;
        }
    } while (userType != 3);

    return 0;
}

