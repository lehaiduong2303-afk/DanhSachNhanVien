#include &lt;bits/stdc++.h&gt;
using namespace std;

int findPosition(vector&lt;int&gt;&amp; A, int x) {
    int n = A.size();
    int left = 0, right = n - 1;
    while (left &lt;= right) {
        int mid = left + (right - left) / 2;
        if (A[mid] == x) return mid + 1; // 1-based index
        if (A[left] &lt;= A[mid]) { // left half is sorted
            if (A[left] &lt;= x &amp;&amp; x &lt; A[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } else { // right half is sorted
            if (A[mid] &lt; x &amp;&amp; x &lt;= A[right]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    return -1; // Not found, though problem assumes present
}

int main() {
    int T;
    cin &gt;&gt; T;
    for(int t = 0; t &lt; T; t++) {
        int n, x;
        cin &gt;&gt; n &gt;&gt; x;
        vector&lt;int&gt; A(n);
        for(int i = 0; i &lt; n; i++) {
            cin &gt;&gt; A[i];
        }
        cout &lt;&lt; findPosition(A, x) &lt;&lt; endl;
    }
    return 0;
}