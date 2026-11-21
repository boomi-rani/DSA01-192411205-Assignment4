#include <iomanip>
#include <iostream>
#include <string>

class Cart {
private:
    std::string items[10];
    double prices[10]{};
    int n = 0;
    double discount = 0.0;

public:
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            sum += prices[i];
        }
        return sum * (100.0 - discount) / 100.0;
    }
};

int main() {
    // Direct object and method chaining.
    Cart first;
    first.add("Rice", 60).add("Dal", 120).setDiscount(10);

    // Pointer to an existing object.
    Cart* pointer = &first;
    pointer->add("Oil", 180);

    // Reference to the same object.
    Cart& reference = first;
    reference.add("Salt", 20);

    // Dynamically allocated object; delete releases it.
    Cart* dynamicCart = new Cart;
    dynamicCart->add("Milk", 50);
    std::cout << "Dynamic cart total: " << dynamicCart->total() << '\n';
    delete dynamicCart;

    // Array of pointers and highest-total search.
    Cart second;
    second.add("Book", 200);
    Cart third;
    third.add("Pen", 30).add("Bag", 500);

    Cart* carts[] = {&first, &second, &third};
    Cart* highest = carts[0];

    for (int i = 1; i < 3; ++i) {
        if (carts[i]->total() > highest->total()) {
            highest = carts[i];
        }
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Required cart total: " << reference.total() << '\n';
    std::cout << "Highest cart total: " << highest->total() << '\n';
    return 0;
}
