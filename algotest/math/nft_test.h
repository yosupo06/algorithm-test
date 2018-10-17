#pragma once

#include <vector>
#include "gtest/gtest.h"

namespace algotest {

class NFTTesterBase {
  public:
    static constexpr long long kMod = 998244353;
    static constexpr long long kG = 3;  // primitive root

  private:
    // a, bを多項式として見たときの積を返す(MOD 998244353)
    virtual std::vector<long long> multiply(std::vector<long long> a,
                                            std::vector<long long> b) = 0;
};

}  // namespace algotest

#include "../random.h"

namespace algotest {

template <class NFT>
class NFTTest : public ::testing::Test {};

TYPED_TEST_CASE_P(NFTTest);

TYPED_TEST_P(NFTTest, StressTest) {
    using ll = long long;
    using V = std::vector<long long>;
    static const int N = 30;
    constexpr ll kMod = NFTTesterBase::kMod;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (int b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_nft;
            V a(a_sz), b(b_sz);
            for (int i = 0; i < a_sz; i++)
                a[i] = gen.uniform(0LL, kMod - 1);
            for (int i = 0; i < b_sz; i++)
                b[i] = gen.uniform(0LL, kMod - 1);
            V ans(a_sz + b_sz - 1);
            for (int i = 0; i < a_sz; i++) {
                for (int j = 0; j < b_sz; j++) {
                    ans[i + j] += a[i] * b[j];
                    ans[i + j] %= kMod;
                }
            }

            auto out = your_nft.multiply(a, b);
            ASSERT_EQ(ans, out);
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(NFTTest, StressTest);

}  // namespace algotest
