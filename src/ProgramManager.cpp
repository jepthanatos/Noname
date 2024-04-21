//==============================================================================
// Name        : ProgramManager.cpp
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Manager of the program.
//==============================================================================

// System includes
#include <iostream>
#include <filesystem>
#include <vector>

// Local includes
#include "ProgramManager.h"
#include "LogManager.h"
#include "FileManager.h"

//==============================================================================
/* Manager */
//==============================================================================

namespace noname
{
    /**
     * Initialize the program by parsing the program arguments.
     */
    int ProgramManager::initialization(int argc, char *argv[])
    {
        // Define the usage string printed to console.
        std::string usage {
            "Usage is: Noname_run"
        };

        // if something is not right then 
        // return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    /**
     * Startup the program services.
     */
    void ProgramManager::startUp()
    {
        Manager::setType("ProgramManager");
        LM.startUp();
        LM.writeLog(Level::Debug, "Starting services...");
        FM.startUp();
        started = LM.isStarted() and
                  FM.isStarted();
    }

    /**
     * Shut down the program services.
     */
    void ProgramManager::shutDown()
    {
        FM.shutDown();
        LM.writeLog(Level::Debug, "Closing services...");
        LM.shutDown();
    }

    /**
     * Run program loop.
     */
    void ProgramManager::run()
    {
        LM.writeLog(Level::Debug, "Starting program...");
        LM.writeLog(Level::Debug, "Ending program...");
    }
}
