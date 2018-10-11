#pragma once

#include <cassert>
#include <cstdint>
#include <random>

namespace algotest {

namespace random {

struct Random {
  private:
    std::mt19937_64 mt;

    // random choice from [0, upper]
    uint64_t next(uint64_t upper) {
        if (upper & (upper + 1)) {
            // b = 00..0011..11
            return mt() & upper;
        }
        int lg = 63 - __builtin_clzll(upper);
        uint64_t mask = (lg == 63) ? ~0ULL : (1ULL << (lg + 1)) - 1;
        while (true) {
            uint64_t r = mt() & mask;
            if (r <= upper)
                return r;
        }
    }

  public:
    Random() : mt() {}

    // random choice from [lower, upper]
    template <class T>
    T next(T lower, T upper) {
        assert(lower <= upper);
        return T(lower + next(uint64_t(upper - lower)));
    };
};

}  // namespace random

}  // namespace algotest
