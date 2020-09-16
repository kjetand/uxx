#include "test.hpp"
#include "uxx/uxx.hpp"

#include <string_view>
#include <type_traits>

using namespace std::string_view_literals;

static_assert(sizeof(uxx::string_ref) == sizeof(const char*));

static_assert(!std::is_default_constructible_v<uxx::string_ref>);
static_assert(!std::is_trivially_constructible_v<uxx::string_ref>);
static_assert(std::is_trivially_copy_constructible_v<uxx::string_ref>);
static_assert(std::is_trivially_move_constructible_v<uxx::string_ref>);
static_assert(std::is_trivially_copyable_v<uxx::string_ref>);
static_assert(std::is_trivially_copy_assignable_v<uxx::string_ref>);
static_assert(std::is_trivially_move_assignable_v<uxx::string_ref>);

static_assert(uxx::string_ref("abc").c_str() == "abc"sv);

TEST_CASE("Support const char*", "[string_ref]")
{
    const char* in = "hello";
    uxx::string_ref s { in };
    REQUIRE("hello"sv == s.c_str());
    const char* out = s;
    REQUIRE("hello"sv == out);
}

TEST_CASE("Support implicit conversion to const char*", "[string_ref]")
{
    uxx::string_ref s { "hello" };
    const char* out = s;
    REQUIRE("hello"sv == out);
}

TEST_CASE("Support std::string", "[string_ref]")
{
    const std::string in = "world";
    uxx::string_ref s { in };
    REQUIRE("world"sv == s.c_str());
}
