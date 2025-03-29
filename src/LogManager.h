#pragma once

#include <string>
#include <fstream>
#include <string_view>
#include <filesystem>

#include "Singleton.h"
#include "Manager.h"

#define LM noname::LogManager::getInstance()

namespace noname
{
    const std::filesystem::path LOGFILE_NAME = "noname.log";

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
        std::ofstream _logFile;
        Level _logLevel{Level::Debug};

    public:
        ~LogManager() override;

        void startUp() noexcept override;
        void shutDown() noexcept override;

        void writeLog(Level level, std::string_view message);

        [[nodiscard]] Level getLevel() const noexcept;
        void setLevel(Level value) noexcept;
    };
}
