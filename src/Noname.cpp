// Local includes
#include "GameManager.h"

using namespace noname;

int main(int argc, char *argv[])
{
    // First check the arguments, if they are wrong exit the program.
    if (GM.initialization(argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;

    // Run the services of the program.
    GM.startUp();

    // Check if the program has started.
    if (GM.isStarted())
    {
        GM.run();
    }

    // Shut down the services of the program.
    GM.shutDown();

    return EXIT_SUCCESS;
}
