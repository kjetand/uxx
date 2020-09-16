#ifndef _UXX_TEST_HPP
#define _UXX_TEST_HPP

#include <catch2/catch.hpp>

#ifndef UXX_SUPPRESS_EXTERNAL_WARNINGS
#ifdef _WIN32
#define UXX_SUPPRESS_EXTERNAL_WARNINGS(code) code
#else
#define UXX_SUPPRESS_EXTERNAL_WARNINGS(code)                \
    _Pragma("GCC diagnostic push");                         \
    _Pragma("GCC diagnostic ignored \"-Wuseless-cast\"");   \
    _Pragma("GCC diagnostic ignored \"-Wold-style-cast\""); \
    code                                                    \
        _Pragma("GCC diagnostic pop");
#endif
#endif

#ifdef _WIN32
#include <imgui.h>
#include <imgui_internal.h>
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <imgui.h>
#include <imgui_internal.h>
#pragma GCC diagnostic pop
#endif

#endif
