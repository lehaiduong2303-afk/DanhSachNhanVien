#include <bits/stdc++.h>
using namespace std;

long long search_rotated(const vector<long long> &a, long long x) {
    int left = 0;
    int right = static_cast<int>(a.size()) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (a[mid] == x) return mid + 1; // 1-based index
        if (a[left] <= a[mid]) {
            if (a[left] <= x && x < a[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else {
            if (a[mid] < x && x <= a[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        long long n, x;
        cin >> n >> x;
        vector<long long> a(n);
        for (long long i = 0; i < n; ++i) cin >> a[i];
        cout << search_rotated(a, x) << '\n';
    }
    return 0;
}
