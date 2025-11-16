#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <limits>

#include "Function.h"

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