#pragma once

#include <string_view>

namespace rw::core {

enum class LogLevel {
    Info,
    Warning,
    Error
};

class Logger {
public:
    static void Write(LogLevel level, std::string_view message);
    static void Info(std::string_view message);
    static void Warning(std::string_view message);
    static void Error(std::string_view message);
};

} // namespace rw::core

