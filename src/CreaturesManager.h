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
#define CM noname::CreaturesManager::getInstance()

namespace noname
{
    class CreaturesManager : public Manager, public Singleton<CreaturesManager>
    {

    private:
        std::unordered_map<std::string, std::shared_ptr<noname::Creature>> _creatures;
        Relationships<Creature> _relationships;

    public:
        void startUp() noexcept
        {
            Manager::setType("CreaturesManager");
            LM.writeLog(Level::Debug, "CreaturesManager::startUp");
            Manager::startUp();
            initializeCreatures();
        }

        void shutDown() noexcept
        {
            _creatures.clear();
            Manager::shutDown();
            LM.writeLog(Level::Debug, "CreaturesManager::shutDown");
        }

        void initializeCreatures()
        {
            std::shared_ptr<Creature> rat = std::make_shared<Creature>("Rat", CreatureType::BEAST);
            std::shared_ptr<Creature> cat = std::make_shared<Creature>("Cat", CreatureType::BEAST);
            _creatures.emplace(rat->getName(), rat);
            _creatures.emplace(cat->getName(), cat);
            _relationships.add_predator_and_prey(*cat, *rat);
        }

        std::unordered_map<std::string, std::shared_ptr<noname::Creature>> getCreaturesList() const
        {
            return _creatures;
        }

        std::shared_ptr<noname::Creature> &getCreature(const std::string &name)
        {
            return _creatures.at(name);
        }

        std::vector<std::shared_ptr<Creature>> findPreyOf(const std::string &name)
        {
            auto prey = _relationships.find_prey_of(name);
            std::vector<std::shared_ptr<Creature>> preyPtrs;
            for (auto &p : prey)
            {
                preyPtrs.push_back(std::make_shared<Creature>(p));
            }
            return preyPtrs;
        }

        std::vector<std::shared_ptr<Creature>> findPredatorOf(const std::string &name)
        {
            auto predators = _relationships.find_predator_of(name);
            std::vector<std::shared_ptr<Creature>> predatorPtrs;
            for (auto &predator : predators)
            {
                predatorPtrs.push_back(std::make_shared<Creature>(predator));
            }
            return predatorPtrs;
        }
    };
}
#endif // __CREATURES_MANAGER_H__
