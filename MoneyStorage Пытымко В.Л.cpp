#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

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

    // Метод для сохранения трат за указанный период в файл
    void SaveSpendingsForPeriod(const std::string& startDate, const std::string& endDate, const std::string& filename) const {
        std::ofstream outputFile(filename); // Открываем файл для записи

        if (!outputFile.is_open()) {
            std::cerr << "Ошибка открытия файла для записи." << std::endl;
            return;
        }

        outputFile << "Траты за период с " << startDate << " по " << endDate << ":" << std::endl;
        bool foundSpendings = false; // Флаг для отслеживания наличия трат за период
        for (const Spending& s : spendings) {
            if (s.getDate() >= startDate && s.getDate() <= endDate) {
                // Вместо вывода на консоль, записываем в файл
                outputFile << "Category: " << GetCategoryString(s.getCategory()) << ", Amount: " << s.getAmount() << ", Date: " << s.getDate() << std::endl;
                foundSpendings = true; // Устанавливаем флаг, если найдены траты
            }
        }

        if (!foundSpendings) {
            outputFile << "Траты в указанный период отсутствуют." << std::endl;
        }

        outputFile.close(); // Закрываем файл после записи
        std::cout<< "Отчет сохранен в файл : "<< filename<< std::endl;
    }

    // Вспомогательная функция для получения строкового представления категории
    std::string GetCategoryString(SpendingCategory category) const {
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
        return categoryStr;
    }


    void showTop3CategorySpendings(const std::string& startDate, const std::string& endDate) const {
        std::map<SpendingCategory, double> categorySpending; // Словарь для хранения сумм трат по категориям

        // Вычисляем суммы трат для каждой категории за указанный период
        for (const Spending& s : spendings) {
            if (s.getDate() >= startDate && s.getDate() <= endDate) {
                categorySpending[s.getCategory()] += s.getAmount();
            }
        }

        // Создаем вектор пар (категория, сумма) и сортируем его в порядке убывания сумм
        std::vector<std::pair<SpendingCategory, double>> sortedCategorySpending(categorySpending.begin(), categorySpending.end());
        std::sort(sortedCategorySpending.begin(), sortedCategorySpending.end(),
            [](const std::pair<SpendingCategory, double>& a, const std::pair<SpendingCategory, double>& b) {
                return a.second > b.second;
            });

        // Выводим топ-3 категории с суммами
        std::cout << "Топ-3 категории трат за период с " << startDate << " по " << endDate << ":" << std::endl;
        for (size_t i = 0; i < std::min(sortedCategorySpending.size(), 3u); ++i) {
            const SpendingCategory& category = sortedCategorySpending[i].first;
            const double amount = sortedCategorySpending[i].second;

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

            std::cout << "Category: " << categoryStr << ", Total Amount: " << amount << std::endl;
        }
    }

    std::vector<Spending> getTopSpending(const std::string& startDate, const std::string& endDate) const {
        std::vector<Spending> topSpendings;

        // Создаем вектор и фильтруем траты для указанного периода
        std::vector<Spending> spendingsForPeriod;
        for (const Spending& s : spendings) {
            if (s.getDate() >= startDate && s.getDate() <= endDate) {
                spendingsForPeriod.push_back(s);
            }
        }

        // Сортируем траты для указанного периода по убыванию суммы
        std::sort(spendingsForPeriod.begin(), spendingsForPeriod.end(),
            [](const Spending& a, const Spending& b) {
                return a.getAmount() > b.getAmount();
            });

        // Находим топ-3 траты по максимальным суммам
        for (size_t i = 0; i < std::min(spendingsForPeriod.size(), 3u); ++i) {
            topSpendings.push_back(spendingsForPeriod[i]);
        }

        return topSpendings;
    }

    // Метод для вывода топ-3 трат за указанный период
    void showTop3Spendings(const std::string& startDate, const std::string& endDate) const {
        std::vector<Spending> topSpendings = getTopSpending(startDate, endDate);
        std::cout << "Топ-3 траты за период с " << startDate << " по " << endDate << ":" << std::endl;
        for (const Spending& s : topSpendings) {
            s.show();
        }
    }

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
    void showSpendingsForPeriod(const std::string& startDate, const std::string& endDate) const {
        std::cout << "Траты за период с " << startDate << " по " << endDate << ":" << std::endl;
        bool foundSpendings = false; // Флаг для отслеживания наличия трат за период
        for (const Spending& s : spendings) {
            if (s.getDate() >= startDate && s.getDate() <= endDate) {
                s.show();
                foundSpendings = true; // Устанавливаем флаг, если найдены траты
            }
        }

        if (!foundSpendings) {
            std::cout << "Траты в указанный период отсутствуют." << std::endl;
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
        cardSpendingManager.showAllSpendings();

       //  Показываем статистику по категориям
       cardSpendingManager.showCategoryStatistics();

        // Генерируем отчет о расходах по категориям
      cardSpendingManager.generateCategoryReport();
       cardAccount.show();

       std::string specificDate1 = "2023-09-2";
       //cardSpendingManager.showSpendingsForDate(specificDate1);
       std::string specificDate2= "2023-09-5";

       cardSpendingManager.showSpendingsForPeriod(specificDate1, specificDate2);

       // Получить топ-3 трат за период с 2023-09-02 по 2023-09-05
       std::string startDate = "2023-09-2";
       std::string endDate = "2023-09-5";
       cardSpendingManager.showTop3Spendings(startDate, endDate);
       cardSpendingManager.showTop3CategorySpendings(startDate, endDate);
       std::string filename = "spendings_forPeriod_report.txt";
       // Сохраните отчет в файл
       cardSpendingManager.SaveSpendingsForPeriod(startDate, endDate, filename);

       

       
        return 0;
    }
