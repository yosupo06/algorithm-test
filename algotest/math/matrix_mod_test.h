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
    virtual long long det(std::vector<std::vector<long long>> mat) = 0;
    // mat * x = vecなるxを返す(MOD 1e9+7)
    virtual std::vector<long long> linear_equation(
        std::vector<std::vector<long long>> mat,
        std::vector<long long> vec) = 0;
    // inverse
    virtual std::vector<std::vector<long long>> inverse(
        std::vector<std::vector<long long>> mat) = 0;
};

template <class MATRIX>
class MatrixModRankTest : public ::testing::Test {};
template <class MATRIX>
class MatrixModDetTest : public ::testing::Test {};
template <class MATRIX>
class MatrixModLinearEquationTest : public ::testing::Test {};
template <class MATRIX>
class MatrixModInverseTest : public ::testing::Test {};

}  // namespace algotest

#include "../random.h"

namespace algotest {

TYPED_TEST_CASE_P(MatrixModRankTest);
TYPED_TEST_CASE_P(MatrixModDetTest);
TYPED_TEST_CASE_P(MatrixModLinearEquationTest);
TYPED_TEST_CASE_P(MatrixModInverseTest);

namespace matrixmod {
using ll = long long;
using Vec = std::vector<ll>;
using Mat = std::vector<Vec>;
constexpr ll kMod = MatrixModTesterBase::kMod;

template <class RNG>
Mat uniform_mat(int n, int m, int k, RNG& gen) {
    assert(k <= std::min(n, m));
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
            for (int l = 0; l < m; l++) {
                mat[i][l] += freq * mat[j][l];
                mat[i][l] %= kMod;
            }
        }
    }
    for (int tm = 0; tm < (n + m) * 10; tm++) {
        if (gen.uniform_bool()) {
            int a = gen.uniform(0, n - 1);
            int b = gen.uniform(0, n - 1);
            ll freq = gen.uniform(0LL, kMod - 1);
            if (a == b && freq == kMod - 1)
                continue;
            for (int i = 0; i < m; i++) {
                mat[a][i] += freq * mat[b][i];
                mat[a][i] %= kMod;
            }
        } else {
            int a = gen.uniform(0, m - 1);
            int b = gen.uniform(0, m - 1);
            ll freq = gen.uniform(0LL, kMod - 1);
            if (a == b && freq == kMod - 1)
                continue;
            for (int i = 0; i < n; i++) {
                mat[i][a] += freq * mat[i][b];
                mat[i][a] %= kMod;
            }
        }
    }

    return mat;
}

}  // namespace matrixmod

TYPED_TEST_P(MatrixModRankTest, RankStressTest) {
    TypeParam your_mat;
    algotest::random::Random gen;
    for (int ph = 0; ph < 200; ph++) {
        int n = gen.uniform(1, 20);
        int m = gen.uniform(1, 20);
        int k = gen.uniform(1, std::min(n, m));
        auto mat = matrixmod::uniform_mat(n, m, k, gen);
        ASSERT_EQ(your_mat.rank(mat), k);
    }
}

TYPED_TEST_P(MatrixModDetTest, DetSpecial) {
    using ll = long long;
    using Vec = std::vector<ll>;
    using Mat = std::vector<Vec>;
    constexpr ll kMod = MatrixModTesterBase::kMod;

    Mat mat(2, Vec(2));
    mat[0][1] = mat[1][0] = 1;

    TypeParam your_mat;
    ASSERT_EQ(your_mat.det(mat), kMod - 1);
}

TYPED_TEST_P(MatrixModDetTest, DetStressTest) {
    using ll = long long;
    using Vec = std::vector<ll>;
    using Mat = std::vector<Vec>;
    constexpr ll kMod = MatrixModTesterBase::kMod;

    TypeParam your_mat;
    algotest::random::Random gen;
    for (int ph = 0; ph < 200; ph++) {
        int n = gen.uniform(1, 20);
        Mat mat = Mat(n, Vec(n));
        ll base = 1;
        for (int i = 0; i < n; i++) {
            mat[i][i] = gen.uniform(1LL, kMod - 1);
            base *= mat[i][i];
            base %= kMod;
            for (int j = i + 1; j < n; j++) {
                mat[i][j] = gen.uniform(0LL, kMod - 1);
            }
        }
        for (int tm = 0; tm < 100; tm++) {
            int a = gen.uniform(0, n - 1);
            int b = gen.uniform(0, n - 1);
            ll freq = gen.uniform(0LL, kMod - 1);
            if (a == b)
                continue;
            if (gen.uniform_bool()) {
                for (int i = 0; i < n; i++) {
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

        ASSERT_EQ(your_mat.det(mat), base);
    }
}

TYPED_TEST_P(MatrixModLinearEquationTest, LinearEquationStressTest) {
    using ll = long long;
    using Vec = std::vector<ll>;
    constexpr ll kMod = MatrixModTesterBase::kMod;

    TypeParam your_mat;
    algotest::random::Random gen;
    for (int ph = 0; ph < 400; ph++) {
        int n = gen.uniform(1, 20);
        int m = gen.uniform(1, 20);
        int k = gen.uniform(1, std::min(n, m));
        auto mat = matrixmod::uniform_mat(n, m, k, gen);

        Vec ans(m);
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

TYPED_TEST_P(MatrixModInverseTest, InverseStressTest) {
    using ll = long long;
    constexpr ll kMod = MatrixModTesterBase::kMod;

    TypeParam your_mat;
    algotest::random::Random gen;
    for (int ph = 0; ph < 200; ph++) {
        int n = gen.uniform(1, 20);
        auto mat = matrixmod::uniform_mat(n, n, n, gen);

        auto out = your_mat.inverse(mat);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ll sm = 0;
                for (int k = 0; k < n; k++) {
                    sm += mat[i][k] * out[k][j];
                    sm %= kMod;
                }
                ll exp = (i == j) ? 1LL : 0LL;
                ASSERT_EQ(exp, sm);
            }
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(MatrixModRankTest, RankStressTest);
REGISTER_TYPED_TEST_CASE_P(MatrixModDetTest, DetStressTest, DetSpecial);
REGISTER_TYPED_TEST_CASE_P(MatrixModLinearEquationTest,
                           LinearEquationStressTest);
REGISTER_TYPED_TEST_CASE_P(MatrixModInverseTest, InverseStressTest);

}  // namespace algotest
