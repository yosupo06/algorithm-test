#pragma once

#include <vector>

namespace algotest {

struct DijkstraEdge {
    int to;
    long long dist;
};

class DijkstraTesterBase {
    /// s, t間の最短距離を返す, 到達不可能ならばnumeric_limit<long long>::max()
    virtual long long min_dist(std::vector<std::vector<DijkstraEdge>> g,
                               int s,
                               int t) = 0;
};

}  // namespace algotest

#include <numeric>
#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename DIJKSTRA>
class DijkstraTest : public ::testing::Test {};

TYPED_TEST_CASE_P(DijkstraTest);

/// 小さなケースでのランダムテスト
TYPED_TEST_P(DijkstraTest, StressTest) {
    using G = std::vector<std::vector<DijkstraEdge>>;
    const long long INF = std::numeric_limits<long long>::max();
    auto gen = algotest::random::Random();

    for (int ph = 0; ph < 100; ph++) {
        int n = gen.uniform(2, 20);
        int m = gen.uniform(1, 100);
        G g(n);
        std::vector<std::vector<long long>> dist(
            n, std::vector<long long>(n, INF));
        for (int i = 0; i < n; i++)
            dist[i][i] = 0;
        for (int i = 0; i < m; i++) {
            int a = gen.uniform(0, n - 1);
            int b = gen.uniform(0, n - 1);
            long long c = gen.uniform(0, 1000);
            g[a].push_back(DijkstraEdge{b, c});
            dist[a][b] = std::min(dist[a][b], c);
        }
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist[i][k] == INF || dist[k][j] == INF)
                        continue;
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        int s, t;
        while (true) {
            s = gen.uniform(0, n - 1);
            t = gen.uniform(0, n - 1);
            if (s == t)
                continue;
            break;
        }

        TypeParam your_dijkstra;
        ASSERT_EQ(dist[s][t], your_dijkstra.min_dist(g, s, t));
    }
}

// おまじない
REGISTER_TYPED_TEST_CASE_P(DijkstraTest, StressTest);

}  // namespace algotest
