#pragma once

#include <numeric>
#include <queue>
#include <vector>

namespace algotest {

struct MinCostFlowDoubleEdge {
    int to;
    int cap;
    double cost;
};

class MinCostFlowDoubleTesterBase {
  public:
    static constexpr double kWarnErr = 1e-6;
    static constexpr double kAssertErr = 1e-3;
    virtual double max_flow_min_cost(
        std::vector<std::vector<MinCostFlowDoubleEdge>> g,
        int s,
        int t) = 0;
};
constexpr double MinCostFlowDoubleTesterBase::kWarnErr;
constexpr double MinCostFlowDoubleTesterBase::kAssertErr;

}  // namespace algotest

#include "../comparator.h"
#include "../random.h"
#include "algotest/graph/mincostflow.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename MCF>
class MinCostFlowDoubleTest : public ::testing::Test {};

TYPED_TEST_CASE_P(MinCostFlowDoubleTest);

namespace mincostflow {

template <class T>
using V = std::vector<T>;
template <class T>
using VV = V<V<T>>;

class MCFDCorrect : MinCostFlowDoubleTesterBase {
  public:
    double max_flow_min_cost(VV<MinCostFlowDoubleEdge> _g, int s, int t) final {
        int n = int(_g.size());
        struct E {
            int to, cap;
            double dist;
            int rev;
        };
        VV<E> g(n);
        VV<E> elist(n);
        auto add_edge = [&](int from, int to, int cap, double dist) {
            elist[from].push_back(E{to, cap, dist, -1});
            g[from].push_back(E{to, cap, dist, int(g[to].size())});
            g[to].push_back(E{from, 0, -dist, int(g[from].size()) - 1});
        };
        for (int i = 0; i < n; i++) {
            for (auto e : _g[i]) {
                add_edge(i, e.to, e.cap, e.cost);
            }
        }

        auto res = get_mcf<int, double>(g, s, t, false);
        res.max_flow(1000000000);

        double sm = (res.dual[t] - res.dual[s]) * res.cap_flow;
        for (int i = 0; i < n; i++) {
            for (auto e : elist[i]) {
                if (!e.cap || res.dual[i] == decltype(res)::INF)
                    continue;
                sm -= max<double>(0, (res.dual[e.to] - res.dual[i]) - e.dist) *
                      e.cap;
            }
        }
        assert(comparator::approx_equal(res.flow, sm, kWarnErr));
        return sm;
    }
};

}  // namespace mincostflow

TYPED_TEST_P(MinCostFlowDoubleTest, StressTest) {
    algotest::random::Random gen;
    using G = std::vector<std::vector<MinCostFlowDoubleEdge>>;

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
            double cost = gen.uniform01() * 100;
            g[x].push_back(MinCostFlowDoubleEdge{y, cap, cost});
        }
        TypeParam your_mcf;
        mincostflow::MCFDCorrect ans_mcf;
        double ans = ans_mcf.max_flow_min_cost(g, s, t);
        double out = your_mcf.max_flow_min_cost(g, s, t);
        EXPECT_PRED3(comparator::approx_equal, ans, out,
                     MinCostFlowDoubleTesterBase::kWarnErr);
        ASSERT_PRED3(comparator::approx_equal, ans, out,
                     MinCostFlowDoubleTesterBase::kAssertErr);
    }
}

// おまじない
REGISTER_TYPED_TEST_CASE_P(MinCostFlowDoubleTest, StressTest);

}  // namespace algotest
