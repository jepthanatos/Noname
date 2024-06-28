#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"

// Two-letter acronym for easier access to manager
#define GM noname::GameManager::getInstance()

namespace noname
{
    class GameManager : public Manager, public Singleton<GameManager>
    {
    public:
        ~GameManager(){};

        // Initialize the program by parsing the program arguments.
        int initialization(int argc, char *argv[]);

        // Startup the program services.
        void startUp();

        // Shut down the program services.
        void shutDown();

        // Run program loop.
        void run();

        unsigned long long getExpForLevel(short level) const;
        unsigned long long getManaForLevel(short level) const;
    };
}
#endif // __GAME_MANAGER_H__
