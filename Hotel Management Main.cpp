// Hotel Management System (C++)
// Manages room bookings, customer details and billing. Data is stored in text files.
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

const string ROOMS_FILE = "rooms.txt";
const string CUSTOMERS_FILE = "customers.txt";
const double TAX_RATE = 0.12;

struct Room {
    int number;
    string type;
    double price;   // per night
    bool booked;
};

struct Customer {
    string name, phone;
    int roomNo;
    string checkIn;
    int nights;
    bool active;    // true = currently staying
};

class Hotel {
    vector<Room> rooms;
    vector<Customer> customers;

    static string readLine(const string& prompt) {
        string s;
        cout << prompt;
        getline(cin, s);
        return s;
    }
    static int readInt(const string& prompt) {
        while (true) {
            string s = readLine(prompt);
            try { size_t p; int v = stoi(s, &p); if (p == s.size()) return v; } catch (...) {}
            cout << "Invalid number, try again.\n";
        }
    }
    Room* findRoom(int no) {
        for (auto& r : rooms) if (r.number == no) return &r;
        return nullptr;
    }
    Customer* findActiveCustomerByRoom(int no) {
        for (auto& c : customers) if (c.active && c.roomNo == no) return &c;
        return nullptr;
    }

    void loadRooms() {
        ifstream f(ROOMS_FILE);
        Room r; int b;
        while (f >> r.number >> r.type >> r.price >> b) { r.booked = b; rooms.push_back(r); }
        if (rooms.empty()) {   // first run: default rooms
            rooms = {{101,"Single",1500,false},{102,"Double",2500,false},
                     {103,"Deluxe",4000,false},{104,"Suite",6000,false},
                     {105,"Single",1500,false},{106,"Double",2500,false}};
            saveRooms();
        }
    }
    void saveRooms() {
        ofstream f(ROOMS_FILE);
        for (auto& r : rooms) f << r.number << ' ' << r.type << ' ' << r.price << ' ' << r.booked << '\n';
    }
    void loadCustomers() {
        ifstream f(CUSTOMERS_FILE);
        string line;
        while (getline(f, line)) {
            stringstream ss(line);
            string part[6];
            bool ok = true;
            for (int i = 0; i < 6; i++) if (!getline(ss, part[i], '|')) { ok = false; break; }
            if (!ok) continue;
            try {
                customers.push_back({part[0], part[1], stoi(part[2]), part[3], stoi(part[4]), part[5] == "1"});
            } catch (...) {}
        }
    }
    void saveCustomers() {
        ofstream f(CUSTOMERS_FILE);
        for (auto& c : customers)
            f << c.name << '|' << c.phone << '|' << c.roomNo << '|' << c.checkIn << '|' << c.nights << '|' << c.active << '\n';
    }
    static void printBill(const Customer& c, double rate) {
        double sub = rate * c.nights, tax = sub * TAX_RATE;
        cout << fixed << setprecision(2);
        cout << "\n------------- BILL -------------\n"
             << "Customer : " << c.name << "\nRoom No. : " << c.roomNo << "\nCheck-in : " << c.checkIn
             << "\nNights   : " << c.nights << "\nRate     : " << rate
             << "\nSubtotal : " << sub << "\nTax (" << int(TAX_RATE * 100) << "%): " << tax
             << "\nTOTAL    : " << sub + tax << "\n--------------------------------\n";
    }

public:
    Hotel() { loadRooms(); loadCustomers(); }

    void viewRooms() {
        cout << "\nROOM NO.  TYPE     PRICE/NIGHT  STATUS\n";
        for (auto& r : rooms)
            cout << left << setw(10) << r.number << setw(9) << r.type << setw(13) << r.price
                 << (r.booked ? "Booked" : "Available") << '\n';
    }

    void bookRoom() {
        viewRooms();
        int no = readInt("\nEnter room number to book: ");
        Room* r = findRoom(no);
        if (!r) { cout << "Room does not exist.\n"; return; }
        if (r->booked) { cout << "Room is already booked.\n"; return; }
        Customer c;
        c.name = readLine("Customer name: ");
        c.phone = readLine("Phone: ");
        c.checkIn = readLine("Check-in date (e.g. 10-May-2026): ");
        c.nights = readInt("Number of nights: ");
        if (c.name.empty() || c.nights <= 0 || c.name.find('|') != string::npos ||
            c.phone.find('|') != string::npos || c.checkIn.find('|') != string::npos) {
            cout << "Invalid details. Booking cancelled.\n"; return;
        }
        c.roomNo = no; c.active = true;
        r->booked = true;
        customers.push_back(c);
        saveRooms(); saveCustomers();
        cout << "Room " << no << " booked successfully for " << c.name << ".\n";
    }

    void customerDetails() {
        bool any = false;
        for (auto& c : customers) {
            if (!c.active) continue;
            any = true;
            cout << "\nName     : " << c.name << "\nPhone    : " << c.phone << "\nRoom No. : " << c.roomNo
                 << "\nCheck-in : " << c.checkIn << "\nNights   : " << c.nights << '\n';
        }
        if (!any) cout << "No customers currently checked in.\n";
    }

    void billing() {
        int no = readInt("Enter room number: ");
        Customer* c = findActiveCustomerByRoom(no);
        if (!c) { cout << "No active booking for that room.\n"; return; }
        printBill(*c, findRoom(no)->price);
    }

    void checkOut() {
        int no = readInt("Enter room number to check out: ");
        Customer* c = findActiveCustomerByRoom(no);
        if (!c) { cout << "No active booking for that room.\n"; return; }
        printBill(*c, findRoom(no)->price);
        c->active = false;
        findRoom(no)->booked = false;
        saveRooms(); saveCustomers();
        cout << "Checked out. Thank you for staying with us!\n";
    }
};

int main() {
    Hotel hotel;
    while (true) {
        cout << "\n===== HOTEL MANAGEMENT SYSTEM =====\n"
             << "1. Book Room\n2. View Available Rooms\n3. Customer Details\n"
             << "4. Billing\n5. Check-out\n6. Exit\nChoice: ";
        string s; getline(cin, s);
        if (!cin) break;
        if (s == "1") hotel.bookRoom();
        else if (s == "2") hotel.viewRooms();
        else if (s == "3") hotel.customerDetails();
        else if (s == "4") hotel.billing();
        else if (s == "5") hotel.checkOut();
        else if (s == "6") { cout << "Goodbye!\n"; break; }
        else cout << "Invalid choice.\n";
    }
    return 0;
}
