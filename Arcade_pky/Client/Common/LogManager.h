#pragma once
#include "Logger.h"
#include "Core/Singleton.h"

#include <fstream>
#include <mutex>
#include <thread>

class LogManager : public Singleton<LogManager>
{
    DECLARE_SINGLETON(LogManager)
    using TimePoint = std::chrono::steady_clock::time_point;

private:
    Logger        _logger;
    std::ofstream _file;
    std::mutex    _mutex;
    std::thread   _logThread;
    TimePoint     _lastDumpTime;
    bool          _isRunning   = false;
    bool          _shouldFlush = false;

public:
#ifdef _DEBUG
    void Init(LogLevel level = LogLevel::Debug);
#else
    void Init(LogLevel level = LogLevel::Error);
#endif // _DEBUG
    void Destroy();

    void FlushManually();

private:
    void ProcessLog();
    void FlushToFile();
    void FlushLogs();
    void OpenNewFile();

public:
    template <typename T, typename... Args>
    void Debug(const T& data, const Args&... args)
    {
        std::lock_guard lock(_mutex);

        _logger.Log(LogLevel::Debug, data, args...);
    }

    template <typename T, typename... Args>
    void Info(const T& data, const Args&... args)
    {
        std::lock_guard lock(_mutex);

        _logger.Log(LogLevel::Info, data, args...);
    }

    template <typename T, typename... Args>
    void Warning(const T& data, const Args&... args)
    {
        std::lock_guard lock(_mutex);

        _logger.Log(LogLevel::Warning, data, args...);
    }

    template <typename T, typename... Args>
    void Error(const T& data, const Args&... args)
    {
        std::lock_guard lock(_mutex);

        _logger.Log(LogLevel::Error, data, args...);
    }

    template <typename T, typename... Args>
    void Fatal(const T& data, const Args&... args)
    {
        std::lock_guard lock(_mutex);

        _logger.Log(LogLevel::Fatal, data, args...);
    }
};
