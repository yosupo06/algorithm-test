#pragma once

#include <vector>

namespace algotest {

class GCDTesterBase {
    /// gcd(x, y)を返す (|x|, |y| <= 1e18)
    virtual long long gcd(long long x, long long y) = 0;
    /// p.first * x + p.second * y = gcd(x, y)なるpairを返す (|x|, |y| <= 1e18)
    virtual std::pair<long long, long long> ext_gcd(long long x,
                                                    long long y) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename GCD>
class GCDTest : public ::testing::Test {};

TYPED_TEST_CASE_P(GCDTest);

TYPED_TEST_P(GCDTest, GCDStressTest) {
    TypeParam your_gcd;
    auto my_gcd = [&](long long x, long long y) {
        x = std::abs(x);
        y = std::abs(y);
        while (y) {
            auto z = x % y;
            x = y;
            y = z;
        }
        return x;
    };
    for (int i = -100; i < 100; i++) {
        for (int j = -100; j < 100; j++) {
            ASSERT_EQ(your_gcd.gcd(i, j), my_gcd(i, j));
        }
    }
}

TYPED_TEST_P(GCDTest, EXTGCDStressTest) {
    TypeParam your_gcd;
    auto my_gcd = [&](long long x, long long y) {
        x = std::abs(x);
        y = std::abs(y);
        while (y) {
            auto z = x % y;
            x = y;
            y = z;
        }
        return x;
    };
    for (int i = -100; i < 100; i++) {
        for (int j = -100; j < 100; j++) {
            auto p = your_gcd.ext_gcd(i, j);
            ASSERT_EQ(p.first * i + p.second * j, my_gcd(i, j));
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(GCDTest, GCDStressTest, EXTGCDStressTest);

}  // namespace algotest
