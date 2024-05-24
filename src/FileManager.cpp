#include "FileManager.h"

// Local includes
#include "LogManager.h"

namespace noname
{
    FileManager::~FileManager()
    {
        shutDown();
    }

    void FileManager::initInputFile(const std::string &input)
    {
        inputFileName = input;
    }

    void FileManager::initOutputFile(const std::string &output)
    {
        outputFileName = output;
    }

    void FileManager::startUp()
    {
        // Open the input file.
        if (inputFileName != "")
        {
            try
            {
                inputFile.open(inputFileName, std::ofstream::in);
                started = true;
            }
            catch (std::ofstream::failure e)
            {
                LM.writeLog(Level::Debug, "Error opening the input file: " + inputFileName);
                LM.writeLog(Level::Debug, e.what());
                std::cerr << e.what() << std::endl;
            }
        }

        // Open the output file.
        if (outputFileName != "")
        {
            try
            {
                outputFile.open(outputFileName, std::ofstream::out);
            }
            catch (std::ofstream::failure e)
            {
                LM.writeLog(Level::Debug, "Error opening the output file: " + outputFileName);
                LM.writeLog(Level::Debug, e.what());
                std::cerr << e.what() << std::endl;
            }
        }
    }

    void FileManager::shutDown()
    {
        if (inputFile.is_open())
            inputFile.close();
        if (outputFile.is_open())
            outputFile.close();
    }

    void FileManager::write(const std::string &text)
    {
        outputFile << text << std::endl;
        outputFile.flush();
    }
}
