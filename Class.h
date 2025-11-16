#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <limits>

class TariffException : public std::invalid_argument {
public:
	explicit TariffException(const std::string& message);
};

class PriceStrategy {
public:
	virtual ~PriceStrategy();
	virtual double calculate_price(double price) const = 0;
};

class NoDiscountStrategy : public PriceStrategy {
public:
	double calculate_price(double price) const override;
};

class DiscountStrategy : public PriceStrategy {
private:
	double discount;
public:
	explicit DiscountStrategy(double d);
	double calculate_price(double price) const override;
};

class Tariff {
protected:
	double price;
	std::string city1;
	std::string city2;
public:
	Tariff(double p, std::string c1, std::string c2);
	virtual ~Tariff();
	double get_base_price() const;
	std::string get_city1() const;
	std::string get_city2() const;
};

class UsualTariff : public Tariff, public NoDiscountStrategy {
public:
	UsualTariff(double p, std::string c1, std::string c2);
	bool is_greater(UsualTariff);
};

class DiscountTariff : public Tariff, public DiscountStrategy {
public:
	DiscountTariff(double p, double discount, std::string c1, std::string c2);
	bool is_less(DiscountTariff);
};

class ATE {
private:
	std::vector<UsualTariff*> usual_tariffs;
	std::vector<DiscountTariff*> discount_tariffs;
	double total = 0.0;
public:
	void add_usual_tariff(double price, const std::string& city1, const std::string& city2);
	void add_discount_tariff(double price, double discount, const std::string& city1, const std::string& city2);
	ATE& operator += (const UsualTariff& tariff);
	ATE& operator += (const DiscountTariff& tariff);
	double calculate_average_price();
	std::pair<int,int> show_tariff_list() const;
	void compare(char type, int index1, int index2);
};