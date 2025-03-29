// System includes
#include <iostream>
#include <filesystem>
#include <vector>

// Local includes
#include "GameManager.h"
#include "LogManager.h"
#include "WeaponsManager.h"
#include "RankingManager.h"
#include "CreaturesManager.h"
#include "SkillsManager.h"
#include "WorldManager.h"
#include "Utils.h"

namespace noname
{
    int GameManager::initialization(int argc, char *argv[])
    {
        // Define the usage string printed to console.
        const std::string usage{"Usage: Noname_run"};

        if (argc < 1)
        {
            LM.writeLog(Level::Error, "Invalid arguments. " + usage);
            return EXIT_FAILURE;
        }

        LM.writeLog(Level::Info, "Initialization successful.");
        return EXIT_SUCCESS;
    }

    void GameManager::startUp() noexcept
    {
        Manager::setType("GameManager");
        LM.startUp();
        LM.writeLog(Level::Debug, "Starting services...");
        WM.startUp();
        CM.startUp();
        RM.startUp();
        SM.startUp();
        World.startUp();
        _started = LM.isStarted() and CM.isStarted() and WM.isStarted() and RM.isStarted() and SM.isStarted() and World.isStarted();
    }

    void GameManager::shutDown() noexcept
    {
        LM.writeLog(Level::Debug, "Closing services...");
        WM.shutDown();
        CM.shutDown();
        RM.shutDown();
        SM.shutDown();
        LM.shutDown();
        World.shutDown();
    }

    void GameManager::run()
    {
        LM.writeLog(Level::Debug, "Starting game...");
        // The world should be created
        // At least 1 player should be added
        // And then multiple creatures
        LM.writeLog(Level::Debug, "Ending game...");
    }

    unsigned long long GameManager::getExpForLevel(short level) const
    {
        return ((50ULL * level * level * level) - (150ULL * level * level) + (400ULL * level)) / 3ULL;
    }

    unsigned long long GameManager::getManaForLevel(short level) const
    {
        return 800ULL * level * level + 800ULL * level + 0ULL;
    }
}
