﻿#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdlib>
#include <ctime>

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

    void showSpendingsForDate(const std::string& date) const {
        std::cout << "Траты для даты " << date << ":" << std::endl;
        bool foundSpendings = false; // Флаг для отслеживания наличия трат за дату
        for (const Spending& s : spendings) {
            if (s.getDate() == date) {
                s.show();
                foundSpendings = true; // Устанавливаем флаг, если найдены траты
            }
        }

        if (!foundSpendings) {
            std::cout << "Траты на указанную дату отсутствуют." << std::endl;
        }
    }
    void addSpending(SpendingCategory category, double amount, const std::string& date) override {
        Spending spending(category, amount, date);
        spendings.insert(spending);
        account.withdraw(amount);

        // Update category statistics
        categoryStatistics[category] += amount;
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
   
    void showCategoryStatistics() const {
        std::cout << "Category Statistics:" << std::endl;
        for (const auto& pair : categoryStatistics) {
            SpendingCategory category = pair.first;
            double totalAmount = pair.second;

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

            std::cout << "Category: " << categoryStr << ", Total Amount: " << totalAmount << std::endl;
        }
    }
    void generateCategoryReport() const {
        std::cout << "Spending Report by Category:" << std::endl;
        for (const auto& pair : categoryStatistics) {
            SpendingCategory category = pair.first;
            double totalAmount = pair.second;

            std::cout << "Category: ";
            switch (category) {
            case Food:
                std::cout << "Food";
                break;
            case Entertainment:
                std::cout << "Entertainment";
                break;
            case Transportation:
                std::cout << "Transportation";
                break;
            case Other:
                std::cout << "Other";
                break;
            }

            std::cout << ", Total Amount: " << totalAmount << std::endl;
        }
    }

private:
    financeacount& account;
    std::multiset<Spending> spendings;
    std::map<SpendingCategory, double> categoryStatistics;
};

int main() {
   
    setlocale(LC_ALL,"");
        // Создаем карту с начальным балансом в $1000
        Card cardAccount("Карта Джона", 100000.0);
        cardAccount.show();

        // Создаем менеджер расходов для карты
        SpendingManager cardSpendingManager(cardAccount);

        // Задаем начальную случайную зерновую последовательность
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // Создаем массив дней и генерируем траты в каждый день
        const int numDays = 10;
        for (int day = 1; day <= numDays; ++day) {
           
            const int numSpendings = std::rand() % 6; // Случайное количество трат (от 0 до 5)

            for (int spending = 0; spending < numSpendings; ++spending) {
                // Случайно выбираем категорию траты (от 0 до 3)
                SpendingCategory category = static_cast<SpendingCategory>(std::rand() % 4);

                // Генерируем случайную сумму траты (от 10 до 100)
                double amount = 10.0 + (std::rand() % 91);

                // Генерируем случайную дату (просто номер дня)
                std::string date = "2023-09-" + std::to_string(day);

                // Добавляем трату в менеджер расходов
                cardSpendingManager.addSpending(category, amount, date);
            }
        }

        // Показываем все расходы
       // cardSpendingManager.showAllSpendings();

        // Показываем статистику по категориям
       //cardSpendingManager.showCategoryStatistics();

        // Генерируем отчет о расходах по категориям
      // cardSpendingManager.generateCategoryReport();
      // cardAccount.show();

      // std::string specificDate = "2023-09-3";
    //   cardSpendingManager.showSpendingsForDate(specificDate);

        return 0;
    }
//добавить отчеты и статистику:
// код, который собирает статистику или создает отчеты на основе категорий расходов
