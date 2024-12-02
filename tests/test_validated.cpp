#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include <ck/validated.hpp>

template<typename T, T Min, T Max>
constexpr auto is_in_bounds() {
    return [](int value) {
        return value >= Min && value <= Max;
    };
}

constexpr int test() {
    ck::validated<int, decltype(is_in_bounds<int, 0, 100>())> v;
    v = 1;
    return 5;
}

TEST_CASE("Validators are correctly selected") {
    constexpr auto result = test();
}