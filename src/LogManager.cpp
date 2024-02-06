//==============================================================================
// Name        : LogManager.cpp
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Manager of the logfile.
//==============================================================================

#include "LogManager.h"

// Sytem includes
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

//==============================================================================
/* Manager */
//==============================================================================

namespace noname
{
    /**
     * Destructor.
     */
    LogManager::~LogManager()
    {
        if (logFile.is_open())
            logFile.close();
    }

    /**
     * Startup the LogManager (open logfile).
     */
    void LogManager::startUp()
    {
        try
        {
            logFile.open(LOGFILE_NAME, std::ofstream::out);
            logLevel = Level::Debug;
            started = true;
        }
        catch (std::ofstream::failure e)
        {
            std::cerr << "Log file creation failed." << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }

    /**
     * Shut down the LogManager (close logfile).
     */
    void LogManager::shutDown()
    {
        logFile.close();
    }

    /**
     * Write to logfile.
     */
    void LogManager::writeLog(Level level, const std::string &message)
    {
        // Lambda to generate pretty time for logs.
        auto prettyTime = []()
        {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::stringstream stream;
            stream << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
            return stream.str();
        };

        if (level >= logLevel) {
            logFile << prettyTime() << " : " << message << std::endl;
            logFile.flush();
        }
    }

    /**
     * Get the log level.
     */
    Level LogManager::getLevel() const
    {
        return logLevel;
    }

    /**
     * Set the log level.
     */
    void LogManager::setLevel(Level value)
    {
        logLevel = value;
    }
}
