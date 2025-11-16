#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <limits>
#include <Windows.h>
#include <string>

#include "Class.h"
#include "Function.h"

void show_menu() {
	std::cout << "==========МЕНЮ==========" << std::endl;
	std::cout << "1) добавить обычный тариф" << std::endl;
	std::cout << "2) добавить льготный тариф" << std::endl;
	std::cout << "3) вывести среднюю цену тарифов" << std::endl;
	std::cout << "4) вывести списки тарифов" << std::endl;
	std::cout << "5) сравнить тарифы" << std::endl;
	std::cout << "0) выход" << std::endl;
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
				if (t == 'u'){
					while (i1 < 1 || i1 > counts.first) {
						std::cout << "Неверный индекс. Введите целое число от 1 до " + std::to_string(counts.first) + ".\n";
						i1 = InputInt("введите номер первого тарифа: ");
					}
				} else if (t == 'd') {
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
				ate.compare(t, i1 - 1, i2 - 1);
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