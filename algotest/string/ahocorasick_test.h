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
     * 例えばtarget = "abcabd", pattern = "ba"なら、
     * {0(="ab...."), 3(="..ab..")}を返す
     */
    virtual std::vector<std::vector<int>> enumerate(
        std::string target,
        std::vector<std::string> patterns) = 0;
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

    for (int i = 0; i < 300; i++) {
        int n = gen.uniform(1, 100);
        int m = gen.uniform(1, 20);
        std::string target = gen.lower_string(n);
        std::vector<std::string> patterns(m);
        for (int j = 0; j < m; j++) {
            if (gen.uniform_bool()) {
                patterns[j] = gen.lower_string(10);
            } else {
                int di = gen.uniform(1, n);
                int st = gen.uniform(0, n - di);
                patterns[j] = target.substr(st, di);
            }
        }
        auto res = your_ahocorasick.enumerate(target, patterns);
        for (int j = 0; j < m; j++) {
            auto pattern = patterns[j];
            std::vector<int> v;
            int di = pattern.size();
            for (int s = 0; s + di <= n; s++) {
                if (target.substr(s, di) == pattern)
                    v.push_back(s);
            }
            ASSERT_EQ(v, res[j]);
        }
    }
}

TYPED_TEST_P(AhoCorasickTest, SkipFailureLinkTest) {
    TypeParam your_ahocorasick;

    auto target = std::string(1000000, 'a');
    std::vector<std::string> patterns(2);
    patterns[0] = std::string(1, 'a');
    patterns[1] = std::string(999999, 'a');
    std::vector<std::vector<int>> ans(2);
    for (int i = 0; i < 1000000; i++)
        ans[0].push_back(i);
    for (int i = 0; i < 2; i++)
        ans[1].push_back(i);

    auto out = your_ahocorasick.enumerate(target, patterns);
    ASSERT_EQ(ans, out);
}

REGISTER_TYPED_TEST_CASE_P(AhoCorasickTest, StressTest, SkipFailureLinkTest);

}  // namespace algotest
