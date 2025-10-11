#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <limits>

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
			throw TariffException("������ ������ ���� ������������ ������ � ��������� �� 0 �� 100");
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
	PriceStrategy* strategy;
public:
	Tariff(double p, std::string c1, std::string c2, PriceStrategy* s) :
		price(p), city1(c1), city2(c2), strategy(s) {
		if (price < 0) {
			throw TariffException("���� �� ����� ���� �������������");
		}
	}
	virtual ~Tariff() = default;
	double get_base_price() const { return price; }
	double get_final_price() const { return strategy->calculate_price(price); }
	std::string get_city1() const { return city1; }
	std::string get_city2() const { return city2; }
};

class UsualTariff : public Tariff {
public:
	UsualTariff(double p, std::string c1, std::string c2) :
		Tariff(p, c1, c2, new NoDiscountStrategy()) {};
};

class DiscountTariff : public Tariff {
public:
	DiscountTariff(double p, double discount, std::string c1, std::string c2) :
		Tariff(p, c1, c2, new DiscountStrategy(discount)) {};
};

class ATE {
private:
	std::vector<Tariff*> tariffs;
public:
	void add_usual_tariff(double price, const std::string& city1, const std::string& city2) {
		tariffs.push_back(new UsualTariff(price, city1, city2));
	}
	void add_discount_tariff(double price, double discount, const std::string& city1, const std::string& city2) {
		tariffs.push_back(new DiscountTariff(price, discount, city1, city2));
	}
	double calculate_average_price() const {
		if (tariffs.empty()) {
			throw TariffException("������ ������� ����");
		}
		double total = 0.0;
		for (int i = 0; i < tariffs.size(); i++) {
			total += tariffs[i]->get_final_price();
		}
		return total / (double)size(tariffs);
	}
	void show_tariff_list() const {
		if (tariffs.empty()) {
			throw TariffException("������ ������� ����");
		}
		for (int i = 0; i < tariffs.size(); i++) {
			std::cout << tariffs[i]->get_city1() << " -> " << tariffs[i]->get_city2() << " = " << tariffs[i]->get_final_price() << std::endl;
		}
	}
};

void show_menu() {
	std::cout << "==========����==========" << std::endl;
	std::cout << "1) �������� ������� �����" << std::endl;
	std::cout << "2) �������� �������� �����" << std::endl;
	std::cout << "3) ������� ������� ���� �������" << std::endl;
	std::cout << "4) ������� ������ �������" << std::endl;
	std::cout << "0) �����" << std::endl;
}

std::string InputString(const std::string& prompt) {
	std::string s;
	std::cout << prompt;
	std::cin >> s;
	while (s.empty()) {
		std::cout << "���� �� ����� ���� ������.\n" << prompt;
		std::cin >> s;
	}
	return s;
}
int InputInt(const std::string& prompt) {
	int n;
	std::cout << prompt;
	while (!(std::cin >> n)) {
		std::cout << "������������ ����.\n" << prompt;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return n;
}
double InputDouble(const std::string& prompt) {
	double d;
	std::cout << prompt;
	while (!(std::cin >> d)) {
		std::cout << "������������ ����.\n" << prompt;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return d;
}


int main() {
	setlocale(LC_ALL, "Russian");
	ATE ate;
	show_menu();
	std::cout << std::endl;
	int choice;
	do {
		choice = InputInt("\n�������� ��������: ");
		switch (choice) {
		case 1: {
			std::string city1 = InputString("������� ������ �����: "), city2 = InputString("������� ������ �����: ");
			double price = InputDouble("������� ���� : ");
			try {
				ate.add_usual_tariff(price, city1, city2);
			}
			catch (const TariffException& e) {
				std::cerr << e.what() << std::endl;
			}
			break;
		}
		case 2: {
			std::string city1 = InputString("������� ������ �����: "), city2 = InputString("������� ������ �����: ");
			double price = InputDouble("������� ���� : "), discount = InputDouble("������� ������ (� ���������): ");
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
			std::cout << "������� ������������ ��������. ������� ����� ����� �� 0 �� 4." << std::endl;
		}
	} while (choice != 0);
}