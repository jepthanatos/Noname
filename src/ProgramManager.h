//==============================================================================
// Name        : ProgramManager.h
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Manager of the program.
//==============================================================================

#ifndef __PROGRAM_MANAGER_H__
#define __PROGRAM_MANAGER_H__

// System includes.
#include <string>

// Local includes.
#include "Singleton.h"
#include "Manager.h"

//==============================================================================
/* ProgramManager */
//==============================================================================

// Two-letter acronym for easier access to manager.
#define PM noname::ProgramManager::getInstance()

/**
 * Manager of the program.
 */
namespace noname
{
    class ProgramManager : public Manager, public Singleton<ProgramManager>
    {
    public:
        ~ProgramManager(){};

        // Initialize the program by parsing the program arguments.
        int initialization(int argc, char *argv[]);

        // Startup the program services.
        void startUp();

        // Shut down the program services.
        void shutDown();

        // Run program loop.
        void run();
    };
}
//------------------------------------------------------------------------------
#endif // __PROGRAM_MANAGER_H__
