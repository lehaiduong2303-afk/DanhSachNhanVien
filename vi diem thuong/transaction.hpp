#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <vector>

struct Transaction {
    std::string username;
    std::string timestamp;
    std::string type;
    double amount;
    std::string detail;
};

std::string nowStr();  
void appendTransaction(const Transaction& t);
std::vector<Transaction> loadTransactionsForUser(const std::string& username);

#endif
