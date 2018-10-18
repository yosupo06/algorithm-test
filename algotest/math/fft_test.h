#pragma once

#include <vector>
#include "gtest/gtest.h"

namespace algotest {

class FFTTesterBase {
  private:
    // a, bを多項式として見たときの積を返す(0 <= a_i, b_i <= 100)
    virtual std::vector<long long> multiply(std::vector<long long> a,
                                            std::vector<long long> b) = 0;
};

}  // namespace algotest

#include "../random.h"

namespace algotest {

template <class FFT>
class FFTTest : public ::testing::Test {};

TYPED_TEST_CASE_P(FFTTest);

TYPED_TEST_P(FFTTest, StressTest) {
    using V = std::vector<long long>;
    static const int N = 30;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (int b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_fft;
            V a(a_sz), b(b_sz);
            for (int i = 0; i < a_sz; i++)
                a[i] = gen.uniform(0, 100);
            for (int i = 0; i < b_sz; i++)
                b[i] = gen.uniform(0, 100);
            V ans(a_sz + b_sz - 1);
            for (int i = 0; i < a_sz; i++) {
                for (int j = 0; j < b_sz; j++) {
                    ans[i + j] += a[i] * b[j];
                }
            }

            auto out = your_fft.multiply(a, b);
            ASSERT_EQ(ans, out);
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(FFTTest, StressTest);

}  // namespace algotest
