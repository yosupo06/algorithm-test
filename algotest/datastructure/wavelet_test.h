#pragma once

#include <algorithm>
#include <vector>

namespace algotest {

class WaveletTesterBase {
    // 最初に1回, 0 <= a_i < n, permとは限らない
    virtual void setup(std::vector<int> a) = 0;
    // a[l] ~ a[r-1]で，xより小さいものの個数を返す
    virtual int rank(int l, int r, int x) = 0;
    // a[l] ~ a[r-1]で，k番目の値を返す (0 <= k < r-l)
    virtual int select(int l, int r, int k) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename WAVELET>
class WaveletTest : public ::testing::Test {};

TYPED_TEST_CASE_P(WaveletTest);

struct WaveletNaive {
    template <class T>
    using V = std::vector<T>;
    V<int> v;

    WaveletNaive(V<int> _v = V<int>()) : v(_v) {}

    int rank(int a, int b, int u) {
        int ans = 0;
        for (int i = a; i < b; i++) {
            if (v[i] < u)
                ans++;
        }
        return ans;
    }

    int select(int a, int b, int k) {
        V<int> buf(v.begin() + a, v.begin() + b);
        std::sort(buf.begin(), buf.end());
        return buf[k];
    }
};

TYPED_TEST_P(WaveletTest, RankStressTest) {
    auto gen = algotest::random::Random();
    for (int tc = 0; tc < 100; tc++) {
        TypeParam your_wavelet;
        int n = gen.uniform(1, 100);
        std::vector<int> v(n);
        for (int i = 0; i < n; i++) {
            v[i] = gen.uniform(0, n - 1);
        }
        your_wavelet.setup(v);
        WaveletNaive wt(v);
        int q = gen.uniform(1, 100);
        for (int ph = 0; ph < q; ph++) {
            int a = gen.uniform(0, n - 1);
            int b = gen.uniform(0, n - 1);
            if (a > b)
                std::swap(a, b);
            b++;
            int u = gen.uniform(0, 100);
            EXPECT_EQ(wt.rank(a, b, u), wt.rank(a, b, u));
        }
    }
}

TYPED_TEST_P(WaveletTest, SelectStressTest) {
    auto gen = algotest::random::Random();
    for (int tc = 0; tc < 100; tc++) {
        TypeParam your_wavelet;
        int n = gen.uniform(1, 100);
        std::vector<int> v(n);
        for (int i = 0; i < n; i++) {
            v[i] = gen.uniform(0, n - 1);
        }
        your_wavelet.setup(v);
        WaveletNaive wt(v);
        int q = gen.uniform(1, 100);
        for (int ph = 0; ph < q; ph++) {
            int a = gen.uniform(0, n - 1);
            int b = gen.uniform(0, n - 1);
            if (a > b)
                std::swap(a, b);
            b++;
            int k = gen.uniform(0, b - a - 1);
            EXPECT_EQ(wt.select(a, b, k), wt.select(a, b, k));
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(WaveletTest, RankStressTest, SelectStressTest);

}  // namespace algotest
