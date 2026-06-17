#include "Engine/Core/Logger.h"

#include <chrono>
#include <iostream>

namespace rw::core {

namespace {

const char* ToLabel(LogLevel level)
{
    switch (level) {
    case LogLevel::Info:
        return "info";
    case LogLevel::Warning:
        return "warning";
    case LogLevel::Error:
        return "error";
    }

    return "unknown";
}

} // namespace

void Logger::Write(LogLevel level, std::string_view message)
{
    const auto now = std::chrono::system_clock::now();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
        now.time_since_epoch()).count();

    std::ostream& stream = level == LogLevel::Error ? std::cerr : std::cout;
    stream << "[rw][" << ToLabel(level) << "][" << seconds << "] "
           << message << '\n';
}

void Logger::Info(std::string_view message)
{
    Write(LogLevel::Info, message);
}

void Logger::Warning(std::string_view message)
{
    Write(LogLevel::Warning, message);
}

void Logger::Error(std::string_view message)
{
    Write(LogLevel::Error, message);
}

} // namespace rw::core

