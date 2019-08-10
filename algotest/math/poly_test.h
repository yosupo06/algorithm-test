#pragma once

#include <vector>
#include "gtest/gtest.h"

namespace algotest {

class PolyTesterBase {
  public:
    static constexpr long long kMod = 998244353;
    static constexpr long long kG = 3;  // primitive root

  private:
    // a, bを多項式として見たときの和を返す(MOD 998244353)
    virtual std::vector<long long> add(std::vector<long long> a,
                                       std::vector<long long> b) = 0;
    // a, bを多項式として見たときの差を返す(MOD 998244353)
    virtual std::vector<long long> sub(std::vector<long long> a,
                                       std::vector<long long> b) = 0;
    // a, bを多項式として見たときの積を返す(MOD 998244353)
    virtual std::vector<long long> mul(std::vector<long long> a,
                                       std::vector<long long> b) = 0;
    virtual std::vector<long long> div(std::vector<long long> a,
                                       std::vector<long long> b) = 0;
    virtual std::vector<long long> inv(std::vector<long long> a,
                                       size_t b) = 0;
    virtual std::vector<long long> sqrt(std::vector<long long> a,
                                       size_t b) = 0;
};

}  // namespace algotest

#include "../random.h"

namespace algotest {

template <class POLY>
class PolyTest : public ::testing::Test {};

TYPED_TEST_CASE_P(PolyTest);

TYPED_TEST_P(PolyTest, AddStressTest) {
    using ll = long long;
    using V = std::vector<ll>;
    static const int N = 30;
    constexpr ll kMod = PolyTesterBase::kMod;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (int b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_poly;
            V a(a_sz), b(b_sz);
            for (int i = 0; i < a_sz; i++)
                a[i] = gen.uniform(1LL, kMod - 1);
            for (int i = 0; i < b_sz; i++)
                b[i] = gen.uniform(1LL, kMod - 1);
            V ans(std::max(a_sz, b_sz));
            for (int i = 0; i < std::max(a_sz, b_sz); i++) {
                ans[i] = 0;
                if (i < a_sz)
                    ans[i] += a[i];
                if (i < b_sz)
                    ans[i] += b[i];
                ans[i] %= kMod;
            }
            while (ans.back() == 0)
                ans.pop_back();
            auto out = your_poly.add(a, b);
            ASSERT_EQ(ans, out);
        }
    }
}

TYPED_TEST_P(PolyTest, SubStressTest) {
    using ll = long long;
    using V = std::vector<ll>;
    static const int N = 30;
    constexpr ll kMod = PolyTesterBase::kMod;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (int b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_poly;
            V a(a_sz), b(b_sz);
            for (int i = 0; i < a_sz; i++)
                a[i] = gen.uniform(1LL, kMod - 1);
            for (int i = 0; i < b_sz; i++)
                b[i] = gen.uniform(1LL, kMod - 1);
            V ans(std::max(a_sz, b_sz));
            for (int i = 0; i < std::max(a_sz, b_sz); i++) {
                ans[i] = 0;
                if (i < a_sz)
                    ans[i] += a[i];
                if (i < b_sz)
                    ans[i] += kMod - b[i];
                ans[i] %= kMod;
            }
            while (ans.back() == 0)
                ans.pop_back();
            auto out = your_poly.sub(a, b);
            ASSERT_EQ(ans, out);
        }
    }
}

TYPED_TEST_P(PolyTest, MulStressTest) {
    using ll = long long;
    using V = std::vector<ll>;
    static const int N = 30;
    constexpr ll kMod = PolyTesterBase::kMod;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (int b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_poly;
            V a(a_sz), b(b_sz);
            for (int i = 0; i < a_sz; i++)
                a[i] = gen.uniform(1LL, kMod - 1);
            for (int i = 0; i < b_sz; i++)
                b[i] = gen.uniform(1LL, kMod - 1);
            V ans(a_sz + b_sz - 1);
            for (int i = 0; i < a_sz; i++) {
                for (int j = 0; j < b_sz; j++) {
                    ans[i + j] += a[i] * b[j] % kMod;
                    ans[i + j] %= kMod;
                }
            }
            while (ans.back() == 0)
                ans.pop_back();
            auto out = your_poly.mul(a, b);
            ASSERT_EQ(ans, out);
        }
    }
}

TYPED_TEST_P(PolyTest, DivMulStressTest) {
    using ll = long long;
    using V = std::vector<ll>;
    static const int N = 30;
    constexpr ll kMod = PolyTesterBase::kMod;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (int b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_poly;
            V a(a_sz), b(b_sz);
            for (int i = 0; i < a_sz; i++)
                a[i] = gen.uniform(1LL, kMod - 1);
            for (int i = 0; i < b_sz; i++)
                b[i] = gen.uniform(1LL, kMod - 1);
            auto c = your_poly.div(your_poly.mul(a, b), b);
            ASSERT_EQ(a, c);
        }
    }
}

TYPED_TEST_P(PolyTest, DivMulWithZeroStressTest) {
    using ll = long long;
    using V = std::vector<ll>;
    static const int N = 30;
    constexpr ll kMod = PolyTesterBase::kMod;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (int b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_poly;
            V a(a_sz), b(b_sz);
            int a_zero = gen.uniform(0, a_sz - 1);
            int b_zero = gen.uniform(0, b_sz - 1);
            for (int i = a_zero; i < a_sz; i++)
                a[i] = gen.uniform(1LL, kMod - 1);
            for (int i = b_zero; i < b_sz; i++)
                b[i] = gen.uniform(1LL, kMod - 1);
            auto c = your_poly.div(your_poly.mul(a, b), b);
            ASSERT_EQ(a, c);
        }
    }
}

TYPED_TEST_P(PolyTest, InvStressTest) {
    using ll = long long;
    using V = std::vector<ll>;
    static const int N = 30;
    constexpr ll kMod = PolyTesterBase::kMod;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (size_t b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_poly;
            V a(a_sz);
            for (int i = 0; i < a_sz; i++)
                a[i] = gen.uniform(1LL, kMod - 1);
            auto c = your_poly.mul(a, your_poly.inv(a, b_sz));



            ASSERT_EQ(c[0], 1);
            for (size_t i = 1; i < std::min(c.size(), b_sz); i++) {
                ASSERT_EQ(c[i], 0);
            }
        }
    }
}


TYPED_TEST_P(PolyTest, SqrtStressTest) {
    using ll = long long;
    using V = std::vector<ll>;
    static const int N = 30;
    constexpr ll kMod = PolyTesterBase::kMod;
    algotest::random::Random gen;

    for (int a_sz = 1; a_sz < N; a_sz++) {
        for (size_t b_sz = 1; b_sz < N; b_sz++) {
            TypeParam your_poly;
            V a(a_sz);
            a[0] = 1;
            for (int i = 1; i < a_sz; i++)
                a[i] = gen.uniform(1LL, kMod - 1);
            auto c = your_poly.mul(your_poly.sqrt(a, b_sz), your_poly.sqrt(a, b_sz));

            for (size_t i = 0; i < std::min({a.size(), c.size(), b_sz}); i++) {
                ASSERT_EQ(c[i], a[i]);
            }
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(PolyTest, AddStressTest, SubStressTest, MulStressTest, DivMulStressTest, DivMulWithZeroStressTest, InvStressTest, SqrtStressTest);

}  // namespace algotest
