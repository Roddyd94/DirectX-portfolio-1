#pragma once

#include <Windows.h>
#include <sstream>

enum class LogLevel : uint8
{
    Debug,
    Info,
    Warning,
    Error,
    Fatal,
    END
};

#define DEBUG_LEVEL   "[DEBUG]"
#define INFO_LEVEL    "[INFO]"
#define WARNING_LEVEL "[WARNING]"
#define ERROR_LEVEL   "[ERROR]"
#define FATAL_LEVEL   "[FATAL]"

class Logger
{
    friend class LogManager;

private:
    std::queue<std::string> _logs;
    LogLevel                _logLevel = LogLevel::END;

public:
    void Init(LogLevel level = LogLevel::Debug);

public:
    template <typename T, typename... Args>
    void Log(LogLevel level, const T& data, const Args&... args)
    {
        if (_logLevel > level)
            return;

        switch (level)
        {
        case LogLevel::Debug:
            Push(DEBUG_LEVEL, data, args...);
            break;
        case LogLevel::Info:
            Push(INFO_LEVEL, data, args...);
            break;
        case LogLevel::Warning:
            Push(WARNING_LEVEL, data, args...);
            break;
        case LogLevel::Error:
            Push(ERROR_LEVEL, data, args...);
            break;
        case LogLevel::Fatal:
            Push(FATAL_LEVEL, data, args...);
            break;
        case LogLevel::END:
            break;
        default:
            break;
        }
    }

private:
    template <typename T, typename... Args>
    void Push(const char* logLevelString, const T& data, const Args&... args)
    {
        SYSTEMTIME sysTime;
        GetLocalTime(&sysTime);

        char timeText[128] = {};
        sprintf_s(timeText, "[%4d-%2d-%2dT%2d:%2d:%2d] ", sysTime.wYear,
          sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,
          sysTime.wSecond);

        std::ostringstream stream;
        stream << timeText;
        stream << logLevelString << ' ';

        Write(stream, data, args...);

        _logs.push(stream.str());
    }

    template <typename T, typename... Args>
    void Write(std::ostringstream& os, const T& data, const Args&... args)
    {
        os << data;
        ((os << ", " << args), ...);
    }
};
