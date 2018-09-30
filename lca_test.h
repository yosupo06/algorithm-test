#include "gtest/gtest.h"
#include "random.h"

template<typename LCA>
class LCATest : public ::testing::Test {
    LCATest() {
        struct E {
            int to;
        };
        auto check = [&](vector <vector<E>> g) {
            int n = int(g.size());
            auto lca = get_lca(g, 0);
            auto hl = get_hl(g, 0);

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    ASSERT_EQ(lca.query(i, j), hl.lca(i, j));
                }
            }
        };

        for (int ph = 0; ph < 100; ph++) {
            VV <E> g(20);
            for (int i = 1; i < 20; i++) {
                int p = rand_int(0, i - 1);
                g[i].push_back(E{p});
                g[p].push_back(E{i});
            }
            check(g);
        }
    }
}
