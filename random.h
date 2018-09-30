#pragma once

#include <random>

namespace Random {
    using ll = long long;
    using namespace std;
    ll rand_int(ll l, ll r) { //[l, r]
        using D = uniform_int_distribution<ll>;
        static random_device rd;
        static mt19937 gen(rd());
        return D(l, r)(gen);
    }
}
