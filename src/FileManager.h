#pragma once

#include <string>
#include <fstream>
#include <filesystem>

#include "Singleton.h"
#include "Manager.h"

namespace noname
{
    class FileManager : public Manager
    {
    private:
        std::filesystem::path inputFileName;
        std::filesystem::path outputFileName;

        std::ifstream inputFile;
        std::ofstream outputFile;

    public:
        ~FileManager() override;

        void initInputFile(const std::filesystem::path &input);
        void initOutputFile(const std::filesystem::path &output);

        void startUp() noexcept override;
        void shutDown() noexcept override;

        void write(const std::string &text);
    };
}
