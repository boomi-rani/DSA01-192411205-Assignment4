#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Vehicle {
protected:
    std::string registration;
    double dailyRate;
    bool rented = false;

public:
    Vehicle(const std::string& reg, double rate)
        : registration(reg), dailyRate(rate) {}

    virtual double charge(int days) const = 0;

    bool rent() {
        if (rented) {
            return false;
        }
        rented = true;
        return true;
    }

    double returnVehicle(int lateDays, double lateFeePerDay = 100.0) {
        if (!rented) {
            return 0.0;
        }
        rented = false;
        return lateDays > 0 ? lateDays * lateFeePerDay : 0.0;
    }

    virtual void display() const {
        std::cout << registration << " | "
                  << (rented ? "Rented" : "Available")
                  << " | Daily rate: " << dailyRate << '\n';
    }

    virtual ~Vehicle() = default;
};

class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& reg, double rate) : Vehicle(reg, rate) {}

    double charge(int days) const override {
        return dailyRate * days;
    }
};

class MotorVehicle : public Vehicle {
protected:
    double fuelOrChargeLevel;
    std::string serviceDueDate;

public:
    MotorVehicle(const std::string& reg, double rate,
                 double level, const std::string& dueDate)
        : Vehicle(reg, rate),
          fuelOrChargeLevel(level), serviceDueDate(dueDate) {}
};

class TwoWheeler : public MotorVehicle {
public:
    TwoWheeler(const std::string& reg, double rate, double fuel,
               const std::string& dueDate)
        : MotorVehicle(reg, rate, fuel, dueDate) {}

    double charge(int days) const override {
        const double fuelServiceFee = 25.0;
        return dailyRate * days + fuelServiceFee;
    }
};

class Car : public MotorVehicle {
private:
    int seatingCapacity;

public:
    Car(const std::string& reg, double rate, double fuel,
        const std::string& dueDate, int seats)
        : MotorVehicle(reg, rate, fuel, dueDate),
          seatingCapacity(seats) {}

    double charge(int days) const override {
        const double passengerServiceFee = seatingCapacity * 10.0;
        return dailyRate * days + passengerServiceFee;
    }

    void display() const override {
        Vehicle::display();
        std::cout << "  Seats: " << seatingCapacity << '\n';
    }
};

class ElectricCar : public MotorVehicle {
private:
    double batteryCapacityKWh;
    double pricePerKWh;
    double estimatedKWhPerDay;

public:
    ElectricCar(const std::string& reg, double dailyRate, double chargeLevel,
                const std::string& dueDate, double capacity,
                double kWhPrice, double dailyEnergyUse)
        : MotorVehicle(reg, dailyRate, chargeLevel, dueDate),
          batteryCapacityKWh(capacity), pricePerKWh(kWhPrice),
          estimatedKWhPerDay(dailyEnergyUse) {}

    double charge(int days) const override {
        return dailyRate * days +
               estimatedKWhPerDay * pricePerKWh * days;
    }

    void display() const override {
        Vehicle::display();
        std::cout << "  Battery capacity: "
                  << batteryCapacityKWh << " kWh\n";
    }
};

// Added later without changing the existing vehicle classes or fleet loop.
class ElectricBus : public Vehicle {
public:
    ElectricBus(const std::string& reg, double rate) : Vehicle(reg, rate) {}

    double charge(int days) const override {
        return dailyRate * days + 500.0; // fixed operating surcharge
    }
};

int main() {
    std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<Bicycle>("BIKE01", 100.0));
    fleet.push_back(std::make_unique<TwoWheeler>(
        "TW02", 250.0, 80.0, "2026-12-01"));
    fleet.push_back(std::make_unique<Car>(
        "CAR03", 1500.0, 70.0, "2026-12-10", 5));
    fleet.push_back(std::make_unique<ElectricCar>(
        "EV04", 1800.0, 90.0, "2026-12-15",
        60.0, 8.0, 12.0));

    // New type added through the same abstract base-class interface.
    fleet.push_back(std::make_unique<ElectricBus>("BUS05", 3000.0));

    std::cout << std::fixed << std::setprecision(2);
    for (const auto& vehicle : fleet) {
        if (vehicle->rent()) {
            std::cout << vehicle->charge(2) << '\n';
            std::cout << "Late fee: "
                      << vehicle->returnVehicle(1) << '\n';
        }
    }

    std::cout << "\nFleet listing:\n";
    for (const auto& vehicle : fleet) {
        vehicle->display();
    }
    return 0;
}
