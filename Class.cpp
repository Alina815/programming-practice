#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <limits>

#include "Class.h"

TariffException::TariffException(const std::string& message) :
	std::invalid_argument("TariffException: " + message) {}

PriceStrategy::~PriceStrategy() = default;

double NoDiscountStrategy::calculate_price(double price) const {
	return price;
}

DiscountStrategy::DiscountStrategy(double d) : discount(d) {
	if (discount < 0 || discount > 100) {
		throw TariffException("скидка должна быть вещественным числом в диапазоне от 0 до 100");
	}
}
double DiscountStrategy::calculate_price(double price) const {
	return (1.0 - discount / 100.0) * price;
}

Tariff::Tariff(double p, std::string c1, std::string c2) :
	price(p), city1(c1), city2(c2) {
	if (price < 0) {
		throw TariffException("цена не может быть отрицательной");
	}
}
Tariff::~Tariff() = default;
double Tariff::get_base_price() const { return price; }
std::string Tariff::get_city1() const { return city1; }
std::string Tariff::get_city2() const { return city2; }


UsualTariff::UsualTariff(double p, std::string c1, std::string c2) :
	Tariff(p, c1, c2) {
}
bool UsualTariff::is_greater(UsualTariff t) {
	return calculate_price(get_base_price()) > t.calculate_price(get_base_price());
}

DiscountTariff::DiscountTariff(double p, double discount, std::string c1, std::string c2) :
	Tariff(p, c1, c2), DiscountStrategy{ discount } {
}
bool DiscountTariff::is_less(DiscountTariff t) {
	return calculate_price(get_base_price()) < t.calculate_price(get_base_price());
}

void ATE::add_usual_tariff(double price, const std::string& city1, const std::string& city2) {
	usual_tariffs.push_back(new UsualTariff(price, city1, city2));
}
void ATE::add_discount_tariff(double price, double discount, const std::string& city1, const std::string& city2) {
	discount_tariffs.push_back(new DiscountTariff(price, discount, city1, city2));
}
ATE& ATE::operator += (const UsualTariff& tariff) {
	total += tariff.calculate_price(tariff.get_base_price());
	return *this;
}
ATE& ATE::operator += (const DiscountTariff& tariff) {
	total += tariff.calculate_price(tariff.get_base_price());
	return *this;
}
double ATE::calculate_average_price() {
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
std::pair<int, int> ATE::show_tariff_list() const {
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
void ATE::compare(char type, int index1, int index2) {
	if (type == 'u') {
		if (index1 < 0 || index1 >= usual_tariffs.size()) {
			throw TariffException("неверные индексы тарифов");
		}
		else {
			if (usual_tariffs[index1]->is_greater(*usual_tariffs[index2])) {
				std::cout << "тариф с номером " << index1 + 1 << " больше тарифа с номером " << index2 + 1 << std::endl;
			}
			else {
				std::cout << "тариф с номером " << index1 + 1 << " не больше тарифа с номером " << index2 + 1 << std::endl;
			}
		}
	}
	else if (type == 'd') {
		if (index1 < 0 || index1 >= discount_tariffs.size()) {
			throw TariffException("неверные индексы тарифов");
		}
		else {
			if (discount_tariffs[index1]->is_less(*discount_tariffs[index2])) {
				std::cout << "тариф с номером " << index1 + 1 << " меньше тарифа с номером " << index2 + 1 << std::endl;
			}
			else {
				std::cout << "тариф с номером " << index1 + 1 << " не меньше тарифа с номером " << index2 + 1 << std::endl;
			}
		}
	}
}