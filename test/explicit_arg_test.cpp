#include "test.hpp"
#include "uxx/uxx.hpp"

struct throwing_type {
    explicit throwing_type() { }
};

using tag_type = void;

static_assert(std::is_default_constructible_v<uxx::explicit_arg<int, tag_type>>);
static_assert(std::is_default_constructible_v<uxx::explicit_arg<int*, tag_type>>);
static_assert(std::is_default_constructible_v<uxx::explicit_arg<int**, tag_type>>);
static_assert(std::is_default_constructible_v<uxx::explicit_arg<const int, tag_type>>);
static_assert(std::is_default_constructible_v<uxx::explicit_arg<const int*, tag_type>>);
static_assert(std::is_default_constructible_v<uxx::explicit_arg<const int**, tag_type>>);

static_assert(!std::is_default_constructible_v<uxx::explicit_arg<int&, tag_type>>);
static_assert(!std::is_default_constructible_v<uxx::explicit_arg<int&&, tag_type>>);
static_assert(!std::is_default_constructible_v<uxx::explicit_arg<int&, tag_type>>);
static_assert(!std::is_default_constructible_v<uxx::explicit_arg<int&&, tag_type>>);
static_assert(!std::is_default_constructible_v<uxx::explicit_arg<const int&, tag_type>>);
static_assert(!std::is_default_constructible_v<uxx::explicit_arg<const int&&, tag_type>>);

static_assert(std::is_nothrow_default_constructible_v<uxx::explicit_arg<int, tag_type>>);
static_assert(!std::is_nothrow_default_constructible_v<uxx::explicit_arg<throwing_type, tag_type>>);

TEST_CASE("Is R-value constructible and assignable", "[explicit_arg]")
{
    uxx::explicit_arg<int, tag_type> val1(123);
    uxx::explicit_arg<const int, tag_type> val2(321);

    REQUIRE(val1.get() == 123);
    REQUIRE(val2.get() == 321);
    REQUIRE(val1 == 123);
    REQUIRE(val2 == 321);

    static_assert(std::is_assignable_v<decltype(val1), int>);
    static_assert(std::is_assignable_v<decltype(val1.get()), int>);
    static_assert(!std::is_assignable_v<decltype(val2), int>);
    static_assert(!std::is_assignable_v<decltype(val2.get()), int>);

    val1.get() = 666;
    REQUIRE(val1.get() == 666);
    val1 = 999;
    REQUIRE(val1.get() == 999);
}

TEST_CASE("Is L-value constructible and assignable", "[explicit_arg]")
{
    const int num1 = 123;
    const int num2 = 321;

    uxx::explicit_arg<int, tag_type> val1(num1);
    uxx::explicit_arg<const int, tag_type> val2(num2);

    REQUIRE(val1.get() == 123);
    REQUIRE(val2.get() == 321);

    val1.get() = 666;
    REQUIRE(val1.get() == 666);
    val1 = 999;
    REQUIRE(val1.get() == 999);
}

TEST_CASE("Is equality comparable", "[explicit_arg]")
{
    REQUIRE(uxx::explicit_arg<int, tag_type> { 123 } == uxx::explicit_arg<int, tag_type> { 123 });
}

TEST_CASE("Support out variables", "[explicit_arg]")
{
    const auto out_argument = [](uxx::explicit_arg<bool, tag_type>& out) {
        out = true;
    };
    uxx::explicit_arg<bool, tag_type> it_is_true { false };
    out_argument(it_is_true);
    REQUIRE(it_is_true.get());
}
