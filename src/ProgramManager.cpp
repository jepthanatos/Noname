// System includes
#include <iostream>
#include <filesystem>
#include <vector>

// Local includes
#include "ProgramManager.h"
#include "LogManager.h"
#include "FileManager.h"

namespace noname
{
    int ProgramManager::initialization(int argc, char *argv[])
    {
        // Define the usage string printed to console.
        std::string usage{
            "Usage is: Noname_run"};

        // if something is not right then
        // return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    void ProgramManager::startUp()
    {
        Manager::setType("ProgramManager");
        LM.startUp();
        LM.writeLog(Level::Debug, "Starting services...");
        FM.startUp();
        started = LM.isStarted() and
                  FM.isStarted();
    }

    void ProgramManager::shutDown()
    {
        FM.shutDown();
        LM.writeLog(Level::Debug, "Closing services...");
        LM.shutDown();
    }

    void ProgramManager::run()
    {
        LM.writeLog(Level::Debug, "Starting program...");
        LM.writeLog(Level::Debug, "Ending program...");
    }
}
