#define NOMINMAX
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <limits>
#include <Windows.h>

class TariffException : public std::invalid_argument {
public:
	explicit TariffException(const std::string& message) :
		std::invalid_argument("TariffException: " + message) {}
};

class PriceStrategy {
public:
	virtual ~PriceStrategy() = default;
	virtual double calculate_price(double price) const = 0;
};

class NoDiscountStrategy : public PriceStrategy {
public:
	double calculate_price(double price) const override {
		return price;
	}
};

class DiscountStrategy : public PriceStrategy {
private:
	double discount;
public:
	explicit DiscountStrategy(double d) : discount(d) {
		if (discount < 0 || discount > 100) {
			throw TariffException("скидка должна быть вещественным числом в диапазоне от 0 до 100");
		}
	}
	double calculate_price(double price) const override {
		return (1.0 - discount / 100.0) * price;
	}
};

class Tariff {
protected:
	double price;
	std::string city1;
	std::string city2;
public:
	Tariff(double p, std::string c1, std::string c2) :
		price(p), city1(c1), city2(c2) {
		if (price < 0) {
			throw TariffException("цена не может быть отрицательной");
		}
	}
	virtual ~Tariff() = default;
	double get_base_price() const { return price; }
	std::string get_city1() const { return city1; }
	std::string get_city2() const { return city2; }
};

class UsualTariff : public Tariff, public NoDiscountStrategy {
public:
	UsualTariff(double p, std::string c1, std::string c2) :
		Tariff(p, c1, c2) {
	};
};

class DiscountTariff : public Tariff, public DiscountStrategy {
public:
	DiscountTariff(double p, double discount, std::string c1, std::string c2) :
		Tariff(p, c1, c2), DiscountStrategy{discount} {
	};
};

class ATE {
private:
	std::vector<UsualTariff*> usual_tariffs;
	std::vector<DiscountTariff*> discount_tariffs;
public:
	void add_usual_tariff(double price, const std::string& city1, const std::string& city2) {
		usual_tariffs.push_back(new UsualTariff(price, city1, city2));
	}
	void add_discount_tariff(double price, double discount, const std::string& city1, const std::string& city2) {
		discount_tariffs.push_back(new DiscountTariff(price, discount, city1, city2));
	}
	double calculate_average_price() const {
		if (usual_tariffs.empty() and discount_tariffs.empty()) {
			throw TariffException("списки тарифов пусты");
		}
		double total = 0.0;
		for (int i = 0; i < usual_tariffs.size(); i++) {
			total += usual_tariffs[i]->calculate_price(usual_tariffs[i]->get_base_price());
		}
		for (int i = 0; i < discount_tariffs.size(); i++) {
			total += discount_tariffs[i]->calculate_price(discount_tariffs[i]->get_base_price());
		}
		return total / (double)(size(usual_tariffs) + size(discount_tariffs));
	}
	void show_tariff_list() const {
		if (usual_tariffs.empty() and discount_tariffs.empty()) {
			throw TariffException("списки тарифов пусты");
		}
		std::cout << "ОБЫЧНЫЕ ТАРИФЫ:\n";
		for (int i = 0; i < usual_tariffs.size(); i++) {
			std::cout << usual_tariffs[i]->get_city1() << " -> " << usual_tariffs[i]->get_city2() << " = " << usual_tariffs[i]->calculate_price(usual_tariffs[i]->get_base_price()) << std::endl;
		}
		std::cout << "\nЛЬГОТНЫЕ ТАРИФЫ:\n";
		for (int i = 0; i < discount_tariffs.size(); i++) {
			std::cout << discount_tariffs[i]->get_city1() << " -> " << discount_tariffs[i]->get_city2() << " = " << discount_tariffs[i]->calculate_price(discount_tariffs[i]->get_base_price()) << std::endl;
		}
	}
};

void show_menu() {
	std::cout << "==========МЕНЮ==========" << std::endl;
	std::cout << "1) добавить обычный тариф" << std::endl;
	std::cout << "2) добавить льготный тариф" << std::endl;
	std::cout << "3) вывести среднюю цену тарифов" << std::endl;
	std::cout << "4) вывести списки тарифов" << std::endl;
	std::cout << "0) выход" << std::endl;
}

std::string InputString(const std::string& prompt) {
	std::string s;
	std::cout << prompt;
	std::cin >> s;
	while (s.empty()) {
		std::cout << "Поле не может быть пустым.\n" << prompt;
		std::cin >> s;
	}
	return s;
}
int InputInt(const std::string& prompt) {
	int n;
	std::cout << prompt;
	while (!(std::cin >> n)) {
		std::cout << "Некорректный ввод.\n" << prompt;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return n;
}
double InputDouble(const std::string& prompt) {
	double d;
	std::cout << prompt;
	while (!(std::cin >> d)) {
		std::cout << "Некорректный ввод.\n" << prompt;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return d;
}


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ATE ate;
	show_menu();
	std::cout << std::endl;
	int choice;
	do {
		choice = InputInt("\nвыберите действие: ");
		switch (choice) {
		case 1: {
			std::string city1 = InputString("введите первый город: "), city2 = InputString("введите второй город: ");
			double price = InputDouble("введите цену : ");
			try {
				ate.add_usual_tariff(price, city1, city2);
			}
			catch (const TariffException& e) {
				std::cerr << e.what() << std::endl;
			}
			break;
		}
		case 2: {
			std::string city1 = InputString("введите первый город: "), city2 = InputString("введите второй город: ");
			double price = InputDouble("введите цену : "), discount = InputDouble("введите скидку (в процентах): ");
			try {
				ate.add_discount_tariff(price, discount, city1, city2);
			}
			catch (const TariffException& e) {
				std::cerr << e.what() << std::endl;
			}
			break;
		}
		case 3: {
			try {
				std::cout << ate.calculate_average_price() << std::endl;
			}
			catch (const TariffException& e) {
				std::cerr << e.what() << std::endl;
			}
			break;
		}
		case 4: {
			try {
				ate.show_tariff_list();
			}
			catch (const TariffException& e) {
				std::cerr << e.what() << std::endl;
			}
			break;
		}
		case 0:
			return 0;
		default:
			std::cout << "Введено некорректное значение. Введите целое число от 0 до 4." << std::endl;
		}
	} while (choice != 0);
}