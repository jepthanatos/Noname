#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

// System includes
#include <fstream>
#include <iostream>
#include <string>

// Local includes
#include "Singleton.h"
#include "Manager.h"

// Two-letter acronym for easier access to manager.
#define FM noname::FileManager::getInstance()

namespace noname
{
    class FileManager : public Manager, public Singleton<FileManager>
    {
    private:
        // Name of the IO files.
        std::string inputFileName;
        std::string outputFileName;

        // IO Files.
        std::ifstream inputFile;
        std::ofstream outputFile;

    public:
        // If files are open, close them.
        ~FileManager();

        // Initialize the input file before the start up.
        void initInputFile(const std::string &input);

        // Initialize the output file before the start up.
        void initOutputFile(const std::string &output);

        // Startup the FileManager (open files).
        void startUp();

        // Shut down the FileManager (close files).
        void shutDown();
    };
}
#endif // __FILE_MANAGER_H__
