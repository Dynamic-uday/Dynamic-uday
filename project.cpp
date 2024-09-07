#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <algorithm> // For std::remove_if and std::find_if

using namespace std;

class Room {
public:
    int roomNumber;
    string roomType;
    string status; // "available" or "occupied"

    Room(int number, const string& type, const string& stat = "available")
        : roomNumber(number), roomType(type), status(stat) {}
};

class Hotel {
private:
    vector<Room> rooms;

public:
    void addRoom(const Room& room) {
        rooms.push_back(room);
    }

    void removeRoom(int roomNumber) {
        rooms.erase(remove_if(rooms.begin(), rooms.end(), [roomNumber](const Room& room) {
            return room.roomNumber == roomNumber;
        }), rooms.end());
    }

    bool checkAvailability(int roomNumber) const {
        for (const auto& room : rooms) {
            if (room.roomNumber == roomNumber) {
                return room.status == "available";
            }
        }
        return false;
    }

    void updateRoomStatus(int roomNumber, const string& status) {
        for (auto& room : rooms) {
            if (room.roomNumber == roomNumber) {
                room.status = status;
                break;
            }
        }
    }

    void listAvailableRooms() const {
        cout << "\nAvailable rooms:\n";
        for (const auto& room : rooms) {
            if (room.status == "available") {
                cout << "Room " << room.roomNumber << " (" << room.roomType << ")\n";
            }
        }
    }
};

class BookingSystem {
private:
    unordered_map<string, pair<int, string>> bookings;

public:
    void bookRoom(const string& bookingId, int roomNumber, const string& customerName) {
        bookings[bookingId] = { roomNumber, customerName };
    }

    void cancelBooking(const string& bookingId) {
        bookings.erase(bookingId);
    }

    pair<int, string> getBookingDetails(const string& bookingId) const {
        auto it = bookings.find(bookingId);
        if (it != bookings.end()) {
            return it->second;
        }
        return {-1, "Not Found"};
    }
};

class Customer {
public:
    string name;
    string contactInfo;
    vector<string> bookingHistory;

    Customer(const string& name, const string& contact)
        : name(name), contactInfo(contact) {}

    void addBooking(const string& bookingId) {
        bookingHistory.push_back(bookingId);
    }
};

class CustomerManager {
private:
    vector<Customer> customers;

public:
    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }

    void removeCustomer(const string& name) {
        customers.erase(remove_if(customers.begin(), customers.end(), [name](const Customer& customer) {
            return customer.name == name;
        }), customers.end());
    }

    Customer* findCustomer(const string& name) {
        for (auto& customer : customers) {
            if (customer.name == name) {
                return &customer;
            }
        }
        return nullptr;
    }
};

class BillingSystem {
private:
    unordered_map<string, double> roomRates;
    unordered_map<string, double> bills;

public:
    void setRoomRate(const string& roomType, double rate) {
        roomRates[roomType] = rate;
    }

    double calculateBill(const string& bookingId, const string& roomType, int numberOfDays) {
        double rate = roomRates[roomType];
        double billAmount = rate * numberOfDays;
        bills[bookingId] = billAmount;
        return billAmount;
    }

    double generateInvoice(const string& bookingId) const {
        auto it = bills.find(bookingId);
        if (it != bills.end()) {
            return it->second;
        }
        return -1; // Indicate no bill found
    }
};

class ReservationQueue {
private:
    queue<string> reservations;

public:
    void addReservation(const string& reservationId) {
        reservations.push(reservationId);
    }

    string processReservation() {
        if (reservations.empty()) {
            return "No reservations to process";
        }
        string reservationId = reservations.front();
        reservations.pop();
        return reservationId;
    }
};

// Utility function to pause the screen and wait for the user to press enter
void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

// Function to display menu options
void displayMenu() {
    cout << "\nHotel Management System\n";
    cout << "1. Add Room\n";
    cout << "2. List Available Rooms\n";
    cout << "3. Book Room\n";
    cout << "4. Cancel Booking\n";
    cout << "5. Add Customer\n";
    cout << "6. View Customer Info\n";
    cout << "7. Calculate Bill\n";
    cout << "8. Process Reservation\n";
    cout << "9. Exit\n";
}

int main() {
    Hotel hotel;
    BookingSystem bookingSystem;
    CustomerManager customerManager;
    BillingSystem billingSystem;
    ReservationQueue reservationQueue;

    billingSystem.setRoomRate("Single", 100.0);
    billingSystem.setRoomRate("Double", 150.0);

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int roomNumber;
                string roomType;
                cout << "Enter room number: ";
                cin >> roomNumber;
                cout << "Enter room type (Single/Double): ";
                cin >> roomType;
                hotel.addRoom(Room(roomNumber, roomType));
                cout << "Room added successfully.\n";
                pause();
                break;
            }
            case 2: {
                hotel.listAvailableRooms();
                pause();
                break;
            }
            case 3: {
                string bookingId, customerName;
                int roomNumber;
                cout << "Enter booking ID: ";
                cin >> bookingId;
                cout << "Enter room number: ";
                cin >> roomNumber;
                cout << "Enter customer name: ";
                cin >> customerName;
                if (hotel.checkAvailability(roomNumber)) {
                    hotel.updateRoomStatus(roomNumber, "occupied");
                    bookingSystem.bookRoom(bookingId, roomNumber, customerName);
                    Customer* customer = customerManager.findCustomer(customerName);
                    if (customer) {
                        customer->addBooking(bookingId);
                    } else {
                        customerManager.addCustomer(Customer(customerName, "N/A"));
                    }
                    cout << "Room booked successfully.\n";
                } else {
                    cout << "Room not available.\n";
                }
                pause();
                break;
            }
            case 4: {
                string bookingId;
                cout << "Enter booking ID to cancel: ";
                cin >> bookingId;
                auto booking = bookingSystem.getBookingDetails(bookingId);
                if (booking.first != -1) {
                    hotel.updateRoomStatus(booking.first, "available");
                    bookingSystem.cancelBooking(bookingId);
                    cout << "Booking cancelled successfully.\n";
                } else {
                    cout << "Booking ID not found.\n";
                }
                pause();
                break;
            }
            case 5: {
                string name, contact;
                cout << "Enter customer name: ";
                cin >> name;
                cout << "Enter customer contact: ";
                cin >> contact;
                customerManager.addCustomer(Customer(name, contact));
                cout << "Customer added successfully.\n";
                pause();
                break;
            }
            case 6: {
                string name;
                cout << "Enter customer name: ";
                cin >> name;
                Customer* customer = customerManager.findCustomer(name);
                if (customer) {
                    cout << "Customer Name: " << customer->name << "\n";
                    cout << "Contact Info: " << customer->contactInfo << "\n";
                    cout << "Booking History:\n";
                    for (const auto& booking : customer->bookingHistory) {
                        cout << booking << "\n";
                    }
                } else {
                    cout << "Customer not found.\n";
                }
                pause();
                break;
            }
            case 7: {
                string bookingId, roomType;
                int days;
                cout << "Enter booking ID: ";
                cin >> bookingId;
                cout << "Enter room type (Single/Double): ";
                cin >> roomType;
                cout << "Enter number of days: ";
                cin >> days;
                double bill = billingSystem.calculateBill(bookingId, roomType, days);
                cout << "Total Bill: $" << bill << "\n";
                pause();
                break;
            }
            case 8: {
                string reservationId;
                cout << "Enter reservation ID: ";
                cin >> reservationId;
                reservationQueue.addReservation(reservationId);
                cout << "Reservation added successfully.\n";
                pause();
                break;
            }
            case 9: {
                cout << "Exiting system...\n";
                break;
            }
            default:
                cout << "Invalid option. Please try again.\n";
                pause();
        }

    } while (choice != 9);

    return 0;
}
