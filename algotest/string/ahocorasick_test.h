#pragma once

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace algotest {

class AhoCorasickTesterBase {
    /**
     * 長さ|patterns|の vector<vector<int>>を返す
     * それぞれの配列には、対応するpatternがtargetの中で出現する位置(の終端idx)を列挙する
     * 例えばtarget = "abcabd", pattern = "ba"なら、{0(="ab...."), 3(="..ab..")}を返す
     */
    virtual std::vector<std::vector<int>> enumerate(std::string target, std::vector<std::string> patterns) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename SA>
class AhoCorasickTest : public ::testing::Test {};

TYPED_TEST_CASE_P(AhoCorasickTest);

TYPED_TEST_P(AhoCorasickTest, StressTest) {
    TypeParam your_ahocorasick;
    auto gen = algotest::random::Random();

    for (int i = 0; i < 100; i++) {
        int n = gen.uniform(1, 100);
        int m = gen.uniform(1, 20);
        std::string target = gen.lower_string(n);
        std::vector<std::string> patterns(m);
        for (int j = 0; j < m; j++) {
            if (gen.uniform01()) {
                patterns[j] = gen.lower_string(10);
            } else {
                int di = gen.uniform(1, n);
                int st = gen.uniform(0, n-di);
                patterns[j] = target.substr(st, di);
            }
        }
        auto res = your_ahocorasick.enumerate(target, patterns);
        for (int j = 0; j < m; j++) {
            auto pattern = patterns[j];
            std::vector<int> v;
            int di = pattern.size();
            for (int s = 0; s + di <= n; s++) {
                if (target.substr(s, di) == pattern) v.push_back(s);
            }
            ASSERT_EQ(v, res[j]);
        }
    }
}

REGISTER_TYPED_TEST_CASE_P(AhoCorasickTest, StressTest);

}  // namespace algotest
