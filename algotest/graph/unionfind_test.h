#pragma once

#include <vector>

namespace algotest {

class UnionFindTesterBase {
    /// 最初に一度呼ばれる。nは頂点数
    virtual void setup(int n) = 0;
    /// u, vを連結する,呼ばれた時点でu, vが非連結とは限らない
    virtual void add(int u, int v) = 0;
    /// u, vが連結か返す, (0 <= u, v < g.size(), u != v)
    virtual bool is_connect(int u, int v) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename UF>
class UnionFindTest : public ::testing::Test {};

namespace unionfind {

struct UnionFind {
    std::vector<int> p, r;
    UnionFind(int N) : p(N, -1), r(N, 1) {}
    void merge(int a, int b) {
        int x = group(a), y = group(b);
        if (x == y) return; //same
        if (r[x] < r[y]) p[x] = y;
        else if (r[x] > r[y]) p[y] = x;
        else {p[x] = y; r[x]++;}
    }
    int group(int a) {
        if (p[a] == -1) return a;
        return p[a] = group(p[a]);
    }
    bool same(int a, int b) {
        return group(a) == group(b);
    }
};

}

TYPED_TEST_CASE_P(UnionFindTest);

/// 小さなケースでのランダムテスト
TYPED_TEST_P(UnionFindTest, StressTest) {
    auto gen = algotest::random::Random();
    for (int ph = 0; ph < 100; ph++) {
        int n = gen.uniform(1, 20);
        int q = gen.uniform(1, 100);

        unionfind::UnionFind uf(n);
        TypeParam your_uf;
        your_uf.setup(n);
        for (int i = 0; i < q; i++) {
            if (gen.uniform01()) {
                //add
                int a = gen.uniform(0, n-1);
                int b = gen.uniform(0, n-1);
                uf.merge(a, b);
                your_uf.add(a, b);
            } else {
                //is_connect
                int a = gen.uniform(0, n-1);
                int b = gen.uniform(0, n-1);
                ASSERT_EQ(uf.same(a, b), your_uf.is_connect(a, b));
            }
        }
    }
}

// おまじない
REGISTER_TYPED_TEST_CASE_P(UnionFindTest, StressTest);

}  // namespace algotest
