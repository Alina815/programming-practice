#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <limits>
using namespace std;

enum class CustomerType {VIP, Usual};

class Tariff {
private:
    string city;
    double pricePerMinute;
    CustomerType customerType;
public:
    Tariff(const string& c, double price, CustomerType ct)
        : city(c), pricePerMinute(price), customerType(ct) {
        if (price < 0) throw invalid_argument("Цена за минуту не может быть отрицательной.");
        if (c.empty()) throw invalid_argument("Город тарифа не может быть пустым.");
    }
    string getCity() const {return city;}
    double getPricePerMinute() const {return pricePerMinute;}
    CustomerType getCustomerType() const {return customerType;}
};

class Call {
private:
    int duration;
    string destination;
    const Tariff* tariff;
public:
    Call(int dur, const string& dest, const Tariff* t)
        : duration(dur), destination(dest), tariff(t) {
        if (dur <= 0) throw invalid_argument("Длительность звонка должна быть положительной.");
        if (dest.empty()) throw invalid_argument("Город назначения не может быть пустым.");
    }
    double calculateCost() const {
        if (!tariff) return 0.0;
        return duration * tariff->getPricePerMinute();
    }
    string getDestination() const { return destination; }
    int getDuration() const { return duration; }
};

class Customer {
private:
    string name;
    CustomerType type;
    vector<Call> calls;
public:
    Customer(const string& n, CustomerType t)
        : name(n), type(t) {
        if (n.empty()) throw invalid_argument("Имя клиента не может быть пустым.");
    }
    void addCall(const Call& call) {
        calls.push_back(call);
    }
    double getTotalCost() const {
        double total = 0.0;
        for (const auto& call : calls) {
            total += call.calculateCost();
        }
        return total;
    }
    string getName() const {return name;}
    CustomerType getType() const {return type;}
    const vector<Call>& getCalls() const {return calls;}
};

// Класс АТС — контейнерный, реализован как Singleton
class ATE {
private:
    vector<Customer> customers;
    vector<Tariff> tariffs;
    ATE() = default;
    ATE(const ATE&) = delete;
    ATE& operator=(const ATE&) = delete;
    // Вспомогательная функция: поиск тарифа по городу и типу клиента
    const Tariff* findTariff(const string& city, CustomerType type) const {
        for (const auto& t : tariffs) {
            if (t.getCity() == city && t.getCustomerType() == type) {
                return &t;
            }
        }
        return nullptr;
    }
public:
    // Единственная точка доступа к экземпляру
    static ATE& getInstance() {
        static ATE instance;
        return instance;
    }
    ~ATE() {
        cout << "Деструктор ATC вызван. Все данные удалены." << endl;
    }
    void addCustomer(const string& name, CustomerType type) {
        try {
            customers.emplace_back(name, type);
            cout << "Клиент '" << name << "' добавлен." << endl;
        }
        catch (const exception& e) {
            cerr << "ОШИБКА: " << e.what() << endl;
        }
    }
    void addTariff(const string& city, double price, CustomerType type) {
        try {
            tariffs.emplace_back(city, price, type);
            cout << "Тариф для города '" << city << "' добавлен." << endl;
        }
        catch (const exception& e) {
            cerr << "ОШИБКА: " << e.what() << endl;
        }
    }
    void registerCall(const string& customerName, const string& destination, int duration) {
        auto it = find_if(customers.begin(), customers.end(),
            [&customerName](const Customer& c) {
                return c.getName() == customerName;
            });
        if (it == customers.end()) {
            cerr << "Клиент '" << customerName << "' не найден." << endl;
            return;
        }
        const Tariff* tariff = findTariff(destination, it->getType());
        if (!tariff) {
            cerr << "Тариф для города '" << destination << "' и типа клиента не найден." << endl;
            return;
        }
        try {
            Call call(duration, destination, tariff);
            it->addCall(call);
            cout << "Звонок зарегистрирован для клиента '" << customerName << "'." << endl;
        }
        catch (const exception& e) {
            cerr << "ОШИБКА: " << e.what() << endl;
        }
    }
    double calculateCustomerTotal(const string& customerName) const {
        auto it = find_if(customers.begin(), customers.end(),
            [&customerName](const Customer& c) {
                return c.getName() == customerName;
            });
        if (it == customers.end()) {
            cerr << "Клиент '" << customerName << "' не найден." << endl;
            return -1.0;
        }
        return it->getTotalCost();
    }
    double calculateAllCallsTotal() const {
        double total = 0.0;
        for (const auto& customer : customers) {
            total += customer.getTotalCost();
        }
        return total;
    }
    void listCustomers() const {
        cout << "\n=== Список клиентов ===\n";
        for (const auto& c : customers) {
            cout << c.getName() << " (тип - " << (c.getType() == CustomerType::VIP ? "VIP" : "Обычный") << "): " << "общая стоимость звонков = " << c.getTotalCost() << endl;
        }
    }
    void listTariffs() const {
        cout << "\n=== Список тарифов ===\n";
        for (const auto& t : tariffs) {
            cout << "город = " << t.getCity() << "; цена/мин = " << t.getPricePerMinute() << "; тип клиента = " << (t.getCustomerType() == CustomerType::VIP ? "VIP" : "Обычный") << endl;
        }
    }
};

// Проверки ввода
string getInputString(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    while (s.empty()) {
        cout << "Поле не может быть пустым.\n" << prompt;
        getline(cin, s);
    }
    return s;
}
int getInputInt(const string& prompt) {
    int n;
    cout << prompt;
    while (!(cin >> n) || n < 0) {
        cout << "Введите целое неотрицательное целое число.\n" << prompt;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return n;
}
double getInputDouble(const string& prompt) {
    double d;
    cout << prompt;
    while (!(cin >> d) || d < 0) {
        cout << "Введите рациональное неотрицательное число.\n" << prompt;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return d;
}
CustomerType getInputCustomerType() {
    cout << "Выберите тип клиента:\n1. VIP\n2. Обычный\nВаш выбор: ";
    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "Неверный выбор. Введите 1 или 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();
    return (choice == 1) ? CustomerType::VIP : CustomerType::Usual;
}

// Главное меню
void showMenu() {
    cout << "\n========== МЕНЮ АТС ==========\n";
    cout << "1. Добавить клиента\n";
    cout << "2. Добавить тариф\n";
    cout << "3. Зарегистрировать звонок\n";
    cout << "4. Рассчитать стоимость звонков клиента\n";
    cout << "5. Рассчитать общую стоимость всех звонков\n";
    cout << "6. Показать список клиентов\n";
    cout << "7. Показать список тарифов\n";
    cout << "0. Выход\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    int choice;
    showMenu();
    do {
        choice = getInputInt("\nВыберите действие: ");
        cin.ignore();
        switch (choice) {
        case 1: {
            string name = getInputString("Введите имя клиента: ");
            CustomerType type = getInputCustomerType();
            ATE::getInstance().addCustomer(name, type);
            break;
        }
        case 2: {
            string city = getInputString("Введите город для тарифа: ");
            double price = getInputDouble("Введите цену за минуту: ");
            CustomerType type = getInputCustomerType();
            ATE::getInstance().addTariff(city, price, type);
            break;
        }
        case 3: {
            string name = getInputString("Введите имя клиента: ");
            string dest = getInputString("Введите город назначения: ");
            int dur = getInputInt("Введите длительность звонка (мин): ");
            ATE::getInstance().registerCall(name, dest, dur);
            break;
        }
        case 4: {
            string name = getInputString("Введите имя клиента: ");
            double total = ATE::getInstance().calculateCustomerTotal(name);
            if (total >= 0) {
                cout << "Общая стоимость звонков для '" << name << "' = " << total << endl;
            }
            break;
        }
        case 5: {
            double total = ATE::getInstance().calculateAllCallsTotal();
            cout << "Общая стоимость всех звонков на АТС = " << total << endl;
            break;
        }
        case 6:
            ATE::getInstance().listCustomers();
            break;
        case 7:
            ATE::getInstance().listTariffs();
            break;
        case 0:
            cout << "Выход из программы." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    } while (choice != 0);

    return 0;
}