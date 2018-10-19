#pragma once

#include <algorithm>
#include <vector>

namespace algotest {

class StaticRMQTesterBase {
    // 最初に1回, 初期状態の数列(0 <= a[i] <= 1e9)
    virtual void setup(std::vector<int> a) = 0;
    // min(a[l], ...  ,a[r-1]) (0 <= l < r <= n)
    virtual int range_min(int l, int r) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename RMQ>
class StaticRMQTest : public ::testing::Test {};

TYPED_TEST_CASE_P(StaticRMQTest);

TYPED_TEST_P(StaticRMQTest, StressTest) {
    auto gen = algotest::random::Random();
    for (int tc = 0; tc < 100; tc++) {
        TypeParam your_rmq;
        int n = gen.uniform(1, 100);
        std::vector<int> a(n);
        for (int i = 0; i < n; i++) {
            a[i] = gen.uniform(0, 1000000000);
        }

        your_rmq.setup(a);
        for (int l = 0; l < n; l++) {
            int mi = a[l];
            for (int r = l+1; r <= n; r++) {
                mi = std::min(mi, a[r-1]);
                ASSERT_EQ(mi, your_rmq.range_min(l, r));
            }
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(StaticRMQTest, StressTest);

}  // namespace algotest
