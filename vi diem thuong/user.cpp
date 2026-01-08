#define _CRT_SECURE_NO_WARNINGS
#include "user.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "fake_sodium.hpp"

using namespace std;

User::User() : wallet(0.0) {}

User::User(const string& u, const string& hash, const string& e,
    const string& p, const string& r, double w)
    : username(u), password_hash(hash), email(e), phone(p), role(r), wallet(w) {
}

string User::getUsername() const { return username; }
string User::getPasswordHash() const { return password_hash; }
string User::getEmail() const { return email; }
string User::getPhone() const { return phone; }
string User::getRole() const { return role; }
double User::getWallet() const { return wallet; }

void User::setUsername(const string& u) { username = u; }
void User::setPasswordHash(const string& h) { password_hash = h; }
void User::setEmail(const string& e) { email = e; }
void User::setPhone(const string& p) { phone = p; }
void User::setRole(const string& r) { role = r; }
void User::setWallet(double w) { wallet = w; }

// ======================= FIX Ở ĐÂY =======================
bool User::checklogin(const string& user, const string& plainPassword) const {
    if (user != username || password_hash.empty()) return false;

    return crypto_pwhash_str_verify(
        password_hash.c_str(),
        plainPassword.c_str(),
        plainPassword.size()
    ) == 0;
}
// ========================================================

bool User::existsMatch(const string& u, const string& e, const string& p) const {
    return (u == username || e == email || p == phone);
}

// -----------------------------------------------------------------------

void saveAllUsers(const vector<User>& users) {
    ofstream fout("users.txt", ios::trunc);
    if (!fout.is_open()) return;

    for (const auto& u : users) {
        fout << u.getUsername() << "\n";
        fout << u.getPasswordHash() << "\n";
        fout << u.getEmail() << "\n";
        fout << u.getPhone() << "\n";
        fout << u.getRole() << "\n";
        fout << fixed << setprecision(2) << u.getWallet() << "\n";
    }
}

void loadUsers(vector<User>& users) {
    ifstream fin("users.txt");
    if (!fin.is_open()) return;

    string user, pass_hash, mail, sdt, role, walletStr;
    while (true) {
        if (!getline(fin, user)) break;
        if (!getline(fin, pass_hash)) break;
        if (!getline(fin, mail)) break;
        if (!getline(fin, sdt)) break;
        if (!getline(fin, role)) break;
        if (!getline(fin, walletStr)) break;

        double w = 0.0;
        try { w = stod(walletStr); }
        catch (...) { w = 0.0; }

        users.emplace_back(user, pass_hash, mail, sdt, role, w);
    }
}

int findUserIndex(const vector<User>& users, const string& username) {
    for (size_t i = 0; i < users.size(); ++i)
        if (users[i].getUsername() == username) return (int)i;
    return -1;
}

bool isUsernameTaken(const vector<User>& users, const string& username, int selfIndex) {
    for (size_t i = 0; i < users.size(); ++i)
        if ((int)i != selfIndex && users[i].getUsername() == username)
            return true;
    return false;
}

bool isEmailOrPhoneTaken(const vector<User>& users, const string& email, const string& phone, int selfIndex) {
    for (size_t i = 0; i < users.size(); ++i) {
        if ((int)i == selfIndex) continue;
        if (!email.empty() && users[i].getEmail() == email) return true;
        if (!phone.empty() && users[i].getPhone() == phone) return true;
    }
    return false;
}
