#include "uxx/uxx.hpp"

uxx::string_ref::string_ref(const std::string& str) noexcept
    : _str(str.c_str())
{
}
