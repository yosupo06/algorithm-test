#pragma once

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace algotest {

class SuffixArrayTesterBase {
    /// sのSuffixArrayを返す, sは英小文字
    virtual std::vector<int> sa(std::string s) = 0;
    virtual std::vector<int> lcp(std::string s, std::vector<int> sa) = 0;
};

}  // namespace algotest

#include "../random.h"
#include "gtest/gtest.h"

namespace algotest {

template <typename SA>
class SuffixArrayTest : public ::testing::Test {};

TYPED_TEST_CASE_P(SuffixArrayTest);

std::vector<int> naive_sa(std::string s) {
    std::vector<int> idx(s.size() + 1);
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(),
              [&](int l, int r) { return s.substr(l) < s.substr(r); });
    return idx;
}

std::vector<int> naive_lcp(std::string s, std::vector<int> sa) {
    int n = int(s.size());
    std::vector<int> lcp(n);
    for (int i = 0; i < n; i++) {
        std::string x = s.substr(sa[i]);
        std::string y = s.substr(sa[i + 1]);
        int c = 0;
        while (x[c] == y[c])
            c++;
        lcp[i] = c;
    }
    return lcp;
}

TYPED_TEST_P(SuffixArrayTest, SAStressTest) {
    TypeParam your_sa;
    auto gen = algotest::random::Random();

    for (int i = 0; i < 100; i++) {
        int n = gen.next(1, 100);
        std::string s = gen.next_lower_string(n);
        auto sa1 = your_sa.sa(s);
        auto sa2 = naive_sa(s);
        ASSERT_EQ(sa1, sa2);
    }
}

TYPED_TEST_P(SuffixArrayTest, LCPStressTest) {
    TypeParam your_sa;
    auto gen = algotest::random::Random();

    for (int i = 0; i < 100; i++) {
        int n = gen.next(1, 100);
        std::string s = gen.next_lower_string(n);
        auto sa = naive_sa(s);
        auto lcp1 = your_sa.lcp(s, sa);
        auto lcp2 = naive_lcp(s, sa);
        ASSERT_EQ(lcp1, lcp2);
    }
}

REGISTER_TYPED_TEST_CASE_P(SuffixArrayTest, SAStressTest, LCPStressTest);

}  // namespace algotest
