#pragma once

#include <vector>

namespace algotest {

class PrimeTesterBase {
    /// xが素因数かどうかを返す (1 <= x <= 2e18)
    virtual bool is_prime(long long x) = 0;
    /// xの素因数の一覧を返す (1 <= x <= 2e18)
    virtual std::vector<long long> factor(long long x) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename Prime>
class PrimeTest : public ::testing::Test {};

TYPED_TEST_CASE_P(PrimeTest);

template <class T>
bool eq_v(std::vector<T> v, std::vector<T> w) {
    if (v.size() != w.size())
        return false;
    sort(begin(v), end(v));
    sort(begin(w), end(w));
    int n = int(v.size());
    for (int i = 0; i < n; i++) {
        if (v[i] != w[i])
            return false;
    }
    return true;
}

TYPED_TEST_P(PrimeTest, IsPrimeTest) {
    TypeParam your_prime;
    ASSERT_FALSE(your_prime.is_prime(1));
    ASSERT_TRUE(your_prime.is_prime(2));
    ASSERT_TRUE(your_prime.is_prime(3));
    ASSERT_FALSE(your_prime.is_prime(4));
    ASSERT_TRUE(your_prime.is_prime(5));
    ASSERT_TRUE(your_prime.is_prime(103));
    ASSERT_FALSE(your_prime.is_prime(103 * 107));
    ASSERT_FALSE(your_prime.is_prime(31 * 151));
}

TYPED_TEST_P(PrimeTest, PollardTest) {
    TypeParam your_prime;
    ASSERT_TRUE(eq_v(your_prime.factor(12), {2, 2, 3}));
    ASSERT_TRUE(eq_v(your_prime.factor(103 * 107), {103LL, 107LL}));
}

TYPED_TEST_P(PrimeTest, IsPrimeBigTest) {
    TypeParam your_prime;
    ASSERT_TRUE(your_prime.is_prime(1000000000000000003LL));
    ASSERT_FALSE(your_prime.is_prime(1000000007LL * 1000000009LL));
}

TYPED_TEST_P(PrimeTest, PollardBigTest) {
    TypeParam your_prime;
    ASSERT_TRUE(eq_v(your_prime.factor(1000000007LL * 1000000009LL),
                     {1000000007LL, 1000000009LL}));
}

REGISTER_TYPED_TEST_CASE_P(PrimeTest,
                           IsPrimeTest,
                           PollardTest,
                           IsPrimeBigTest,
                           PollardBigTest);

}  // namespace algotest
