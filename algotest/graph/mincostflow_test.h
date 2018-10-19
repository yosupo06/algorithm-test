#pragma once

#include <numeric>
#include <queue>
#include <vector>

namespace algotest {

struct MinCostFlowEdge {
    int to;
    int cap;
    long long cost;
};

class MinCostFlowTesterBase {
    virtual long long max_flow_min_cost(
        std::vector<std::vector<MinCostFlowEdge>> g,
        int s,
        int t) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "algotest/graph/mincostflow.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename MCF>
class MinCostFlowTest : public ::testing::Test {};

TYPED_TEST_CASE_P(MinCostFlowTest);

namespace mincostflow {

using ll = long long;
template <class T>
using V = std::vector<T>;
template <class T>
using VV = V<V<T>>;

class MCFCorrect : MinCostFlowTesterBase {
  public:
    ll max_flow_min_cost(VV<MinCostFlowEdge> _g, int s, int t) final {
        int n = int(_g.size());
        struct E {
            int to, cap;
            ll dist;
            int rev;
        };
        VV<E> g(n);
        VV<E> elist(n);
        auto add_edge = [&](int from, int to, int cap, ll dist) {
            elist[from].push_back(E{to, cap, dist, -1});
            g[from].push_back(E{to, cap, dist, int(g[to].size())});
            g[to].push_back(E{from, 0, -dist, int(g[from].size()) - 1});
        };
        for (int i = 0; i < n; i++) {
            for (auto e : _g[i]) {
                add_edge(i, e.to, e.cap, e.cost);
            }
        }

        auto res = get_mcf<int, ll>(g, s, t, false);
        res.max_flow(1000000000);

        ll sm = (res.dual[t] - res.dual[s]) * res.cap_flow;
        for (int i = 0; i < n; i++) {
            for (auto e : elist[i]) {
                if (!e.cap || res.dual[i] == decltype(res)::INF)
                    continue;
                sm -= max(0LL, (res.dual[e.to] - res.dual[i]) - e.dist) * e.cap;
            }
        }
        assert(res.flow == sm);
        return sm;
    }
};

}  // namespace mincostflow

TYPED_TEST_P(MinCostFlowTest, IssueNegative) {
    using G = std::vector<std::vector<MinCostFlowEdge>>;

    int n = 3;
    int s = 1;
    int t = 0;
    G g(n);
    g[1].push_back(MinCostFlowEdge{0, 86, 58});
    g[1].push_back(MinCostFlowEdge{0, 82, 65});
    g[2].push_back(MinCostFlowEdge{1, 79, 100});
    g[2].push_back(MinCostFlowEdge{1, 90, 42});
    TypeParam your_mcf;
    mincostflow::MCFCorrect ans_mcf;
    ASSERT_EQ(ans_mcf.max_flow_min_cost(g, s, t),
              your_mcf.max_flow_min_cost(g, s, t));
}

TYPED_TEST_P(MinCostFlowTest, StressTestSmall) {
    algotest::random::Random gen;
    using G = std::vector<std::vector<MinCostFlowEdge>>;

    for (int ph = 0; ph < 10000; ph++) {
        int n = gen.uniform(2, 4);
        int m = gen.uniform(0, 7);
        int s, t;
        while (true) {
            s = gen.uniform(0, n - 1);
            t = gen.uniform(0, n - 1);
            if (s != t)
                break;
        }
        G g(n);

        for (int i = 0; i < m; i++) {
            int x, y;
            while (true) {
                x = gen.uniform(0, n - 1);
                y = gen.uniform(0, n - 1);
                if (x == y)
                    continue;
                break;
            }
            int cap = gen.uniform(0, 100);
            long long cost = gen.uniform(0, 100);
            g[x].push_back(MinCostFlowEdge{y, cap, cost});
        }
        TypeParam your_mcf;
        mincostflow::MCFCorrect ans_mcf;
        ASSERT_EQ(ans_mcf.max_flow_min_cost(g, s, t),
                  your_mcf.max_flow_min_cost(g, s, t));
    }
}

TYPED_TEST_P(MinCostFlowTest, StressTest) {
    algotest::random::Random gen;
    using G = std::vector<std::vector<MinCostFlowEdge>>;

    for (int ph = 0; ph < 300; ph++) {
        int n = gen.uniform(1, 200);
        int m = gen.uniform(0, 300);
        int s, t;
        while (true) {
            s = gen.uniform(0, n - 1);
            t = gen.uniform(0, n - 1);
            if (s != t)
                break;
        }
        G g(n);

        for (int i = 0; i < m; i++) {
            int x, y;
            while (true) {
                x = gen.uniform(0, n - 1);
                y = gen.uniform(0, n - 1);
                if (x == y)
                    continue;
                break;
            }
            int cap = gen.uniform(0, 100);
            long long cost = gen.uniform(0, 100);
            g[x].push_back(MinCostFlowEdge{y, cap, cost});
        }
        TypeParam your_mcf;
        mincostflow::MCFCorrect ans_mcf;
        ASSERT_EQ(ans_mcf.max_flow_min_cost(g, s, t),
                  your_mcf.max_flow_min_cost(g, s, t));
    }
}

// おまじない
REGISTER_TYPED_TEST_CASE_P(MinCostFlowTest,
                           IssueNegative,
                           StressTestSmall,
                           StressTest);

}  // namespace algotest
