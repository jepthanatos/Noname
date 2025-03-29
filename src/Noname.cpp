// Local includes
#include "GameManager.h"
#include <iostream>
#include <exception>

using namespace noname;

int main(int argc, char *argv[])
{
    // First check the arguments, if they are wrong exit the program.
    if (GM.initialization(argc, argv) == EXIT_FAILURE)
    {
        std::cerr << "Error: Initialization failed. Exiting program." << std::endl;
        return EXIT_FAILURE;
    }

    // Run the services of the program.
    GM.startUp();

    try
    {
        // Check if the program has started successfully.
        if (GM.isStarted())
        {
            GM.run();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        GM.shutDown();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught. Exiting program." << std::endl;
        GM.shutDown();
        return EXIT_FAILURE;
    }

    // Shut down the services of the program.
    GM.shutDown();

    return EXIT_SUCCESS;
}