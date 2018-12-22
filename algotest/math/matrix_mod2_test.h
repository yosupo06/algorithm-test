#pragma once

#include <vector>
#include "gtest/gtest.h"

namespace algotest {

class MatrixMod2TesterBase {
  private:
    // matのrankを返す(MOD 2)
    virtual int rank(std::vector<std::vector<int>> mat) = 0;
    // mat * x = vecなるxを返す(MOD 2)
    virtual std::vector<int> linear_equation(std::vector<std::vector<int>> mat,
                                             std::vector<int> vec) = 0;
};

}  // namespace algotest

#include "../random.h"

namespace algotest {

template <class MATRIX>
class MatrixMod2Test : public ::testing::Test {};

TYPED_TEST_CASE_P(MatrixMod2Test);

namespace matrixmod2 {
using Vec = std::vector<int>;
using Mat = std::vector<Vec>;

template <class RNG>
Mat uniform_mat(int n, int m, int r, RNG& gen) {
    assert(r <= std::min(n, m));
    Mat mat = Mat(n, Vec(m));
    for (int i = 0; i < r; i++) {
        mat[i][i] = 1;
        for (int j = i + 1; j < m; j++) {
            mat[i][j] = gen.uniform_bool();
        }
    }
    for (int i = r; i < n; i++) {
        for (int j = 0; j < r; j++) {
            int freq = gen.uniform_bool();
            for (int k = 0; k < m; k++) {
                mat[i][k] ^= freq * mat[j][k];
            }
        }
    }

    for (int tm = 0; tm < (n + m) * 10; tm++) {
        if (gen.uniform_bool()) {
            int a = gen.uniform(0, n - 1);
            int b = gen.uniform(0, n - 1);
            if (a == b)
                continue;
            for (int i = 0; i < m; i++) {
                mat[a][i] ^= mat[b][i];
            }
        } else {
            int a = gen.uniform(0, m - 1);
            int b = gen.uniform(0, m - 1);
            if (a == b)
                continue;
            for (int i = 0; i < n; i++) {
                mat[i][a] ^= mat[i][b];
            }
        }
    }

    return mat;
}

}  // namespace matrixmod2

TYPED_TEST_P(MatrixMod2Test, RankStressTest) {
    TypeParam your_mat;
    algotest::random::Random gen;
    for (int ph = 0; ph < 200; ph++) {
        int n = gen.uniform(1, 20);
        int m = gen.uniform(1, 20);
        int k = gen.uniform(1, std::min(n, m));

        auto mat = matrixmod2::uniform_mat(n, m, k, gen);

        ASSERT_EQ(your_mat.rank(mat), k);
    }
}

TYPED_TEST_P(MatrixMod2Test, LinearEquationStressTest) {
    using Vec = std::vector<int>;

    TypeParam your_mat;
    algotest::random::Random gen;
    for (int ph = 0; ph < 200; ph++) {
        int n = gen.uniform(1, 20);
        int m = gen.uniform(1, 20);
        int k = gen.uniform(1, std::min(n, m));
        auto mat = matrixmod2::uniform_mat(n, m, k, gen);

        Vec ans(m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                mat[i][j] = gen.uniform_bool();
            }
        }
        for (int j = 0; j < m; j++) {
            ans[j] = gen.uniform_bool();
        }
        Vec vec(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                vec[i] ^= mat[i][j] * ans[j];
            }
        }
        auto out = your_mat.linear_equation(mat, vec);
        ASSERT_EQ(out.size(), m);
        Vec vec2(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                vec2[i] ^= mat[i][j] * out[j];
            }
        }
        ASSERT_EQ(vec, vec2);
    }
}

REGISTER_TYPED_TEST_CASE_P(MatrixMod2Test,
                           RankStressTest,
                           LinearEquationStressTest);

}  // namespace algotest
