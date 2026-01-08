#include "transaction.hpp"
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;


void appendTransaction(const Transaction& t) {
    ofstream fout("transactions.txt", ios::app);
    if (!fout.is_open()) return;
    fout << t.username << "|"
        << t.timestamp << "|"
        << t.type << "|"
        << fixed << setprecision(2) << t.amount << "|"
        << t.detail << "\n";
}

vector<Transaction> loadTransactionsForUser(const string& username) {
    vector<Transaction> res;
    ifstream fin("transactions.txt");
    if (!fin.is_open()) return res;

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);
        if (p4 == string::npos) continue;

        Transaction t{};
        t.username = line.substr(0, p1);
        t.timestamp = line.substr(p1 + 1, p2 - p1 - 1);
        t.type = line.substr(p2 + 1, p3 - p2 - 1);
        try { t.amount = stod(line.substr(p3 + 1, p4 - p3 - 1)); }
        catch (...) { t.amount = 0.0; }
        t.detail = line.substr(p4 + 1);

        if (t.username == username)
            res.push_back(t);
    }
    

    return res;
}
