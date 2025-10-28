#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

struct Order {
    string id;
    string name;
    string item;
    int quantity;
    double price;
    string status;
};

void loadOrders(vector<Order>& orders) {
    ifstream fin("orders.txt");
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, item, qty, price, total, status;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, item, ',');
        getline(ss, qty, ',');
        getline(ss, price, ',');
        getline(ss, total, ',');
        getline(ss, status, ',');
        Order o;
        o.id = id;
        o.name = name;
        o.item = item;
        o.quantity = qty.empty() ? 0 : stoi(qty);
        o.price = price.empty() ? 0.0 : stod(price);
        o.status = status.empty() ? "Pending" : status;
        orders.push_back(o);
    }
}

void saveOrders(const vector<Order>& orders) {
    ofstream fout("orders.txt");
    for (const auto& o : orders) {
        double t = o.quantity * o.price;
        fout << o.id << "," << o.name << "," << o.item << ","
            << o.quantity << "," << fixed << showpoint << setprecision(2) << o.price << ","
            << fixed << showpoint << setprecision(2) << t << ","
            << o.status << "\n";
    }
}

void placeOrder(vector<Order>& orders) {
    Order o;
    cout << "Enter Order ID: ";
    cin >> o.id;
    cout << "Enter customer name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, o.name);
    cout << "Enter menu item: ";
    getline(cin, o.item);
    cout << "Enter quantity: ";
    cin >> o.quantity;
    cout << "Enter price per item: ";
    cin >> o.price;
    o.status = "Pending";
    orders.push_back(o);
    cout << "Order added!\n";
}

void displayOrders(const vector<Order>& orders) {
    cout << "=========== CURRENT ORDERS ===========\n";
    cout << left << setw(8) << "ID"
        << setw(10) << "Name"
        << setw(10) << "Item"
        << right << setw(5) << "Qty"
        << setw(8) << "Price"
        << setw(9) << "Total"
        << setw(10) << "Status" << "\n";
    cout << "---------------------------------------------------\n";
    for (const auto& o : orders) {
        double t = o.quantity * o.price;
        cout << left << setw(8) << o.id
            << setw(10) << o.name.substr(0, 9)
            << setw(10) << o.item.substr(0, 9)
            << right << setw(5) << o.quantity
            << setw(8) << fixed << showpoint << setprecision(2) << o.price
            << setw(9) << fixed << showpoint << setprecision(2) << t
            << setw(10) << o.status << "\n";
    }
}

void markServed(vector<Order>& orders) {
    string target;
    cout << "Enter Order ID to mark as served: ";
    cin >> target;
    for (auto& o : orders) {
        if (o.id == target) {
            if (o.status == "Pending") {
                o.status = "Served";
                cout << "Order " << target << " marked as Served.\n";
            }
            else {
                cout << "Order " << target << " is already " << o.status << ".\n";
            }
            return;
        }
    }
    cout << "Order not found.\n";
}

void salesSummary(const vector<Order>& orders) {
    int total = (int)orders.size();
    int served = 0;
    int pending = 0;
    double sales = 0.0;
    for (const auto& o : orders) {
        if (o.status == "Served") {
            served++;
            sales += o.quantity * o.price;
        }
        else {
            pending++;
        }
    }
    cout << "========== SALES SUMMARY ==========\n";
    cout << "Total Orders: " << total << "\n";
    cout << "Served: " << served << "\n";
    cout << "Pending: " << pending << "\n";
    cout << "Total Sales: $" << fixed << showpoint << setprecision(2) << sales << "\n";
    cout << "===================================\n";
}

int main() {
    vector<Order> orders;
    loadOrders(orders);
    int choice;
    do {
        cout << "\n****** Welcome to Restaurant Order Manager ******\n";
        cout << "1. Place New Order\n2. Display All Orders\n3. Mark Order as Served\n4. View Sales Summary\n5. Exit\n\u2192 ";
        cin >> choice;
        switch (choice) {
        case 1: placeOrder(orders); break;
        case 2: displayOrders(orders); break;
        case 3: markServed(orders); break;
        case 4: salesSummary(orders); break;
        case 5: saveOrders(orders);
            cout << "Orders saved to orders.txt. Have a great day!\n";
            break;
        default: cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);
    return 0;
}
