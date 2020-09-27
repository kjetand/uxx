#ifndef _UXX_COMMON_HPP
#define _UXX_COMMON_HPP

#if defined(_WIN32)
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#pragma GCC diagnostic pop
#endif

#endif
