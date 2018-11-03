#pragma once

#include <vector>

namespace algotest {

struct SCCEdge {
    int to;
};

class SCCTesterBase {
    /// トポロジカルオーダーを返す
    virtual std::vector<int> topological_order(
        std::vector<std::vector<SCCEdge>> g) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename UF>
class SCCTest : public ::testing::Test {};

TYPED_TEST_CASE_P(SCCTest);

TYPED_TEST_P(SCCTest, Usage) {
    using G = std::vector<std::vector<SCCEdge>>;

    G g(5);
    g[0].push_back(SCCEdge{1});
    g[1].push_back(SCCEdge{2});
    g[2].push_back(SCCEdge{0});
    g[3].push_back(SCCEdge{1});
    g[0].push_back(SCCEdge{4});

    TypeParam your_scc;
    auto order = your_scc.topological_order(g);

    ASSERT_EQ(order, std::vector<int>({1, 1, 1, 0, 2}));
}

/// 小さなケースでのランダムテスト
TYPED_TEST_P(SCCTest, StressTest) {
    using M = std::vector<std::vector<bool>>;
    using G = std::vector<std::vector<SCCEdge>>;
    auto gen = algotest::random::Random();

    for (int ph = 0; ph < 100; ph++) {
        int n = gen.uniform(1, 50);
        int p = gen.uniform(1, 50);
        G g(n);
        M naive(n, std::vector<bool>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j)
                    continue;
                if (gen.uniform(0, 99) < p) {
                    g[i].push_back(SCCEdge{j});
                    naive[i][j] = true;
                }
            }
        }
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (naive[i][k] && naive[k][j])
                        naive[i][j] = true;
                }
            }
        }
        TypeParam your_scc;
        auto order = your_scc.topological_order(g);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (naive[i][j])
                    ASSERT_TRUE(order[i] <= order[j]);
            }
        }
    }
}

// おまじない
REGISTER_TYPED_TEST_CASE_P(SCCTest, Usage, StressTest);

}  // namespace algotest
