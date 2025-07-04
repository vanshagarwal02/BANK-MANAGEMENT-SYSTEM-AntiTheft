#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <limits>
#include <openssl/sha.h> // For SHA-256
#include <map>
using namespace std;

class Transaction{
    string type;
    double amount;
    string date;
public:
    Transaction(const string& t, double amt, const string& d) : type(t), amount(amt), date(d) {}
    void display() const {
        cout << date << " | " << type << " | Amount: " << amount << endl;
    }
    // Add getters for AML access
    string getType() const { return type; }
    string getDate() const { return date; }
};

string hashPin(int pin) {
    string pinStr = to_string(pin);
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)pinStr.c_str(), pinStr.size(), hash);
    char buf[2*SHA256_DIGEST_LENGTH+1];
    buf[2*SHA256_DIGEST_LENGTH] = 0;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(buf + i*2, "%02x", hash[i]);
    return string(buf);
}

class Account{
    int accountNumber;
    string name;
    int age;
    double salary;
    string pinHash; // Store hashed PIN
    double balance;
    vector<Transaction> transactions;
public:
    Account(int accNo, const string& n, int a, double sal, int p, double bal = 0.0)
        : accountNumber(accNo), name(n), age(a), salary(sal), pinHash(hashPin(p)), balance(bal) {}
    int getAccountNumber() const { return accountNumber; }
    string getName() const { return name; }
    int getAge() const { return age; }
    double getSalary() const { return salary; }
    bool checkPin(int p) const { return pinHash == hashPin(p); }
    double getBalance() const { return balance; }
    void deposit(double amt);
    bool withdraw(double amt);
    void addTransaction(const string& type, double amt);
    void showTransactions() const;
};


// AML Red Flags
class AMLChecker {
public:
    static bool isLargeDeposit(double amt) {
        return amt > 50000;
    }
    static bool isGhostAccount(const Account& acc) {
        return (acc.getBalance() > 100000) && (acc.getName().empty() || acc.getAge() == 0);
    }
    static bool isFrequentTransactions(const vector<Transaction>& txs) {
        if (txs.size() < 11) return false;
        time_t now = time(0);
        int count = 0;
        for (auto it = txs.rbegin(); it != txs.rend(); ++it) {
            struct tm tm = {};
            strptime(it->getDate().c_str(), "%a %b %d %H:%M:%S %Y", &tm);
            time_t t = mktime(&tm);
            if (difftime(now, t) <= 60) count++;
            if (count > 10) return true;
        }
        return false;
    }
    static bool isCircularBehavior(const vector<Transaction>& txs) {
        if (txs.size() < 2) return false;
        const Transaction& last = txs[txs.size()-1];
        const Transaction& prev = txs[txs.size()-2];
        return (last.getType() == "Withdraw" && prev.getType() == "Deposit" && last.getDate() == prev.getDate());
    }
};

void Account::deposit(double amt){
    balance += amt;
    addTransaction("Deposit", amt);
    // AML check
    if (AMLChecker::isLargeDeposit(amt)) {
        cout << "[AML WARNING] Large deposit detected!\n";
    }
}

bool Account::withdraw(double amt){
    if (amt > balance) return false;
    balance -= amt;
    addTransaction("Withdraw", amt);
    // AML check for circular behavior
    if (AMLChecker::isCircularBehavior(transactions)) {
        cout << "[AML WARNING] Circular behavior detected!\n";
    }
    return true;
}

void Account::addTransaction(const string& type, double amt){
    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back(); // remove newline
    transactions.emplace_back(type, amt, dt);
    // AML check for frequent transactions
    if (AMLChecker::isFrequentTransactions(transactions)) {
        cout << "[AML WARNING] Frequent transactions detected!\n";
    }
}

void Account::showTransactions() const {
    cout << "Transaction History for Account " << accountNumber << ":\n";
    for (const auto& t : transactions) t.display();
}

class Bank{
    vector<Account> accounts;
    int nextAccountNumber = 1001;
public:
    void mainMenu();
    void userMenu(Account& acc);
    void createAccount();
    Account* login();
    Account* findAccount(int accNo);
    void showAllAccounts(); // New admin view
};

void Bank::mainMenu(){
    int choice;
    while (true) {
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Create Account\n2. Login\n3. Admin View (All Accounts)\n4. Exit\nEnter choice: ";
        cin >> choice;
        if (choice == 1) createAccount();
        else if (choice == 2) {
            Account* acc = login();
            if (acc) userMenu(*acc);
        } else if (choice == 3) showAllAccounts();
        else if (choice == 4) break;
        else cout << "Invalid choice!\n";
    }
}

void Bank::createAccount(){
    string name;
    int age, pin;
    double salary;
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter age: ";
    while (!(cin >> age)) {
        cout << "Invalid input. Enter a valid age: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (age < 18) {
        cout << "Account creation failed: Age must be at least 18.\n";
        return;
    }
    cout << "Enter salary: ";
    while (!(cin >> salary)) {
        cout << "Invalid input. Enter a valid salary: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (salary < 10000) {
        cout << "Account creation failed: Salary must be at least 10000.\n";
        return;
    }
    cout << "Set PIN: ";
    while (!(cin >> pin)) {
        cout << "Invalid input. Enter a valid PIN: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    accounts.emplace_back(nextAccountNumber++, name, age, salary, pin);
    cout << "Account created! Your account number is " << (nextAccountNumber-1) << endl;
}

Account* Bank::login() {
    int accNo, pin;
    cout << "Enter account number: "; cin >> accNo;
    cout << "Enter PIN: "; cin >> pin;
    for (auto& acc : accounts) {
        if (acc.getAccountNumber() == accNo && acc.checkPin(pin)) {
            cout << "Login successful!\n";
            return &acc;
        }
    }
    cout << "Invalid credentials!\n";
    return nullptr;
}

void Bank::userMenu(Account& acc) {
    int choice;
    while (true) {
        cout << "\n--- User Menu ---\n";
        cout << "1. Check Balance\n2. Deposit\n3. Withdraw\n4. Transaction History\n5. Logout\nEnter choice: ";
        cin >> choice;
        if (choice == 1) {
            cout << "Balance: " << acc.getBalance() << endl;
            if (AMLChecker::isGhostAccount(acc)) {
                cout << "[AML WARNING] Ghost account detected!\n";
            }
        } else if (choice == 2) {
            double amt; cout << "Enter amount: "; cin >> amt;
            acc.deposit(amt);
            cout << "Deposited!\n";
        } else if (choice == 3) {
            double amt; cout << "Enter amount: "; cin >> amt;
            if (acc.withdraw(amt)) cout << "Withdrawn!\n";
            else cout << "Insufficient balance!\n";
        } else if (choice == 4) acc.showTransactions();
        else if (choice == 5) break;
        else cout << "Invalid choice!\n";
    }
}

void Bank::showAllAccounts() {
    cout << "\n--- All Bank Accounts & Transactions ---\n";
    if (accounts.empty()) {
        cout << "No accounts found.\n";
        return;
    }
    for (const auto& acc : accounts) {
        cout << "\nAccount Number: " << acc.getAccountNumber() << endl;
        cout << "Name: " << acc.getName() << endl;
        cout << "Age: " << acc.getAge() << endl;
        cout << "Salary: " << acc.getSalary() << endl;
        cout << "Balance: " << acc.getBalance() << endl;
        acc.showTransactions();
    }
}

int main(){
    Bank bank;
    bank.mainMenu();
    return 0;
}
