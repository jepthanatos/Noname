#ifndef __WORLD_MANAGER_H__
#define __WORLD_MANAGER_H__

// System includes
#include <string>
#include <vector>
#include <memory>
#include <string_view>

// Local includes
#include "Creature.h"
#include "CreaturesManager.h"
#include "Player.h"

// Two-letter acronym for easier access to manager
#define World noname::WorldManager::getInstance()

namespace noname
{
    class WorldManager : public Manager, public Singleton<WorldManager>
    {
    private:
        std::vector<std::unique_ptr<Creature>> _creatures;
        std::unique_ptr<Player> _player;

    public:
        ~WorldManager() override = default;

        void startUp() noexcept override
        {
            Manager::setType("WorldManager");
            LM.writeLog(Level::Debug, "WorldManager::startUp");
            Manager::startUp();
            addPlayer();
            addCreatures();
        }

        void shutDown() noexcept override
        {
            _creatures.clear();
            Manager::shutDown();
            LM.writeLog(Level::Debug, "WorldManager::shutDown");
        }

        void addCreatures()
        {
            for (const auto &creature : CM.getCreaturesList())
            {
                _creatures.push_back(std::make_unique<Creature>(*creature.second));
            }
        }

        void addPlayer()
        {
            _player = std::make_unique<Player>("Vagadonnaego");
        }
    };
}

#endif // __WORLD_MANAGER_H__
