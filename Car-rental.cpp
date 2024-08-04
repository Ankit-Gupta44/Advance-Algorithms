#include <bits/stdc++.h>
#include <ctime>

using namespace std;

// Helper function to get current date as a string
string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    return to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);
}

class Car;

class User {
public:
    int userId;
    string name;
    string email;

    User(int id, const string& n, const string& e) : userId(id), name(n), email(e) {}

    void searchCar(vector<Car>& cars, int carId);
    void bookCar(vector<Car>& cars, int carId);
};

class Car {
public:
    int carId;
    string make;
    float rentalPrice;
    bool availability;

    Car(int id, const string& mk, float price)
        : carId(id), make(mk), rentalPrice(price), availability(true) {}

    void getDetails() {
        cout << "Car ID: " << carId << ", Make: " << make << ", Rental Price: " << rentalPrice << ", Available: " << (availability ? "Yes" : "No") << std::endl;
    }

    bool isAvailable() {
        return availability;
    }

    void setAvailable(bool avail) {
        availability = avail;
    }
};

class Rental {
public:
    int rentalId;
    int userId;
    int carId;
    string startDate;
    string endDate;
    float totalCost;

    Rental(int rid, int uid, int cid, const string& start, const string& end, float cost)
        : rentalId(rid), userId(uid), carId(cid), startDate(start), endDate(end), totalCost(cost) {}

    void createRental() {
        cout<<"Rental created for User ID: "<<userId<<", Car ID: "<<carId<<", Start Date: "<<startDate<<", End Date: "<<endDate<<", Total Cost: $"<<totalCost<<endl;
    }

    void endRental() {
        cout << "Rental ended for Rental ID: " << rentalId << ", Car ID: " << carId << endl;
    }

    float calculateCost(int days, float pricePerDay) {
        return days * pricePerDay;
    }
};

class CarInventory {
private:
    std::vector<Car> cars;

public:
    void addCar(const Car& car) {
        cars.push_back(car);
    }

    void removeCar(int carId) {
        cars.erase(std::remove_if(cars.begin(), cars.end(), [carId](Car& c) { return c.carId == carId; }), cars.end());
    }

    Car* searchCar(int carId) {
        for (auto& car : cars) {
            if (car.carId == carId) {
                return &car;
            }
        }
        return nullptr;
    }


    void updateAvailability(int carId, bool avail) {
        Car* car = searchCar(carId);
        if (car != nullptr) {
            car->setAvailable(avail);
        }
    }

    std::vector<Car>& getCars() {
        return cars;
    }
};

class Payment {
public:
    int paymentId;
    int rentalId;
    float amount;

    Payment(int pid, int rid, float amt) : paymentId(pid), rentalId(rid), amount(amt) {}

    void processPayment() {
        // Payment processing logic
        cout << "Processing payment of $" << amount << " for rental ID: " << rentalId << endl;
    }
};


void User::searchCar( vector<Car>& cars, int carId) {
    for (auto& car : cars) {
        if (car.carId == carId && car.isAvailable()) {
            car.getDetails();
            return;
        }
    }
    cout << "Car with ID: " << carId << " is not available." << endl;
}


void User::bookCar(vector<Car>& cars, int carId) {
    for (auto& car : cars) {
        if (car.carId == carId && car.isAvailable()) {
            car.setAvailable(false);
            cout << "Car with ID: " << carId << " has been booked." << endl;
            return;
        }
    }
    cout << "Car with ID: " << carId << " is not available for booking." << endl;
}


int main() {
    // Create some users
    User user1(1, "John Doe", "john@example.com");
    User user2(2, "Jane Smith", "jane@example.com");

    // Create a car inventory
    CarInventory inventory;
    inventory.addCar(Car(1, "Toyota", 50.0));
    inventory.addCar(Car(2, "Honda", 45.0));

    // User searches for a car
    user1.searchCar(inventory.getCars(), 1);

    // User books a car
    user1.bookCar(inventory.getCars(), 1);

    // User tries to book the same car again
    user1.bookCar(inventory.getCars(), 1);

    // Create a rental
    Rental rental1(1, user1.userId, 1, "2024-08-01", "2024-08-05", 200.0);
    rental1.createRental();

    // Process payment
    Payment payment1(1, rental1.rentalId, rental1.totalCost);
    payment1.processPayment();

    // End rental and make the car available again
    rental1.endRental();
    inventory.updateAvailability(1, true);
    cout<<getCurrentDate();
    return 0;
}
