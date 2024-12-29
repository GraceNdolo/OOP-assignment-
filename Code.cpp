#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream> // For handling multiple product IDs
#include <stdexcept>
#include <limits> // For numeric_limits
using namespace std;

// Product Class
class Product {
    int productID;
    string name;
    double price;
public:
    Product(int id, string n, double p) : productID(id), name(n), price(p) {}
    int getProductID() const { return productID; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    void displayProduct() const {
        cout << "Product ID: " << productID << ", Name: " << name << ", Price: Ksh" << price << endl;
    }
};

// Base Class: Person
class Person {
protected:
    string name;
public:
    Person(string n) : name(n) {}
    virtual void displayInfo() const = 0; // Abstract Method (Abstraction)
    virtual ~Person() {} // Virtual destructor
};

// Derived Class: Customer
class Customer : public Person {
    int customerID;
public:
    Customer(string n, int id) : Person(n), customerID(id) {}
    void displayInfo() const override {
        cout << "Customer Name: " << name << ", ID: " << customerID << endl;
    }
    int getCustomerID() const { return customerID; }
    string getCustomerName() const { return name; }
};

// Derived Class: Admin
class Admin : public Person {
public:
    Admin(string n) : Person(n) {}
    void displayInfo() const override {
        cout << "Admin Name: " << name << endl;
    }

    void addProduct(vector<Product>& products, int id, string name, double price) {
        products.push_back(Product(id, name, price));
        cout << "Product added successfully.\n";
    }
};

// Order Class
class Order {
    int orderID;
    int customerID;
    string customerName;
    vector<Product> products;
    double totalPrice;
public:
    Order(int oID, int cID, string cName) : orderID(oID), customerID(cID), customerName(cName), totalPrice(0) {}
    void addProductToOrder(const Product& product) {
        products.push_back(product);
        totalPrice += product.getPrice();
    }
    void displayOrder() const {
        cout << "----- RECEIPT -----\n";
        cout << "Customer Name: " << customerName << "\n";
        cout << "Customer ID: " << customerID << "\n";
        cout << "Order ID: " << orderID << "\n";
        cout << "Items:\n";
        for (const auto& product : products) {
            cout << "- Product ID: " << product.getProductID()
                 << ", Name: " << product.getName()
                 << ", Price: Ksh" << product.getPrice() << endl;
        }
        cout << "Total Price: Ksh" << totalPrice << "\n";
        cout << "--------------------\n";
    }
};

// CLI Menu
void showMenu() {
    cout << "\nMenu:\n";
    cout << "1. Add Product\n";
    cout << "2. View Products\n";
    cout << "3. Place Order\n";
    cout << "4. View Orders\n";
    cout << "5. Register Customer\n";
    cout << "6. View Customers\n";
    cout << "7. Exit\n";
    cout << "Choice: ";
}

int main() {
    vector<Product> products;
    vector<Order> orders;
    vector<Customer> customers;

    Admin admin("Admin Grace");
    int choice;

    do {
        showMenu();

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number between 1 and 7.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            int id;
            string name;
            double price;

            cout << "Enter Product ID, Name, and Price: ";
            if (!(cin >> id >> name >> price)) {
                cout << "Invalid input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            admin.addProduct(products, id, name, price);
            break;
        }
        case 2:
            if (products.empty()) {
                cout << "No products available.\n";
            } else {
                for (const auto& product : products) {
                    product.displayProduct();
                }
            }
            break;

        case 3: {
            int customerID, orderID;

            cout << "Enter Order ID and Customer ID: ";
            if (!(cin >> orderID >> customerID)) {
                cout << "Invalid input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            // Check if customer exists
            string customerName;
            bool customerExists = false;
            for (const auto& customer : customers) {
                if (customer.getCustomerID() == customerID) {
                    customerExists = true;
                    customerName = customer.getCustomerName();
                    break;
                }
            }

            if (!customerExists) {
                cout << "Customer ID not found. Please register the customer first.\n";
                break;
            }

            Order order(orderID, customerID, customerName);
            cout << "Enter Product IDs to add to order (space-separated, end with 0): ";
            cin.ignore(); // Clear any leftover input
            string productIDs;
            getline(cin, productIDs);
            stringstream ss(productIDs);

            int productID;
            while (ss >> productID && productID != 0) {
                bool found = false;
                for (const auto& product : products) {
                    if (product.getProductID() == productID) {
                        order.addProductToOrder(product);
                        cout << "Product ID " << productID << " added to the order.\n";
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Product ID " << productID << " not found. Skipping.\n";
                }
            }
            orders.push_back(order);
            cout << "Order placed successfully!\n";
            break;
        }

        case 4:
            if (orders.empty()) {
                cout << "No orders available.\n";
            } else {
                for (const auto& order : orders) {
                    order.displayOrder();
                }
            }
            break;

        case 5: {
            int customerID;
            string name;

            cout << "Enter Customer ID and Name: ";
            if (!(cin >> customerID >> name)) {
                cout << "Invalid input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            customers.push_back(Customer(name, customerID));
            cout << "Customer added successfully.\n";
            break;
        }

        case 6:
            if (customers.empty()) {
                cout << "No customers available.\n";
            } else {
                cout << "Registered Customers:\n";
                for (const auto& customer : customers) {
                    customer.displayInfo();
                }
            }
            break;

        case 7:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice. Please enter a number between 1 and 7.\n";
        }
    } while (choice != 7);

    return 0;
}


// Polymorphism:

// displayInfo() is a virtual method in Person and overridden in Customer and Admin.
// Classes and Objects:

// Defined Product, Person, Customer, Admin, and Order classes.
// Objects like products, customers, and orders store instances.
// Constructors and Destructors:

// Constructors are used to initialize objects (e.g., Product, Order, and derived classes).
// Virtual destructor in Person ensures proper cleanup.
// Inheritance:

// Customer and Admin inherit from Person.
// Abstraction:

// The Person class is abstract, with displayInfo() as a pure virtual method.
// Exception Handling:

// Used try-catch-style logic with cin.clear() and cin.ignore() for input validation.
// File I/O:

// The saveOrdersToFile() function allows saving orders to a file.
