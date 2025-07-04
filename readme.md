# 🏦 Bank Management System (C++ CLI Project with AML)

A simple, secure, and extensible **Bank Management System** built in **C++** using Object-Oriented Programming (OOP) principles. This project features a **Command-Line Interface (CLI)** for user interaction, secure PIN storage, and advanced **Anti-Money Laundering (AML)** detection.

---

## 🚀 Features

- 🔐 **Secure Login** with Account Number and PIN (PINs are hashed using SHA-256)
- 🧾 **Transactions**: Deposit & Withdraw with real-time balance updates
- 🕒 **Timestamped Transaction History** for every account
- ✅ **Account Validation**: Age (18+) and Salary (10,000+) checks during account creation
- 📦 **Multi-user support** (all accounts and transactions are stored in memory for the session)
- 🛡️ **PIN Hashing**: Uses OpenSSL's SHA-256 for secure PIN storage and verification
- 🖥️ **Simple CLI Interface**: Easy to use, menu-driven navigation
- 🕵️‍♂️ **AML (Anti-Money Laundering) Detection**: Flags suspicious activity in real time
- 👀 **Admin View**: Print all account details and full transaction histories for every user

---

## 🛡️ AML (Anti-Money Laundering) Detection

The system automatically checks for suspicious activity after every transaction and prints a warning if a red flag is detected. Implemented AML rules:

| Rule                  | Description                                                        |
|-----------------------|--------------------------------------------------------------------|
| 🚩 Large deposits     | Deposit amount > ₹50,000                                            |
| 🌀 Circular behavior  | Deposit immediately followed by a withdrawal (same timestamp)        |
| 📊 Frequent txns      | More than 10 transactions within a minute                           |
| 👻 Ghost accounts     | High balance but missing name or age                                |

**How it works:**
- After each deposit/withdrawal, the system checks for these red flags and prints `[AML WARNING] ...` if detected.
- Admins can view all accounts and their full transaction histories, including timestamps, to audit for suspicious activity.

---

## 🛠️ Requirements

- C++ compiler (g++)
- OpenSSL development libraries (`libssl-dev` on Ubuntu/Debian)

---

## ⚙️ How to Build

```sh
g++ Bank_System.cpp -lssl -lcrypto -o bank_app
```

---

## ▶️ How to Run

```sh
./bank_app
```

---

## 👤 User Menu

- **Create Account**: Enter name, age, salary, and set a PIN (securely hashed)
- **Login**: Enter account number and PIN
- **Check Balance**: View your current balance
- **Deposit/Withdraw**: Add or remove funds (AML checks run automatically)
- **Transaction History**: See all your transactions with timestamps
- **Logout**: Return to main menu

---

## 🛡️ AML Red Flags (How to Test)

- **Large Deposit**: Deposit more than ₹50,000 to see a warning
- **Frequent Transactions**: Make more than 10 transactions in a minute
- **Circular Behavior**: Deposit and immediately withdraw the same amount
- **Ghost Account**: (For testing, create an account with missing name/age in code)

---

## 👨‍💼 Admin View (Print All Accounts)

- From the main menu, select `Admin View (All Accounts)`
- The system will print all account details:
  - Account Number, Name, Age, Salary, Balance
  - Full transaction history with timestamps for each account

---

## 📝 Notes

- Make sure OpenSSL is installed:
  - On Ubuntu/Debian: `sudo apt-get install libssl-dev`
- If you get linker errors about `SHA256`, add `-lssl -lcrypto` to your compile command.
- All data is stored in memory for the session (no file/database persistence).

---

## 💡 Example Usage

```
--- Bank Management System ---
1. Create Account
2. Login
3. Admin View (All Accounts)
4. Exit
Enter choice: 1
Enter name: Alice
Enter age: 25
Enter salary: 50000
Set PIN: 4321
Account created! Your account number is 1001

--- Bank Management System ---
1. Create Account
2. Login
3. Admin View (All Accounts)
4. Exit
Enter choice: 3

--- All Bank Accounts & Transactions ---
Account Number: 1001
Name: Alice
Age: 25
Salary: 50000
Balance: 0
Transaction History for Account 1001:
(No transactions yet)
```

---
