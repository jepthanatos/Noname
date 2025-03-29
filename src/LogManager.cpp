#include "LogManager.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace noname
{
    LogManager::~LogManager()
    {
        if (_logFile.is_open())
        {
            _logFile.close();
        }
    }

    void LogManager::startUp() noexcept
    {
        try
        {
            setType("LogManager");
            _logFile.open(LOGFILE_NAME.string(), std::ios::out | std::ios::app);
            if (!_logFile.is_open())
            {
                throw std::ios_base::failure("Failed to open log file: " + LOGFILE_NAME.string());
            }
            Manager::startUp();
            writeLog(Level::Info, "-- Starting the game --");
            writeLog(Level::Debug, "LogManager::startUp");
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void LogManager::shutDown() noexcept
    {
        writeLog(Level::Debug, "LogManager::shutDown");
        writeLog(Level::Info, "-- Closing the game --");
        if (_logFile.is_open())
        {
            _logFile.close();
        }
    }

    void LogManager::writeLog(Level level, std::string_view message)
    {
        if (level < _logLevel)
        {
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream stream;
        stream << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

        if (_logFile.is_open())
        {
            _logFile << stream.str() << " : " << message << '\n';
            _logFile.flush();
        }
    }

    Level LogManager::getLevel() const noexcept
    {
        return _logLevel;
    }

    void LogManager::setLevel(Level value) noexcept
    {
        _logLevel = value;
    }
}
