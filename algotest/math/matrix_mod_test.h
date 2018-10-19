#pragma once

#include <vector>
#include "gtest/gtest.h"

namespace algotest {

class MatrixModTesterBase {
  public:
    static constexpr long long kMod = 1e9 + 7;

  private:
    // matのrankを返す(MOD 1e9+7)
    virtual int rank(std::vector<std::vector<long long>> mat) = 0;
    // mat * x = vecなるxを返す(MOD 1e9+7)
    virtual std::vector<long long> linear_equation(
        std::vector<std::vector<long long>> mat,
        std::vector<long long> vec) = 0;
};

}  // namespace algotest

#include "../random.h"

namespace algotest {

template <class MATRIX>
class MatrixModTest : public ::testing::Test {};

TYPED_TEST_CASE_P(MatrixModTest);

TYPED_TEST_P(MatrixModTest, RankStressTest) {
    using ll = long long;
    using Vec = std::vector<ll>;
    using Mat = std::vector<Vec>;
    constexpr ll kMod = MatrixModTesterBase::kMod;

    TypeParam your_mat;
    algotest::random::Random gen;
    for (int ph = 0; ph < 200; ph++) {
        int n = gen.uniform(1, 20);
        int m = gen.uniform(n, 20);
        int k = gen.uniform(1, n);
        Mat mat = Mat(n, Vec(m));
        for (int i = 0; i < k; i++) {
            mat[i][i] = 1;
            for (int j = i + 1; j < m; j++) {
                mat[i][j] = gen.uniform(0LL, kMod - 1);
            }
        }
        for (int i = k; i < n; i++) {
            for (int j = 0; j < k; j++) {
                ll freq = gen.uniform(0LL, kMod - 1);
                for (int k = 0; k < m; k++) {
                    mat[i][k] += freq * mat[j][k];
                    mat[i][k] %= kMod;
                }
            }
        }

        for (int tm = 0; tm < 100; tm++) {
            int a = gen.uniform(0, n - 1);
            int b = gen.uniform(0, n - 1);
            ll freq = gen.uniform(0LL, kMod - 1);
            if (a == b && freq == kMod - 1)
                continue;
            if (gen.uniform_bool()) {
                for (int i = 0; i < m; i++) {
                    mat[a][i] += freq * mat[b][i];
                    mat[a][i] %= kMod;
                }
            } else {
                for (int i = 0; i < n; i++) {
                    mat[i][a] += freq * mat[i][b];
                    mat[i][a] %= kMod;
                }
            }
        }

        if (gen.uniform_bool()) {
            // trans
            Mat _mat = mat;
            mat = Mat(m, Vec(n));
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    mat[j][i] = _mat[i][j];
                }
            }
        }

        ASSERT_EQ(your_mat.rank(mat), k);
    }
}

TYPED_TEST_P(MatrixModTest, LinearEquationStressTest) {
    using ll = long long;
    using Vec = std::vector<ll>;
    using Mat = std::vector<Vec>;
    constexpr ll kMod = MatrixModTesterBase::kMod;

    TypeParam your_mat;
    algotest::random::Random gen;
    for (int ph = 0; ph < 200; ph++) {
        int n = gen.uniform(1, 20);
        int m = gen.uniform(1, 20);
        Mat mat = Mat(n, Vec(m));
        Vec ans(m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                mat[i][j] = gen.uniform(0LL, kMod - 1);
            }
        }
        for (int j = 0; j < m; j++) {
            ans[j] = gen.uniform(0LL, kMod - 1);
        }
        Vec vec(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                vec[i] += mat[i][j] * ans[j];
                vec[i] %= kMod;
            }
        }
        auto out = your_mat.linear_equation(mat, vec);
        ASSERT_EQ(out.size(), m);
        Vec vec2(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                vec2[i] += mat[i][j] * out[j];
                vec2[i] %= kMod;
            }
        }
        ASSERT_EQ(vec, vec2);
    }
}

REGISTER_TYPED_TEST_CASE_P(MatrixModTest,
                           RankStressTest,
                           LinearEquationStressTest);

}  // namespace algotest
