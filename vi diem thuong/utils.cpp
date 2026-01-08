#define _CRT_SECURE_NO_WARNINGS
#include "utils.hpp"
#include <fstream>
#include <ctime>
#include <cstring>

using namespace std;

string nowStr() {
    time_t now = time(nullptr);
    char* dt = ctime(&now);
    if (dt) dt[strlen(dt) - 1] = '\0';
    return string(dt);
}

void writeAuditLog(const string& message) {
    ofstream logFile("audit_log.txt", ios::app);
    if (!logFile.is_open()) return;
    logFile << "[" << nowStr() << "] " << message << "\n";
}
