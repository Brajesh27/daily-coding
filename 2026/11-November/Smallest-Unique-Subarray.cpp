/*
 * LeetCode 3934. Smallest Unique Subarray  (Hard)
 * POTD: 2026-11-25
 * Link: https://leetcode.com/problems/smallest-unique-subarray/
 * Topics: Array, Hash Table, Binary Search, Rolling Hash
 *
 * Task: return the minimum length L such that some subarray of length L
 * occurs exactly once in nums.
 *
 * Key observation (monotonic):
 *   If a piece of length L appears only once, adding one more element to it
 *   gives a piece of length L+1 that also appears only once (it contains the
 *   unique piece inside it). So lengths go: not-ok, not-ok, ..., ok, ok, ok.
 *   -> Binary search for the first "ok" length.
 *
 * Checking one length L quickly:
 *   Give every piece a "fingerprint" number using a polynomial rolling hash.
 *   With prefix hashes, the fingerprint of any piece is computed in O(1).
 *   Count fingerprints in a hash map; if any count == 1, length L works.
 *   Two different (base, mod) pairs are combined to make collisions very unlikely.
 *
 * Complexity: Time O(n log n), Space O(n)
 */

class Solution {
    typedef long long ll;
    static const ll MOD1 = 1000000007LL, MOD2 = 998244353LL;
    static const ll B1 = 100003LL, B2 = 131071LL;   // bases > max value (1e5)

    vector<ll> h1, h2, p1, p2;   // prefix hashes and powers of the bases

    // fingerprint of nums[i .. i+len-1], both hashes packed into one 64-bit key
    unsigned long long getHash(int i, int len) {
        ll a = (h1[i + len] - h1[i] * p1[len] % MOD1 + MOD1) % MOD1;
        ll b = (h2[i + len] - h2[i] * p2[len] % MOD2 + MOD2) % MOD2;
        return ((unsigned long long)a << 32) | (unsigned long long)b;
    }

    // does some piece of length len appear exactly once?
    bool check(int len, int n) {
        unordered_map<unsigned long long, int> count;
        count.reserve(2 * (n - len + 1));
        for (int i = 0; i + len <= n; i++) count[getHash(i, len)]++;
        for (auto& [key, c] : count)
            if (c == 1) return true;
        return false;
    }

public:
    int smallestUniqueSubarray(vector<int>& nums) {
        int n = nums.size();

        // build prefix hashes: h[i+1] = h[i] * B + nums[i]
        h1.assign(n + 1, 0); h2.assign(n + 1, 0);
        p1.assign(n + 1, 1); p2.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * B1 + nums[i]) % MOD1;
            h2[i + 1] = (h2[i] * B2 + nums[i]) % MOD2;
            p1[i + 1] = p1[i] * B1 % MOD1;
            p2[i + 1] = p2[i] * B2 % MOD2;
        }

        // binary search for the smallest length that works
        int lo = 1, hi = n;              // length n always works (whole array)
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (check(mid, n)) hi = mid;  // works -> answer is mid or smaller
            else lo = mid + 1;            // fails -> answer is bigger
        }
        return lo;
    }
};
