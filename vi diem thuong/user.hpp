#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User {
private:
    std::string username;
    std::string password_hash;
    std::string email;
    std::string phone;
    std::string role;
    double wallet;
public:
    User();
    User(const std::string& u, const std::string& hash, const std::string& e,
        const std::string& p, const std::string& r, double w);

    std::string getUsername() const;
    std::string getPasswordHash() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::string getRole() const;
    double getWallet() const;

    void setUsername(const std::string& u);
    void setPasswordHash(const std::string& h);
    void setEmail(const std::string& e);
    void setPhone(const std::string& p);
    void setRole(const std::string& r);
    void setWallet(double w);

    bool checklogin(const std::string& user, const std::string& plainPassword) const;
    bool existsMatch(const std::string& u, const std::string& e, const std::string& p) const;
};

int findUserIndex(const std::vector<User>& users, const std::string& username);
bool isUsernameTaken(const std::vector<User>& users, const std::string& username, int selfIndex);
bool isEmailOrPhoneTaken(const std::vector<User>& users, const std::string& email, const std::string& phone, int selfIndex);

void saveAllUsers(const std::vector<User>& users);
void loadUsers(std::vector<User>& users);

#endif
