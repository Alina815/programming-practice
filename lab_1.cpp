#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

class ATE {
private:
    std::string address;
    int number;
    double price;
public:
    void set_data(std::string address, int number, double price) {
        this->address = address;
        this->number = number;
        this->price = price;
    }
    std::string get_address() {
        return this->address;
    }
    int get_number() {
        return this->number;
    }
    double get_price() {
        return this->price;
    }
    double sum_price() {
        return this->number * this->price;
    }
};

int main() {
    std::string address, t_number, t_price;
    int number;
    double price;
    size_t pos1, pos2 = 0;
    std::cout << "input address, number, price separated by space: ";
    if (!(std::cin >> address >> t_number >> t_price)) {
        std::cout << "data was not read" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 1;
    }
    try {
        number = std::stoi(t_number, &pos1);
        price = std::stod(t_price, &pos2);
        if (pos1 != t_number.length() or pos2 != t_price.length()) {
            std::cerr << "string has extra characters" << std::endl;
            return 1;
        }

    }
    catch (const std::invalid_argument&) {
        std::cerr << "input data is not a number" << std::endl;
        return 1;
    }
    catch (const std::out_of_range&) {
        std::cerr << "input number is out of range" << std::endl;
        return 1;
    }
    ATE example;
    example.set_data(address, number, price);
    double result = example.sum_price();
    std::cout << result << std::endl;
    return 0;
}
