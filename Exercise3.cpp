#define NOMINMAX
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <limits>
#include <Windows.h>
#include <string>

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
	bool is_greater(UsualTariff);
};

bool UsualTariff::is_greater(UsualTariff t) {
	return calculate_price(get_base_price()) > t.calculate_price(get_base_price());
}

class DiscountTariff : public Tariff, public DiscountStrategy {
public:
	DiscountTariff(double p, double discount, std::string c1, std::string c2) :
		Tariff(p, c1, c2), DiscountStrategy{ discount } {
	};
	bool is_less(DiscountTariff);
};

bool DiscountTariff::is_less(DiscountTariff t) {
	return calculate_price(get_base_price()) < t.calculate_price(get_base_price());
}

class ATE {
private:
	std::vector<UsualTariff*> usual_tariffs;
	std::vector<DiscountTariff*> discount_tariffs;
	double total = 0.0;
public:
	void add_usual_tariff(double price, const std::string& city1, const std::string& city2) {
		usual_tariffs.push_back(new UsualTariff(price, city1, city2));
	}
	void add_discount_tariff(double price, double discount, const std::string& city1, const std::string& city2) {
		discount_tariffs.push_back(new DiscountTariff(price, discount, city1, city2));
	}
	ATE& operator += (const UsualTariff& tariff) {
		total += tariff.calculate_price(tariff.get_base_price());
		return *this;
	}

	ATE& operator += (const DiscountTariff& tariff) {
		total += tariff.calculate_price(tariff.get_base_price());
		return *this;
	}
	double calculate_average_price() {
		if (usual_tariffs.empty() and discount_tariffs.empty()) {
			throw TariffException("списки тарифов пусты");
		}
		for (int i = 0; i < usual_tariffs.size(); i++) {
			*this += *usual_tariffs[i];
		}
		for (int i = 0; i < discount_tariffs.size(); i++) {
			*this += *discount_tariffs[i];
		}
		return total / (double)(size(usual_tariffs) + size(discount_tariffs));
	}
	std::pair<int, int> show_tariff_list() const {
		if (usual_tariffs.empty() and discount_tariffs.empty()) {
			throw TariffException("списки тарифов пусты");
		}
		std::cout << "\nОБЫЧНЫЕ ТАРИФЫ:\n";
		for (int i = 0; i < usual_tariffs.size(); i++) {
			std::cout << i + 1 << ") " << usual_tariffs[i]->get_city1() << " -> " << usual_tariffs[i]->get_city2() << " = " << usual_tariffs[i]->calculate_price(usual_tariffs[i]->get_base_price()) << std::endl;
		}
		std::cout << "\nЛЬГОТНЫЕ ТАРИФЫ:\n";
		for (int i = 0; i < discount_tariffs.size(); i++) {
			std::cout << i + 1 << ") " << discount_tariffs[i]->get_city1() << " -> " << discount_tariffs[i]->get_city2() << " = " << discount_tariffs[i]->calculate_price(discount_tariffs[i]->get_base_price()) << std::endl;
		}
		std::pair<int, int> counts;
		counts.first = usual_tariffs.size();
		counts.second = discount_tariffs.size();
		return counts;
	}
	void compare(char type, int index1, int index2) {
		if (type == 'u') {
			if (index1 < 0 || index1 >= usual_tariffs.size()) {
				throw TariffException("неверные индексы тарифов");
			} else {
				if (usual_tariffs[index1]->is_greater(*usual_tariffs[index2])) {
					std::cout << "тариф с номером " << index1+1 << " больше тарифа с номером " << index2+1;
				} else {
					std::cout << "тариф с номером " << index1+1 << " не больше тарифа с номером " << index2+1;
				}
			}
		}
		else if (type == 'd') {
			if (index1 < 0 || index1 >= discount_tariffs.size()) {
				throw TariffException("неверные индексы тарифов");
			}
			else {
				if (discount_tariffs[index1]->is_less(*discount_tariffs[index2])) {
					std::cout << "тариф с номером " << index1+1 << " меньше тарифа с номером " << index2+1;
				}
				else {
					std::cout << "тариф с номером " << index1+1 << " не меньше тарифа с номером " << index2+1;
				}
			}
		}
	}
};

void show_menu() {
	std::cout << "==========МЕНЮ==========" << std::endl;
	std::cout << "1) добавить обычный тариф" << std::endl;
	std::cout << "2) добавить льготный тариф" << std::endl;
	std::cout << "3) вывести среднюю цену тарифов" << std::endl;
	std::cout << "4) вывести списки тарифов" << std::endl;
	std::cout << "5) сравнить тарифы" << std::endl;
	std::cout << "0) выход" << std::endl;
}

char InputChar(const std::string& prompt) {
	char c;
	std::cout << prompt;
	std::cin >> c;
	while (c != 'u' && c != 'd') {
		std::cout << "Неверное значение типа.\n" << prompt;
		std::cin >> c;
	}
	return c;
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
		case 5:
			try {
				std::pair<int, int> counts = ate.show_tariff_list();
				std::cout << std::endl;
				char t = InputChar("введите тип тарифов ('u' - обычный тариф, 'd' - льготный тариф): ");
				int i1 = InputInt("введите номер первого тарифа: ");
				if (t == 'u') {
					while (i1 < 1 || i1 > counts.first) {
						std::cout << "Неверный индекс. Введите целое число от 1 до " + std::to_string(counts.first) + ".\n";
						i1 = InputInt("введите номер первого тарифа: ");
					}
				}
				else if (t == 'd') {
					while (i1 < 1 || i1 > counts.second) {
						std::cout << "Неверный индекс. Введите целое число от 1 до " + std::to_string(counts.second) + ".\n";
						i1 = InputInt("введите номер первого тарифа: ");
					}
				}
				int i2 = InputInt("введите номер второго тарифа: ");
				if (t == 'u') {
					while (i1 < 1 || i2 > counts.first) {
						std::cout << "Неверный индекс. Введите целое число от 1 до " + std::to_string(counts.first) + ".\n";
						i2 = InputInt("введите номер второго тарифа: ");
					}
				}
				else if (t == 'd') {
					while (i1 < 1 || i2 > counts.second) {
						std::cout << "Неверный индекс. Введите целое число от 1 до " + std::to_string(counts.second) + ".\n";
						i2 = InputInt("введите номер второго тарифа: ");
					}
				}
				ate.compare(t, i1-1, i2-1);
			}
			catch (const TariffException& e) {
				std::cerr << e.what() << std::endl;
			}
			break;
		case 0:
			return 0;
		default:
			std::cout << "Введено некорректное значение. Введите целое число от 0 до 4." << std::endl;
		}
	} while (choice != 0);
}