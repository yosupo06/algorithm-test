#pragma once

#include <random>

// TODO: uniform_int_distributionを自前実装する(環境依存性を消すため)
struct Random {
    std::mt19937 mt;
    Random() {
        mt = std::mt19937();
    }
    template<class T> T next(T a, T b) { return T(); }
    template<> int next<int>(int a, int b) {
        return int(next<long long>(a, b));
    }
    template<> long long next<long long>(long long a, long long b) {
        return std::uniform_int_distribution<long long>(a, b)(mt);
    }
};
