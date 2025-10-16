#include <bits/stdc++.h>
using namespace std;

struct NhanVien {
    string ma, ten, gt, ns, dchi, mst, nkhd;
};


string taoma(int id) {
    string s = to_string(id);
    while (s.size() < 5) s = "0" + s;
    return s;
}


int chuyenNgay(string s) {
    if (s.size() < 10) return 0; // tránh lỗi nếu input bị rỗng
    string d = s.substr(0, 2);
    string m = s.substr(3, 2);
    string y = s.substr(6, 4);
    return stoi(y + m + d);
}


void nhap(NhanVien &a) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, a.ten);
    getline(cin, a.gt);
    getline(cin, a.ns);
    getline(cin, a.dchi);
    getline(cin, a.mst);
    getline(cin, a.nkhd);
}


void sapxep(NhanVien ds[], int n) {
    sort(ds, ds + n, [](NhanVien a, NhanVien b) {
        return chuyenNgay(a.ns) < chuyenNgay(b.ns);
    });
}


void inds(NhanVien ds[], int n) {
    for (int i = 0; i < n; i++) {
        ds[i].ma = taoma(i + 1);
        cout << ds[i].ma << " " << ds[i].ten << " " << ds[i].gt << " "
             << ds[i].ns << " " << ds[i].dchi << " " << ds[i].mst << " "
             << ds[i].nkhd << "\n";
    }
}


int main(){
    struct NhanVien ds[50];
    int N,i;
    cin >> N;
    for(i = 0; i < N; i++) nhap(ds[i]);
    sapxep(ds, N);
    inds(ds, N);
    return 0;
}