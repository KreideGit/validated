#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include <ck/validated.hpp>

TEST_CASE("Validated class tests", "[validated]") {
    struct IsPositive {
        constexpr bool operator()(int x) const { return x > 0; }
    };

    struct LessThan {
        int limit;
        LessThan(int l) : limit(l) {}
        bool operator()(int x) const { return x < limit; }
    };

    SECTION("Default construction with stateless validator") {
        ck::validated<int, IsPositive> v;
        REQUIRE(v.try_assign(5));
        REQUIRE(v.value() == 5);
    }

    SECTION("Default construction with stateful validator") {
        ck::validated<int, LessThan> v{LessThan{10}};
        REQUIRE(v.try_assign(5));
        REQUIRE(v.value() == 5);
    }

    SECTION("Construction with value only") {
        ck::validated<int, IsPositive> v{5};
        REQUIRE(v.value() == 5);
    }

    SECTION("Construction with value and validator") {
        ck::validated<int, LessThan> v{5, LessThan{10}};
        REQUIRE(v.value() == 5);
    }

    SECTION("Assignment operator") {
        ck::validated<int, IsPositive> v;
        v = 5;
        REQUIRE(v.value() == 5);
        REQUIRE_THROWS_AS(v = -1, ck::validation_error);
    }

    SECTION("try_assign method") {
        ck::validated<int, IsPositive> v;
        REQUIRE(v.try_assign(5) == true);
        REQUIRE(v.value() == 5);
        REQUIRE(v.try_assign(-1) == false);
    }

    SECTION("assign_unsafe method") {
        ck::validated<int, IsPositive> v;
        v.assign_unsafe(-1);
        REQUIRE(v.value() == -1);
    }

    SECTION("Stateful validator changes behavior") {
        LessThan lessThanTen{10};
        ck::validated<int, LessThan> v{lessThanTen};
        REQUIRE(v.try_assign(5));
        REQUIRE_FALSE(v.try_assign(15));
    }

    SECTION("Stateless validator in validated_base") {
        ck::validated_base<IsPositive> base;
        auto validator = base.validator();
        REQUIRE(validator(5) == true);
        REQUIRE(validator(-1) == false);
    }

    SECTION("Stateful validator in validated_base") {
        LessThan lessThanTen{10};
        ck::validated_base<LessThan> base{lessThanTen};
        auto& validator = base.validator();
        REQUIRE(validator(5) == true);
        REQUIRE(validator(15) == false);
    }
}