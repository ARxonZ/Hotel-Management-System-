Hotel Management System (C++)
A console-based Hotel Management System that handles room bookings, customer details and billing, with data saved in text files.
Features
Book a room (name, phone, check-in date, nights)
View available / booked rooms
View current customer details
Billing with 12% tax
Check-out (frees the room)
File handling: rooms.txt and customers.txt are created automatically and persist between runs
Build & Run
g++ -std=c++11 -o hotel main.cpp
./hotel
(or make then ./hotel)
Menu
1. Book Room
2. View Available Rooms
3. Customer Details
4. Billing
5. Check-out
6. Exit
Concepts used
Structs/classes, vectors, file I/O (ifstream/ofstream), input validation, menu-driven design.
