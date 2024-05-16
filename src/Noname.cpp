// Local includes
#include "ProgramManager.h"

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
