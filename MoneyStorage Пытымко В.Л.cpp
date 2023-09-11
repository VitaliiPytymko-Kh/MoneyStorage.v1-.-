#include <iostream>
#include <string>
#include <vector>
#include <set>

class financeacount {
public:
    financeacount(const std::string& name, double balance) : name(name), balance(balance) {}

    virtual void show() const = 0;
    virtual void addbalance(double amount) = 0;
    virtual void withdraw(double amount) = 0;

protected:
    std::string name;
    double balance;
};

class Card : public financeacount {
public:
    Card(const std::string& name, double balance) : financeacount(name, balance) {}

    void show() const override {
        std::cout << "Card Account: " << name << ", Balance: " << balance << std::endl;
    }

    void addbalance(double amount) override {
        balance += amount;
    }

    void withdraw(double amount) override {
        if (balance >= amount) {
            balance -= amount;
        }
        else {
            std::cout << "Insufficient funds in Card Account." << std::endl;
        }
    }
};
class Wallet : public financeacount {
public:
    Wallet(const std::string& name, double balance) : financeacount(name, balance) {}

    void show() const override {
        std::cout << "Wallet Account: " << name << ", Balance: " << balance << std::endl;
    }

    void addbalance(double amount) override {
        balance += amount;
    }

    void withdraw(double amount) override {
        if (balance >= amount) {
            balance -= amount;
        }
        else {
            std::cout << "Insufficient funds in Wallet Account." << std::endl;
        }
    }
};


enum SpendingCategory {
    Food,
    Entertainment,
    Transportation,
    Other
};

class Spending {
public:
    Spending(SpendingCategory category, double amount, const std::string& date)
        : category(category), amount(amount), date(date) {}

    bool operator<(const Spending& other) const {
        // Сравниваем по категории, сумме и дате
        if (category < other.category) {
            return true;
        }
        else if (category > other.category) {
            return false;
        }

        if (amount < other.amount) {
            return true;
        }
        else if (amount > other.amount) {
            return false;
        }

        return date < other.date;
    }

    void show() const {
        std::string categoryStr;
        switch (category) {
        case Food:
            categoryStr = "Food";
            break;
        case Entertainment:
            categoryStr = "Entertainment";
            break;
        case Transportation:
            categoryStr = "Transportation";
            break;
        case Other:
            categoryStr = "Other";
            break;
        }

        std::cout << "Category: " << categoryStr << ", Amount: " << amount << ", Date: " << date << std::endl;
    }

    SpendingCategory getCategory() const {
        return category;
    }

    double getAmount() const {
        return amount;
    }

    std::string getDate() const {
        return date;
    }

private:
    SpendingCategory category;
    double amount;
    std::string date;
};

// Интерфейс для управления тратами
class ISpendingManager {
public:
    virtual void addSpending(SpendingCategory category, double amount, const std::string& date) = 0;
    virtual void showAllSpendings() const = 0;
    virtual double getTotalSpending(SpendingCategory category) const = 0;
};

// Реализация управления тратами
class SpendingManager : public ISpendingManager {
public:
    SpendingManager(financeacount& account) : account(account) {}

    void addSpending(SpendingCategory category, double amount, const std::string& date) override {
        Spending spending(category, amount, date);
        spendings.insert(spending);
        account.withdraw(amount); // Управление тратами через снятие денег с аккаунта
    }

    void showAllSpendings() const override {
        for (const Spending& s : spendings) {
            s.show();
        }
    }

    double getTotalSpending(SpendingCategory category) const override {
        double total = 0.0;
        for (const Spending& s : spendings) {
            if (s.getCategory() == category) {
                total += s.getAmount();
            }
        }
        return total;
    }

private:
    financeacount& account;
    std::multiset<Spending> spendings;
};

int main() {
    Card c1("Visa", 1000.0);
    c1.show();

    Wallet w1("My Wallet", 2000.0);
    w1.show();

    SpendingManager cardSpendingManager(c1);
    SpendingManager walletSpendingManager(w1);

    cardSpendingManager.addSpending(Food, 50.0, "09.09.23");
    cardSpendingManager.addSpending(Entertainment, 30.0, "09.09.23");

    walletSpendingManager.addSpending(Food, 70.0, "09.09.23");
    walletSpendingManager.addSpending(Entertainment, 40.0, "09.09.23");

    cardSpendingManager.showAllSpendings();
    walletSpendingManager.showAllSpendings();

    c1.show();
    w1.show();
    return 0;
}
