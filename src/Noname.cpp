//==============================================================================
// Name        : Noname.cpp
// Author      : Jorge
// Copyright   : https://creativecommons.org/licenses/by/4.0/
// Description : Main program of a running journal.
//==============================================================================

// Local includes.
#include "ProgramManager.h"

//==============================================================================
/* Noname */
//==============================================================================

/**
 * Main program
 */
using namespace noname;

int main(int argc, char *argv[])
{
    // First check the arguments, if they are wrong exit the program.
    if (PM.initialization(argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;

    // Run the services of the program.
    PM.startUp();

    // Check if the program has started.
    if (PM.isStarted())
    {
        PM.run();
    }

    // Shut down the services of the program.
    PM.shutDown();

    return EXIT_SUCCESS;
}

//==============================================================================
