#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "fake_sodium.hpp"
#include "user.hpp"
#include "utils.hpp"
#include "transaction.hpp"

using namespace std;

int main() {
    fake_sodium_init();


    
    srand((unsigned)time(nullptr));
    vector<User> users;

    loadUsers(users);

    cout << "\n--- HE THONG TAI KHOAN + VI DIEM ---\n";

    while (true) {
        cout << "\n1. Dang ky\n2. Dang nhap\n3. Thoat\nLua chon: ";
        int choice;
        if (!(cin >> choice)) break;
        cin.ignore();

        // ------------------------------------------------------------------
        // ĐĂNG KÝ
        // ------------------------------------------------------------------
        if (choice == 1) {
            string user, pass, mail, sdt;
            cout << "Nhap ten dang nhap: ";
            getline(cin, user);

            if (findUserIndex(users, user) != -1) {
                cout << "Ten dang nhap da ton tai!\n";
                continue;
            }

            cout << "Nhap mat khau: ";
            getline(cin, pass);

            cout << "Nhap email: ";
            getline(cin, mail);

            cout << "Nhap so dien thoai: ";
            getline(cin, sdt);

            bool exists = false;
            for (const auto& u : users)
                if (u.existsMatch(user, mail, sdt)) {
                    exists = true;
                    break;
                }

            if (exists) {
                cout << "Tai khoan da ton tai!\n";
            }
            else {
                char hashed[crypto_pwhash_STRBYTES];
                if (crypto_pwhash_str(hashed, pass.c_str(), pass.size(),
                    crypto_pwhash_OPSLIMIT_INTERACTIVE,
                    crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
                {
                    cerr << "Loi: khong du bo nho de tao hash!\n";
                }
                else {
                    string hashStr(hashed);
                    users.emplace_back(user, hashStr, mail, sdt, "USER", 0.0);
                    saveAllUsers(users);
                    writeAuditLog("Tao tai khoan USER: " + user);
                    cout << "Dang ky tai khoan thanh cong! (USER)\n";
                }
            }
        }

        // ------------------------------------------------------------------
        // ĐĂNG NHẬP
        // ------------------------------------------------------------------
        else if (choice == 2) {
            string user, pass;
            cout << "Nhap ten dang nhap: ";
            getline(cin, user);

            cout << "Nhap mat khau: ";
            getline(cin, pass);

            int idx = -1;
            for (size_t i = 0; i < users.size(); ++i) {
                if (users[i].checklogin(user, pass)) {
                    idx = (int)i;
                    break;
                }
            }
            if (idx == -1) {
                cout << "Sai ten dang nhap hoac mat khau!\n";
                continue;
            }

            bool isAdminSession = (users[idx].getRole() == "ADMIN");

            cout << "Dang nhap thanh cong! Xin chao, "
                << users[idx].getUsername()
                << " (Vai tro: " << users[idx].getRole() << ")\n";

            // Admin có thể mạo danh user khác
            if (isAdminSession) {
                cout << "\n[ADMIN] Ban co muon truy cap vao tai khoan khac (nhap ten user de mao danh, bo trong de bo qua): ";
                string target;
                getline(cin, target);

                if (!target.empty() && target != users[idx].getUsername()) {
                    int tidx = findUserIndex(users, target);
                    if (tidx >= 0) {
                        writeAuditLog("Admin '" + users[idx].getUsername() +
                            "' mao danh '" + users[tidx].getUsername() + "'");
                        idx = tidx;
                        cout << "Da chuyen sang: " << users[idx].getUsername() << "\n";
                    }
                    else cout << "Khong tim thay nguoi dung '" << target << "'.\n";
                }
            }

            // ----------------------------------------------------------------
            // MENU NGƯỜI DÙNG
            // ----------------------------------------------------------------
            while (true) {
                cout << "\n--- MENU NGUOI DUNG ---\n";
                cout << "1. Cap nhat thong tin ca nhan (OTP)\n";
                cout << "2. Thay doi mat khau\n";
                cout << "3. Xem vi diem\n";
                cout << "4. Xem lich su giao dich\n";
                cout << "5. Chuyen diem sang nguoi khac (OTP)\n";

                if (isAdminSession) {
                    cout << "6. [Admin] Xem danh sach tai khoan\n";
                    cout << "7. [Admin] Tao tai khoan ho\n";
                    cout << "8. [Admin] Cap nhat thong tin ho (OTP)\n";
                    cout << "9. Dang xuat\n";
                }
                else {
                    cout << "6. Dang xuat\n";
                }

                cout << "Lua chon: ";
                int opt;
                if (!(cin >> opt)) {
                    cin.clear();
                    string tmp; getline(cin, tmp);
                    break;
                }
                cin.ignore();

                User& curr = users[idx];

                //------------------------------------------------------------------
                // NGƯỜI DÙNG THƯỜNG
                //------------------------------------------------------------------
                if (!isAdminSession) {
                    if (opt == 1) {
                        int otp = 100000 + rand() % 900000;
                        cout << "[GIA LAP] OTP: " << otp << "\n";
                        bool ok = false;

                        for (int a = 1; a <= 3; a++) {
                            cout << "Nhap OTP (" << a << "/3): ";
                            string ent;
                            getline(cin, ent);
                            if (ent == to_string(otp)) {
                                ok = true;
                                break;
                            }
                            cout << "OTP sai.\n";
                        }

                        if (!ok) {
                            cout << "Xac thuc that bai\n";
                            writeAuditLog("User '" + curr.getUsername() + "' sai OTP.");
                        }
                        else {
                            string ne, np;
                            cout << "Email moi: "; getline(cin, ne);
                            cout << "Phone moi: "; getline(cin, np);

                            if (isEmailOrPhoneTaken(users, ne, np, idx)) {
                                cout << "Email/Phone da co nguoi dung.\n";
                            }
                            else {
                                curr.setEmail(ne);
                                curr.setPhone(np);
                                saveAllUsers(users);
                                writeAuditLog("User '" + curr.getUsername() + "' cap nhat thong tin.");
                                cout << "Cap nhat thanh cong.\n";
                            }
                        }
                    }

                    else if (opt == 2) {
                        cout << "Nhap mat khau hien tai: ";
                        string old; getline(cin, old);

                        if (!curr.checklogin(curr.getUsername(), old)) {
                            cout << "Sai mat khau.\n";
                        }
                        else {
                            cout << "Nhap mat khau moi: ";
                            string np; getline(cin, np);

                            char hashed_new[crypto_pwhash_STRBYTES];
                            if (crypto_pwhash_str(hashed_new, np.c_str(), np.size(),
                                crypto_pwhash_OPSLIMIT_INTERACTIVE,
                                crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0)
                            {
                                cerr << "Loi hash!\n";
                            }
                            else {
                                curr.setPasswordHash(string(hashed_new));
                                saveAllUsers(users);

                                writeAuditLog("User '" + curr.getUsername() + "' doi mat khau.");
                                cout << "Doi thanh cong.\n";
                            }
                        }
                    }

                    else if (opt == 3) {
                        cout << "So du: " << curr.getWallet() << "\n";
                    }

                    else if (opt == 4) {
                        auto txs = loadTransactionsForUser(curr.getUsername());
                        if (txs.empty()) cout << "Khong co giao dich.\n";
                        else {
                            for (auto& t : txs) {
                                cout << t.timestamp << " | "
                                    << t.type << " | "
                                    << t.amount << " | "
                                    << t.detail << "\n";
                            }
                        }
                    }

                    else if (opt == 5) {
                        cout << "User nhan: ";
                        string to;
                        getline(cin, to);

                        int tid = findUserIndex(users, to);
                        if (tid < 0) {
                            cout << "Khong tim thay user nhan.\n";
                            continue;
                        }

                        User& A = curr;
                        User& B = users[tid];

                        cout << "So diem chuyen: ";
                        string amtStr;
                        getline(cin, amtStr);

                        double d = 0.0;
                        try {
                            size_t pos;
                            d = stod(amtStr, &pos);
                            if (pos != amtStr.length()) {
                                cout << "So diem khong hop le.\n";
                                continue;
                            }
                        }
                        catch (...) {
                            cout << "So diem khong hop le.\n";
                            continue;
                        }

                        if (d <= 0.0) {
                            cout << "So diem phai lon hon 0.\n";
                            continue;
                        }

                        int otp = 100000 + rand() % 900000;
                        cout << "[GIA LAP] OTP: " << otp << "\n";
                        bool ok = false;
                        for (int a = 1; a <= 3; a++) {
                            cout << "Nhap OTP (" << a << "/3): ";
                            string ent;
                            getline(cin, ent);
                            if (ent == to_string(otp)) {
                                ok = true;
                                break;
                            }
                            cout << "OTP sai!\n";
                        }
                        if (!ok) {
                            cout << "Xac thuc that bai!\n";
                            writeAuditLog("User '" + A.getUsername() + "' sai OTP khi chuyen den '" + B.getUsername() + "'.");
                            continue;
                        }

                        double oldA = A.getWallet();
                        double oldB = B.getWallet();

                        try {
                            if (A.getWallet() < d) {
                                cout << "Low balance. Cannot proceed.\n";
                                continue;
                            }

                            A.setWallet(A.getWallet() - d);
                            B.setWallet(B.getWallet() + d);

                            saveAllUsers(users);

                            appendTransaction({ A.getUsername(), nowStr(), "Chuyen", -d, "To:" + B.getUsername() });
                            appendTransaction({ B.getUsername(), nowStr(), "Nhan", d, "From:" + A.getUsername() });

                            string txid = to_string(time(nullptr)) + "_" + A.getUsername() + "_" + B.getUsername();
                            writeAuditLog("Giao dich hoan tat | " + txid + " | From:" + A.getUsername() + " | To:" + B.getUsername() + " | Amount:" + to_string(d));
                            cout << "Giao dich hoan tat. Ma GD: " << txid << "\n";
                        }
                        catch (...) {
                            A.setWallet(oldA);
                            B.setWallet(oldB);

                            saveAllUsers(users);

                            string txid = to_string(time(nullptr)) + "_" + A.getUsername() + "_" + B.getUsername();
                            writeAuditLog("ROLLBACK giao dich | " + txid + " | From:" + A.getUsername() + " | To:" + B.getUsername());
                            cout << "Co loi xay ra. Giao dich da bi huy. Ma GD: " << txid << "\n";
                        }
                    }

                    else if (opt == 6) {
                        cout << "Dang xuat...\n";
                        break;
                    }

                    else cout << "Khong hop le.\n";
                }

                //------------------------------------------------------------------
                // ADMIN
                //------------------------------------------------------------------
                else {
                    if (opt == 1) {
                        int otp = 100000 + rand() % 900000;
                        cout << "[OTP] " << otp << "\n";
                        bool ok = false;

                        for (int a = 1; a <= 3; a++) {
                            string ent;
                            cout << "Nhap OTP: ";
                            getline(cin, ent);
                            if (ent == to_string(otp)) { ok = true; break; }
                        }

                        if (!ok) cout << "Sai OTP.\n";
                        else {
                            string ne, np;
                            cout << "Email moi: "; getline(cin, ne);
                            cout << "Phone moi: "; getline(cin, np);

                            if (isEmailOrPhoneTaken(users, ne, np, idx))
                                cout << "Email/Phone bi trung.\n";
                            else {
                                users[idx].setEmail(ne);
                                users[idx].setPhone(np);
                                saveAllUsers(users);
                                cout << "Cap nhat thanh cong.\n";
                            }
                        }
                    }

                    else if (opt == 6) {
                        cout << "\n--- DANH SACH USER ---\n";
                        for (auto& u : users) {
                            cout << u.getUsername() << " | " << u.getEmail()
                                << " | " << u.getPhone()
                                << " | " << u.getRole()
                                << " | " << u.getWallet() << "\n";
                        }
                    }

                    else if (opt == 9) {
                        cout << "Dang xuat admin...\n";
                        break;
                    }

                    else {
                        cout << "Chuc nang admin nay ban chua tach ra o file rieng (ban dau khong co)\n";
                    }
                }
            }
        }

        else if (choice == 3) {
            cout << "Thoat...\n"; 
            break;
        }

        else cout << "Lua chon sai!\n";
    }

    return 0;
}
