#pragma once

#include <vector>

struct LCAEdge {
    int to;
};

class LCATesterBase {
    /// 最初に一度呼ばれる。rは木の根
    virtual void setup(std::vector<std::vector<LCAEdge>> g, int r) = 0;
    /// u, vのLCAの頂点番号を返す
    virtual int query(int u, int v) = 0;
};

#include "gtest/gtest.h"
#include "lca.h" // "正しい"LCAが入っている
#include "random.h" // 乱数生成ライブラリ

// おまじない
template<typename LCA>
class LCATest : public ::testing::Test {};
TYPED_TEST_CASE_P(LCATest);


/// 小さなケースでのランダムテスト
TYPED_TEST_P(LCATest, StressTest) {
    using G = std::vector<std::vector<LCAEdge>>;
    auto check = [&](G g) {
        int n = int(g.size());
        TypeParam your_lca;
        int r = Random::rand_int(0, n - 1);
        your_lca.setup(g, r);
        auto my_lca = YosupoLCA::get_lca(g, r);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ASSERT_EQ(your_lca.query(i, j), my_lca.query(i, j));
            }
        }
    };

    for (int ph = 0; ph < 100; ph++) {
        G g(20);
        for (int i = 1; i < 20; i++) {
            int p = Random::rand_int(0, i - 1);
            g[i].push_back(LCAEdge{p});
            g[p].push_back(LCAEdge{i});
        }
        check(g);
    }
}

/// 有向木を入れたときでも動くのが望ましい
TYPED_TEST_P(LCATest, DirectedTest) {
    using G = std::vector<std::vector<LCAEdge>>;
    /*
     * 2 - 1 - 0
     *       \ 3
     */
    G g(4);
    g[2].push_back(LCAEdge{1});
    g[1].push_back(LCAEdge{0});
    g[1].push_back(LCAEdge{3});

    TypeParam your_lca;
    your_lca.setup(g, 2);

    ASSERT_EQ(your_lca.query(2, 0), 2);
    ASSERT_EQ(your_lca.query(3, 1), 1);
    ASSERT_EQ(your_lca.query(0, 3), 1);
    ASSERT_EQ(your_lca.query(3, 3), 3);
}

// おまじない
REGISTER_TYPED_TEST_CASE_P(LCATest,
        StressTest, DirectedTest);
