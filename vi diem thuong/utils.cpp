#include "utils.hpp"
#include <ctime>
#include <fstream>
#include <string>
std::string nowStr() {
    time_t t = time(nullptr);
    char buf[26];
    ctime_s(buf, sizeof(buf), &t);
    std::string s(buf);
    if(!s.empty() && s.back()=='\n') s.pop_back();
    return s;
}
void writeAuditLog(const std::string& m){
    std::ofstream f("audit_log.txt", std::ios::app);
    f<<"["<<nowStr()<<"] "<<m<<"\n";
}
