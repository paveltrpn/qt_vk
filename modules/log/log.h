
#pragma once

#include <iostream>
#include <string>
#include <format>
#include <source_location>
#include <exception>

#include <android/log.h>

namespace tire::log {
    template<bool enable = true, typename... Ts>
    void info(std::format_string<Ts...> msg, Ts &&...args) {
        if constexpr (enable) {
            const std::string out = std::vformat(msg.get(), std::make_format_args(args...));
            __android_log_write(ANDROID_LOG_INFO, "", out.c_str());
        }
    }

    template<bool enable = true, typename... Ts>
    void debug(std::format_string<Ts...> msg, Ts &&...args) {
        if constexpr (enable) {
            const std::string out = std::vformat(msg.get(), std::make_format_args(args...));
            __android_log_write(ANDROID_LOG_DEBUG, "", out.c_str());
        }
    }

    template<bool enable = true, typename... Ts>
    void warning(std::format_string<Ts...> msg, Ts &&...args) {
        if constexpr (enable) {
            const std::string out = std::vformat(msg.get(), std::make_format_args(args...));
            __android_log_write(ANDROID_LOG_WARN, "", out.c_str());
        }
    }

    template<bool enable = true, typename... Ts>
    void error(std::format_string<Ts...> msg, Ts &&...args) {
        if constexpr (enable) {
            const std::string out = std::vformat(msg.get(), std::make_format_args(args...));
            __android_log_write(ANDROID_LOG_ERROR, "", out.c_str());
        }
    }

    template<typename... Ts>
    void fatal(std::format_string<Ts...> msg, Ts &&...args) {
        constexpr char preamble[] = "\033[3;31m[fatal] \033[0m\t";
        std::cout << preamble
                  << std::vformat(msg.get(), std::make_format_args(args...))
                  << "\n";

        // Terminate
        std::terminate();
    }

    void print_source(
            const std::source_location location = std::source_location::current());
}  // namespace tire::log
