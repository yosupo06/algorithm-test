#pragma once

#include "gtest/gtest.h"
#include "lca.h" // "正しい"LCAが入っている
#include "random.h" // 乱数生成ライブラリ

// ここから
template<typename LCA>
class LCATest : public ::testing::Test {
};

using testing::Types;

TYPED_TEST_CASE_P(LCATest);
// ここまでおまじない

// テストケース1: 小さなケースでのランダムテスト
TYPED_TEST_P(LCATest, StressTest) {
    struct E {
        int to;
    };
    auto check = [&](std::vector <std::vector<E>> g) {
        int n = int(g.size());
        TypeParam your_lca;
        your_lca.setup(g, 0);
        auto my_lca = YosupoLCA::get_lca(g, 0);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ASSERT_EQ(your_lca.query(i, j), my_lca.query(i, j));
            }
        }
    };

    for (int ph = 0; ph < 100; ph++) {
        std::vector <std::vector<E>> g(20);
        for (int i = 1; i < 20; i++) {
            int p = Random::rand_int(0, i - 1);
            g[i].push_back(E{p});
            g[p].push_back(E{i});
        }
        check(g);
    }
}

// テストケース2: 有向木を入れたときでも動くのが望ましい
TYPED_TEST_P(LCATest, DirectedTest) {
    struct E {
        int to;
    };
    /*
     * 2 - 1 - 0
     *       \ 3
     */
    std::vector <std::vector<E>> g(4);
    g[2].push_back(E{1});
    g[1].push_back(E{0});
    g[1].push_back(E{3});

    TypeParam your_lca;
    your_lca.setup(g, 2);

    ASSERT_EQ(your_lca.query(2, 0), 2);
    ASSERT_EQ(your_lca.query(3, 1), 1);
    ASSERT_EQ(your_lca.query(0, 3), 1);
    ASSERT_EQ(your_lca.query(3, 3), 3);
}

// おまじない…
REGISTER_TYPED_TEST_CASE_P(LCATest,
        StressTest, DirectedTest);
