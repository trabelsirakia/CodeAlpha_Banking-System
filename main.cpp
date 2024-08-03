#include <iostream>
#include <vector>
#include <string>

// Forward declarations
class Account;
class Customer;
class BankingService;

// Class definitions
class Transaction {
public:
    Transaction(const std::string& type, double amount)
        : type(type), amount(amount) {}

    std::string getDetails() const {
        return type + ": $" + std::to_string(amount);
    }

private:
    std::string type;
    double amount;
};

class Account {
public:
    Account(const std::string& accountNumber, double balance)
        : accountNumber(accountNumber), balance(balance) {}

    void deposit(double amount) {
        balance += amount;
        transactions.push_back(Transaction("Deposit", amount));
    }

    bool withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            transactions.push_back(Transaction("Withdrawal", amount));
            return true;
        }
        return false;
    }

    bool transfer(Account& toAccount, double amount) {
        if (withdraw(amount)) {
            toAccount.deposit(amount);
            return true;
        }
        return false;
    }

    double getBalance() const {
        return balance;
    }

    std::string getAccountNumber() const {
        return accountNumber;
    }

    void printAccountInfo() const {
        std::cout << "Account Number: " << accountNumber << "\n";
        std::cout << "Balance: $" << balance << "\n";
        std::cout << "Transactions:\n";
        for (const auto& transaction : transactions) {
            std::cout << "  " << transaction.getDetails() << "\n";
        }
    }

private:
    std::string accountNumber;
    double balance;
    std::vector<Transaction> transactions;
};

class Customer {
public:
    Customer(const std::string& name, const std::string& id)
        : name(name), id(id) {}

    std::string getId() const {
        return id;
    }

    void addAccount(const Account& account) {
        accounts.push_back(account);
    }

    std::vector<Account>& getAccounts() {
        return accounts;
    }

    const std::vector<Account>& getAccounts() const {
        return accounts;
    }

    void printCustomerInfo() const {
        std::cout << "Customer Name: " << name << "\n";
        std::cout << "Customer ID: " << id << "\n";
        std::cout << "Accounts:\n";
        for (const auto& account : accounts) {
            account.printAccountInfo();
            std::cout << "----------------------\n";
        }
    }

private:
    std::string name;
    std::string id;
    std::vector<Account> accounts;
};

class BankingService {
public:
    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }

    Customer* findCustomerById(const std::string& id) {
        for (auto& customer : customers) {
            if (customer.getId() == id) {
                return &customer;
            }
        }
        return nullptr;
    }

    Account* findAccountByNumber(const std::string& accountNumber) {
        for (auto& customer : customers) {
            for (auto& account : customer.getAccounts()) {
                if (account.getAccountNumber() == accountNumber) {
                    return &account;
                }
            }
        }
        return nullptr;
    }

    const std::vector<Customer>& getCustomers() const {
        return customers;
    }

private:
    std::vector<Customer> customers;
};

// Function prototypes
void displayMenu();
void manageCustomers(BankingService& bankingService);
void manageAccounts(Customer& customer);
void performTransactions(BankingService& bankingService, Customer& customer);

int main() {
    BankingService bankingService;

    // Create some customers and accounts for demonstration
    Customer customer1("Alice", "C001");
    Customer customer2("Bob", "C002");
    Account account1("A001", 1000.0);
    Account account2("A002", 500.0);
    customer1.addAccount(account1);
    customer2.addAccount(account2);
    bankingService.addCustomer(customer1);
    bankingService.addCustomer(customer2);

    int choice;
    while (true) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string id;
                std::cout << "Enter Customer ID: ";
                std::cin >> id;
                Customer* customer = bankingService.findCustomerById(id);
                if (customer) {
                    manageAccounts(*customer);
                    performTransactions(bankingService, *customer);
                } else {
                    std::cout << "Customer not found.\n";
                }
                break;
            }
            case 2: {
                manageCustomers(bankingService);
                break;
            }
            case 3:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

void displayMenu() {
    std::cout << "\nBanking System Dashboard\n";
    std::cout << "1. Manage Accounts and Transactions\n";
    std::cout << "2. Manage Customers\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice: ";
}

void manageCustomers(BankingService& bankingService) {
    std::cout << "\nCustomer Management:\n";
    for (const auto& customer : bankingService.getCustomers()) {
        customer.printCustomerInfo();
    }
}

void manageAccounts(Customer& customer) {
    std::cout << "\nAccount Management:\n";
    customer.printCustomerInfo();
}

void performTransactions(BankingService& bankingService, Customer& customer) {
    int choice;
    while (true) {
        std::cout << "\nTransaction Options:\n";
        std::cout << "1. Deposit\n";
        std::cout << "2. Withdraw\n";
        std::cout << "3. Transfer\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                double amount;
                std::cout << "Enter deposit amount: ";
                std::cin >> amount;
                if (amount > 0) {
                    customer.getAccounts()[0].deposit(amount);
                    std::cout << "Deposit successful.\n";
                } else {
                    std::cout << "Invalid amount.\n";
                }
                break;
            }
            case 2: {
                double amount;
                std::cout << "Enter withdrawal amount: ";
                std::cin >> amount;
                if (amount > 0 && customer.getAccounts()[0].withdraw(amount)) {
                    std::cout << "Withdrawal successful.\n";
                } else {
                    std::cout << "Insufficient funds or invalid amount.\n";
                }
                break;
            }
            case 3: {
                double amount;
                std::string toAccountNumber;
                std::cout << "Enter transfer amount: ";
                std::cin >> amount;
                std::cout << "Enter recipient account number: ";
                std::cin >> toAccountNumber;
                Account* recipientAccount = bankingService.findAccountByNumber(toAccountNumber);
                if (recipientAccount && amount > 0 && customer.getAccounts()[0].transfer(*recipientAccount, amount)) {
                    std::cout << "Transfer successful.\n";
                } else {
                    std::cout << "Transfer failed.\n";
                }
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

