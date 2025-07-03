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
