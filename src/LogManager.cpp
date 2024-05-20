#include "LogManager.h"

// Sytem includes
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace noname
{
    LogManager::~LogManager()
    {
        if (logFile.is_open())
            logFile.close();
    }

    void LogManager::startUp()
    {
        try
        {
            Manager::setType("LogManager");
            logFile.open(LOGFILE_NAME, std::ofstream::out | std::ofstream::app);
            logLevel = Level::Debug;
            started = true;
            writeLog(Level::Debug, "LogManager::startUp");
        }
        catch (std::ofstream::failure e)
        {
            std::cerr << "Log file creation failed." << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }

    void LogManager::shutDown()
    {
        writeLog(Level::Debug, "LogManager::shutDown");
        logFile.close();
    }

    void LogManager::writeLog(Level level, const std::string &message)
    {
        // Lambda to generate pretty time for logs
        auto prettyTime = []()
        {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::stringstream stream;
            stream << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
            return stream.str();
        };

        if (level >= logLevel)
        {
            logFile << prettyTime() << " : " << message << std::endl;
            logFile.flush();
        }
    }

    Level LogManager::getLevel() const
    {
        return logLevel;
    }

    void LogManager::setLevel(Level value)
    {
        logLevel = value;
    }
}
