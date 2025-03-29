#include "FileManager.h"
#include "LogManager.h"

namespace noname
{
    FileManager::~FileManager()
    {
        shutDown();
    }

    void FileManager::initInputFile(const std::filesystem::path &input)
    {
        inputFileName = input;
    }

    void FileManager::initOutputFile(const std::filesystem::path &output)
    {
        outputFileName = output;
    }

    void FileManager::startUp() noexcept
    {
        try
        {
            if (!inputFileName.empty())
            {
                inputFile.open(inputFileName, std::ios::in);
                if (!inputFile.is_open())
                {
                    throw std::ios_base::failure("Failed to open input file: " + inputFileName.string());
                }
            }

            if (!outputFileName.empty())
            {
                outputFile.open(outputFileName, std::ios::out);
                if (!outputFile.is_open())
                {
                    throw std::ios_base::failure("Failed to open output file: " + outputFileName.string());
                }
            }

            _started = true;
        }
        catch (const std::exception &e)
        {
            LM.writeLog(Level::Error, e.what());
        }
    }

    void FileManager::shutDown() noexcept
    {
        if (inputFile.is_open())
        {
            inputFile.close();
        }
        if (outputFile.is_open())
        {
            outputFile.close();
        }
    }

    void FileManager::write(const std::string &text)
    {
        if (outputFile.is_open())
        {
            outputFile << text << '\n';
            outputFile.flush();
        }
    }
}
