#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

// System includes
#include <string>
#include <fstream>

// Engine includes
#include "Singleton.h"
#include "Manager.h"

// Two-letter acronym for easier access to manager
#define LM noname::LogManager::getInstance()

namespace noname
{
    const std::string LOGFILE_NAME = "noname.log";

    enum class Level
    {
        Debug,
        Info,
        Warning,
        Error
    };

    class LogManager : public Manager, public Singleton<LogManager>
    {
    private:
        std::ofstream logFile;
        Level logLevel;

    public:
        // If logfile is open, close it.
        ~LogManager();

        // Startup the LogManager (open logfile).
        void startUp();

        // Shut down the LogManager (close logfile).
        void shutDown();

        // Write to logfile.
        void writeLog(Level level, const std::string &message);

        // Get the log level.
        Level getLevel() const;

        // Set the log level.
        void setLevel(Level value);
    };
}
#endif // __LOG_MANAGER_H__
