#ifndef __CREATURES_MANAGER_H__
#define __CREATURES_MANAGER_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"
#include "LogManager.h"
#include "Creature.h"
#include "Relationships.h"

// System includes
#include <vector>
#include <unordered_map>

// Two-letter acronym for easier access to manager
#define MM noname::CreaturesManager::getInstance()

namespace noname
{
    class CreaturesManager : public Manager, public Singleton<CreaturesManager>
    {

    private:
        std::unordered_map<std::string, Creature> _creatures;
        Relationships _relationships;

    public:
        void startUp()
        {
            Manager::setType("CreaturesManager");
            LM.writeLog(Level::Debug, "CreaturesManager::startUp");
            Manager::startUp();
        }

        void shutDown()
        {
            _creatures.clear();
            Manager::shutDown();
            LM.writeLog(Level::Debug, "CreaturesManager::shutDown");
        }

        void initializeCreatures()
        {
            _creatures.insert({"Rat", {"Rat", CreatureType::BEAST}});
            _creatures.insert({"Cat", {"Cat", CreatureType::BEAST}});
            _relationships.add_predator_and_prey({"Cat", CreatureType::BEAST}, {"Rat", CreatureType::BEAST});
        }

        std::unordered_map<std::string, Creature> getCreaturesList() const { return _creatures; }

        Creature getCreature(const std::string &name) { return _creatures.find(name)->second; }
    };

}
#endif // __CREATURES_MANAGER_H__
