#pragma once

#include <algorithm>
#include <vector>

namespace algotest {

class FenwickTesterBase {
    // 最初に1回, 初期状態の数列(0 <= a[i] <= 1e9)
    virtual void setup(std::vector<long long> a) = 0;
    // a[k] += x (0 <= x <= 1e9)
    virtual void add(int k, long long x) = 0;
    // a[l] + ... + a[r-1] (0 <= l <= r <= n)
    virtual long long sum(int l, int r) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename FENWICK>
class FenwickTest : public ::testing::Test {};

TYPED_TEST_CASE_P(FenwickTest);

struct FenwickNaive {
    template <class T>
    using V = std::vector<T>;
    using ll = long long;
    V<ll> a;

    FenwickNaive(V<ll> _a = V<ll>()) : a(_a) {}

    void add(int k, ll x) { a[k] += x; }

    ll sum(int l, int r) {
        ll ans = 0;
        for (int i = l; i < r; i++)
            ans += a[i];
        return ans;
    }
};

TYPED_TEST_P(FenwickTest, StressTest) {
    auto gen = algotest::random::Random();
    for (int tc = 0; tc < 100; tc++) {
        TypeParam your_fenwick;
        int n = gen.next(1, 100);
        std::vector<long long> a(n);
        for (int i = 0; i < n; i++) {
            a[i] = gen.next(0, 1000000000);
        }
        your_fenwick.setup(a);
        FenwickNaive fw(a);
        int q = gen.next(1, 100);
        for (int ph = 0; ph < q; ph++) {
            int ty = gen.next(0, 1);
            if (ty == 0) {
                // add
                int k = gen.next(0, n - 1);
                long long x = gen.next(0, 1000000000);
                your_fenwick.add(k, x);
                fw.add(k, x);
            } else {
                int a = gen.next(0, n - 1);
                int b = gen.next(0, n - 1);
                if (a > b)
                    std::swap(a, b);
                b++;
                EXPECT_EQ(your_fenwick.sum(a, b), fw.sum(a, b));
            }
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(FenwickTest, StressTest);

}  // namespace algotest
