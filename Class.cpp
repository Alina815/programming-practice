#include "Class.h"

using namespace System;
using namespace msclr::interop;

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

double DiscountStrategy::get_discount() const { return discount; }

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

DiscountTariff::DiscountTariff(double p, double discount, std::string c1, std::string c2) :
	Tariff(p, c1, c2), DiscountStrategy{ discount } {
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
	if (usual_tariffs.empty() && discount_tariffs.empty()) {
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

std::vector<std::string> ATE::Split(std::string str, char sep) {
	std::stringstream ss(str);
	std::vector<std::string> tokens;
	std::string token;
	while (std::getline(ss, token, sep)) {
		tokens.push_back(token);
	}
	return tokens;
}

void ATE::SetLists(System::Collections::Generic::List<System::String^>^ tariff_list) {
	for (auto* tariff : usual_tariffs) {
		delete tariff;
	}
	usual_tariffs.clear();
	for (auto* tariff : discount_tariffs) {
		delete tariff;
	}
	discount_tariffs.clear();
	for (int i = 0; i < tariff_list->Count; i++) {
		std::string line = marshal_as<std::string>(tariff_list[i]);
		std::vector<std::string> parts = Split(line, ';');
		if (parts.size() == 4 && parts[0] == "u") {
			std::string city1 = parts[1];
			std::string city2 = parts[2];
			double price = stod(parts[3]);
			usual_tariffs.push_back(new UsualTariff(price, city1, city2));
		}
		else if (parts.size() == 5 && parts[0] == "d") {
			std::string city1 = parts[1];
			std::string city2 = parts[2];
			double price = stod(parts[3]);
			double discount = stod(parts[4]);
			discount_tariffs.push_back(new DiscountTariff(price, discount, city1, city2));
		}
		else {
			throw TariffException("Неверный формат строки в списке тарифов.");
		}
	}
}

int ATE::CompareTariffsByPrice(System::String^ x, System::String^ y) {
	try {
		array<System::String^>^ parts_x = x->Split(gcnew array<System::Char>{L';'});
		array<System::String^>^ parts_y = y->Split(gcnew array<System::Char>{L';'});
		if (parts_x->Length < 4 || parts_x->Length > 5 || parts_y->Length < 4 || parts_y->Length > 5)
			throw TariffException("Неверный формат строки в списке тарифов.");
		if (Convert::ToDouble(parts_x[3]) < Convert::ToDouble(parts_y[3])) {
			return -1;
		}
		else if (Convert::ToDouble(parts_x[3]) > Convert::ToDouble(parts_y[3])) {
			return 1;
		}
		else {
			return 0;
		}
	}
	catch (...) {
		throw TariffException("Ошибка при сортировке тарифов по цене.");
	}

}

int ATE::CompareTariffsByCity1(System::String^ x, System::String^ y) {
	try {
		array<System::String^>^ parts_x = x->Split(gcnew array<System::Char>{L';'});
		array<System::String^>^ parts_y = y->Split(gcnew array<System::Char>{L';'});
		if (parts_x->Length < 4 || parts_x->Length > 5 || parts_y->Length < 4 || parts_y->Length > 5)
			throw TariffException("Неверный формат строки в списке тарифов.");
		return System::String::Compare(parts_x[1], parts_y[1]);
	} 
	catch (...) {
		throw TariffException("Ошибка при сортировке тарифов по цене.");
	}
}

int ATE::CompareTariffsByCity2(System::String^ x, System::String^ y) {
	try {
		array<System::String^>^ parts_x = x->Split(gcnew array<System::Char>{L';'});
		array<System::String^>^ parts_y = y->Split(gcnew array<System::Char>{L';'});
		if (parts_x->Length < 4 || parts_x->Length > 5 || parts_y->Length < 4 || parts_y->Length > 5)
			throw TariffException("Неверный формат строки в списке тарифов.");
		return System::String::Compare(parts_x[2], parts_y[2]);
	}
	catch (...) {
		throw TariffException("Ошибка при сортировке тарифов по цене.");
	}
}

System::Collections::Generic::List<System::String^>^ ATE::SortTariffsByPrice(System::Collections::Generic::List<System::String^>^ tariff_list) {
	System::Collections::Generic::List<System::String^>^ sorted_list = gcnew System::Collections::Generic::List<System::String^>(tariff_list);
	sorted_list->Sort(gcnew System::Comparison<System::String^>(ATE::CompareTariffsByPrice));
	return sorted_list;
}

System::Collections::Generic::List<System::String^>^ ATE::SortTariffsByCity1(System::Collections::Generic::List<System::String^>^ tariff_list) {
	System::Collections::Generic::List<System::String^>^ sorted_list = gcnew System::Collections::Generic::List<System::String^>(tariff_list);
	sorted_list->Sort(gcnew System::Comparison<System::String^>(ATE::CompareTariffsByCity1));
	return sorted_list;
}

System::Collections::Generic::List<System::String^>^ ATE::SortTariffsByCity2(System::Collections::Generic::List<System::String^>^ tariff_list) {
	System::Collections::Generic::List<System::String^>^ sorted_list = gcnew System::Collections::Generic::List<System::String^>(tariff_list);
	sorted_list->Sort(gcnew System::Comparison<System::String^>(ATE::CompareTariffsByCity2));
	return sorted_list;
}