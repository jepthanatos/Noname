//==============================================================================
// Name        : FileManager.cpp
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Manager of the input and output files.
//==============================================================================

#include "FileManager.h"

// Local includes
#include "LogManager.h"

//==============================================================================
/* Manager */
//==============================================================================

namespace noname
{
    /**
     * Destructor
     */
    FileManager::~FileManager()
    {
        shutDown();
    }

    /**
     * Initialize the input file before the start up.
     */
    void FileManager::initInputFile(const std::string &input)
    {
        inputFileName = input;
    }

    /**
     * Initialize the output file before the start up.
     */
    void FileManager::initOutputFile(const std::string &output)
    {
        outputFileName = output;
    }

    /**
     * Startup the FileManager (open files).
     */
    void FileManager::startUp()
    {
        // Open the input file.
        try
        {
            inputFile.open(inputFileName);
            started = true;
        }
        catch (std::ofstream::failure e)
        {
            LM.writeLog(Level::Debug, "Error opening the input file");
            LM.writeLog(Level::Debug, e.what());
            std::cerr << e.what() << std::endl;
        }

        // Open the output file.
        if (outputFileName != "")
        {
            try
            {
                outputFile.open(outputFileName);
            }
            catch (std::ofstream::failure e)
            {
                LM.writeLog(Level::Debug, "Error opening the output file");
                LM.writeLog(Level::Debug, e.what());
                std::cerr << e.what() << std::endl;
            }
        }
    }

    /**
     * Shut down the FileManager (close files).
     */
    void FileManager::shutDown()
    {
        if (inputFile.is_open())
            inputFile.close();
        if (outputFile.is_open())
            outputFile.close();
    }
}
