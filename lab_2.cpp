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
        if (price < 0) throw invalid_argument("���� �� ������ �� ����� ���� �������������.");
        if (c.empty()) throw invalid_argument("����� ������ �� ����� ���� ������.");
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
        if (dur <= 0) throw invalid_argument("������������ ������ ������ ���� �������������.");
        if (dest.empty()) throw invalid_argument("����� ���������� �� ����� ���� ������.");
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
        if (n.empty()) throw invalid_argument("��� ������� �� ����� ���� ������.");
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

// ����� ��� � ������������, ���������� ��� Singleton
class ATE {
private:
    vector<Customer> customers;
    vector<Tariff> tariffs;
    ATE() = default;
    ATE(const ATE&) = delete;
    ATE& operator=(const ATE&) = delete;
    // ��������������� �������: ����� ������ �� ������ � ���� �������
    const Tariff* findTariff(const string& city, CustomerType type) const {
        for (const auto& t : tariffs) {
            if (t.getCity() == city && t.getCustomerType() == type) {
                return &t;
            }
        }
        return nullptr;
    }
public:
    // ������������ ����� ������� � ����������
    static ATE& getInstance() {
        static ATE instance;
        return instance;
    }
    ~ATE() {
        cout << "���������� ATC ������. ��� ������ �������." << endl;
    }
    void addCustomer(const string& name, CustomerType type) {
        try {
            customers.emplace_back(name, type);
            cout << "������ '" << name << "' ��������." << endl;
        }
        catch (const exception& e) {
            cerr << "������: " << e.what() << endl;
        }
    }
    void addTariff(const string& city, double price, CustomerType type) {
        try {
            tariffs.emplace_back(city, price, type);
            cout << "����� ��� ������ '" << city << "' ��������." << endl;
        }
        catch (const exception& e) {
            cerr << "������: " << e.what() << endl;
        }
    }
    void registerCall(const string& customerName, const string& destination, int duration) {
        auto it = find_if(customers.begin(), customers.end(),
            [&customerName](const Customer& c) {
                return c.getName() == customerName;
            });
        if (it == customers.end()) {
            cerr << "������ '" << customerName << "' �� ������." << endl;
            return;
        }
        const Tariff* tariff = findTariff(destination, it->getType());
        if (!tariff) {
            cerr << "����� ��� ������ '" << destination << "' � ���� ������� �� ������." << endl;
            return;
        }
        try {
            Call call(duration, destination, tariff);
            it->addCall(call);
            cout << "������ ��������������� ��� ������� '" << customerName << "'." << endl;
        }
        catch (const exception& e) {
            cerr << "������: " << e.what() << endl;
        }
    }
    double calculateCustomerTotal(const string& customerName) const {
        auto it = find_if(customers.begin(), customers.end(),
            [&customerName](const Customer& c) {
                return c.getName() == customerName;
            });
        if (it == customers.end()) {
            cerr << "������ '" << customerName << "' �� ������." << endl;
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
        cout << "\n=== ������ �������� ===\n";
        for (const auto& c : customers) {
            cout << c.getName() << " (��� - " << (c.getType() == CustomerType::VIP ? "VIP" : "�������") << "): " << "����� ��������� ������� = " << c.getTotalCost() << endl;
        }
    }
    void listTariffs() const {
        cout << "\n=== ������ ������� ===\n";
        for (const auto& t : tariffs) {
            cout << "����� = " << t.getCity() << "; ����/��� = " << t.getPricePerMinute() << "; ��� ������� = " << (t.getCustomerType() == CustomerType::VIP ? "VIP" : "�������") << endl;
        }
    }
};

// �������� �����
string getInputString(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    while (s.empty()) {
        cout << "���� �� ����� ���� ������.\n" << prompt;
        getline(cin, s);
    }
    return s;
}
int getInputInt(const string& prompt) {
    int n;
    cout << prompt;
    while (!(cin >> n) || n < 0) {
        cout << "������� ����� ��������������� ����� �����.\n" << prompt;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return n;
}
double getInputDouble(const string& prompt) {
    double d;
    cout << prompt;
    while (!(cin >> d) || d < 0) {
        cout << "������� ������������ ��������������� �����.\n" << prompt;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return d;
}
CustomerType getInputCustomerType() {
    cout << "�������� ��� �������:\n1. VIP\n2. �������\n��� �����: ";
    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)) {
        cout << "�������� �����. ������� 1 ��� 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();
    return (choice == 1) ? CustomerType::VIP : CustomerType::Usual;
}

// ������� ����
void showMenu() {
    cout << "\n========== ���� ��� ==========\n";
    cout << "1. �������� �������\n";
    cout << "2. �������� �����\n";
    cout << "3. ���������������� ������\n";
    cout << "4. ���������� ��������� ������� �������\n";
    cout << "5. ���������� ����� ��������� ���� �������\n";
    cout << "6. �������� ������ ��������\n";
    cout << "7. �������� ������ �������\n";
    cout << "0. �����\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    int choice;
    showMenu();
    do {
        choice = getInputInt("\n�������� ��������: ");
        cin.ignore();
        switch (choice) {
        case 1: {
            string name = getInputString("������� ��� �������: ");
            CustomerType type = getInputCustomerType();
            ATE::getInstance().addCustomer(name, type);
            break;
        }
        case 2: {
            string city = getInputString("������� ����� ��� ������: ");
            double price = getInputDouble("������� ���� �� ������: ");
            CustomerType type = getInputCustomerType();
            ATE::getInstance().addTariff(city, price, type);
            break;
        }
        case 3: {
            string name = getInputString("������� ��� �������: ");
            string dest = getInputString("������� ����� ����������: ");
            int dur = getInputInt("������� ������������ ������ (���): ");
            ATE::getInstance().registerCall(name, dest, dur);
            break;
        }
        case 4: {
            string name = getInputString("������� ��� �������: ");
            double total = ATE::getInstance().calculateCustomerTotal(name);
            if (total >= 0) {
                cout << "����� ��������� ������� ��� '" << name << "' = " << total << endl;
            }
            break;
        }
        case 5: {
            double total = ATE::getInstance().calculateAllCallsTotal();
            cout << "����� ��������� ���� ������� �� ��� = " << total << endl;
            break;
        }
        case 6:
            ATE::getInstance().listCustomers();
            break;
        case 7:
            ATE::getInstance().listTariffs();
            break;
        case 0:
            cout << "����� �� ���������." << endl;
            break;
        default:
            cout << "�������� �����. ���������� �����." << endl;
        }
    } while (choice != 0);

    return 0;
}