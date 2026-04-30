#include "pch.h"

#include "LogManager.h"

#include "Core/DirectoryManager.h"

#define MAX_LINES 1'000

void LogManager::Init(LogLevel level)
{
#ifdef _DEBUG
    _logger.Init(LogLevel::Debug);
#else
    _logger.Init(LogLevel::Error);
#endif // _DEBUG
    _logThread = std::thread(&LogManager::ProcessLog, this);
    _isRunning = true;
}

void LogManager::Destroy()
{
    _isRunning = false;

    if (_logThread.joinable())
        _logThread.join();

    FlushLogs();
}

void LogManager::FlushManually()
{
    std::lock_guard lock(_mutex);
    _shouldFlush = true;
}

void LogManager::ProcessLog()
{
    while (_isRunning)
    {
#ifdef _DEBUG
        FlushLogs();
#else
        if (_shouldFlush)
        {
            FlushToFile();
            _lastDumpTime = std::chrono::steady_clock::now();
            _shouldFlush  = false;
            continue;
        }

        auto now = std::chrono::steady_clock::now();
        if (_logger._logs.size() > MAX_LINES
            || now - _lastDumpTime >= std::chrono::minutes(5))
        {
            FlushToFile();
            _lastDumpTime = std::chrono::steady_clock::now();
        }
#endif
    }
}

void LogManager::FlushToFile()
{
    OpenNewFile();
    FlushLogs();
    _file.close();
}

void LogManager::FlushLogs()
{
    std::lock_guard lock(_mutex);

    while (!_logger._logs.empty())
    {
        std::string msg = _logger._logs.front();
        _logger._logs.pop();

        if (!_file.is_open())
            continue;

        msg += "\n";

#ifdef _DEBUG
        OutputDebugStringA(msg.c_str());
#else
        _file << msg;
#endif
    }
}

void LogManager::OpenNewFile()
{
    auto logPath = DirectoryManager::Instance().GetCachePath("Logs");
    if (!logPath)
        return;

    SYSTEMTIME sysTime;
    GetLocalTime(&sysTime);

    char fileName[128] = {};
    sprintf_s(fileName, "%4d-%2d-%2dT%2d_%2d_%2d_%3d.txt", sysTime.wYear,
      sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,
      sysTime.wSecond, sysTime.wMilliseconds);

    std::filesystem::path filePath = *logPath / fileName;

    if (_file.is_open())
        _file.close();

    _file.open(filePath, std::ios::out | std::ios::app);
}
