#pragma once

#include <algorithm>
#include <cmath>

namespace algotest {

namespace comparator {

bool approx_equal(double x, double y, double max_rel) {
    if (std::isnan(x) || std::isnan(y)) return false;
    if (x == y) return true;
    return ((x - y) / std::max(std::abs(x), std::abs(y)) <= max_rel);
}

}  // namespace random

}  // namespace algotest
