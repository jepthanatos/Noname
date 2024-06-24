#ifndef __HERITABLES_H__
#define __HERITABLES_H__

// System includes
#include <vector>
#include <string>
#include <random>

// Local includes
#include "Property.h"
#include "LogManager.h"

namespace noname
{
    enum class HeritableType
    {
        STRENGTH,
        DEXTERY,
        CONSTITUTION,
        INTELLIGENCE,
        CHARISMA,
        GOOD_LOOKING,
        LAST_HERITABLE
    };

    static std::string HeritableToString(HeritableType s)
    {
        switch (s)
        {
        case HeritableType::STRENGTH:
            return "STRENGTH";
        case HeritableType::DEXTERY:
            return "DEXTERY";
        case HeritableType::CONSTITUTION:
            return "CONSTITUTION";
        case HeritableType::INTELLIGENCE:
            return "INTELLIGENCE";
        case HeritableType::CHARISMA:
            return "CHARISMA";
        case HeritableType::GOOD_LOOKING:
            return "GOOD_LOOKING";
        default:
            return "";
        }
    };

    class Heritables
    {
    protected:
        std::vector<Property<short>> _heritables;

    public:
        Heritables()
        {
            // Random initialization for testing - TO BE DONE PROPERLY
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> attributesDist(0, 100);

            for (int i = 0; i < static_cast<int>(HeritableType::LAST_HERITABLE); ++i)
            {
                _heritables.push_back(attributesDist(gen));
            }
        };

        short at(HeritableType type)
        {
            return _heritables.at(static_cast<int>(type));
        }

        void determineHeritablesValues(const Heritables &father, const Heritables &mother)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(1, 2);
            std::uniform_real_distribution<> mutationChance(0.0, 1.0);
            double mutationRate = 0.1; // 10% chance of mutation

            // Function to apply complex mutation
            auto applyComplexMutation = [&](int attribute)
            {
                std::uniform_int_distribution<> mutationTypeDist(1, 3);
                int mutationType = mutationTypeDist(gen);

                // LM.writeLog(Level::Debug, "Character " + std::string{_name} + " has complex mutation of type " + std::to_string(mutationType));

                switch (mutationType)
                {
                case 1:
                { // Random increase/decrease within a range
                    std::uniform_int_distribution<> changeDist(-3, 3);
                    attribute += changeDist(gen);
                    break;
                }
                case 2:
                {                                                            // Percentage change
                    std::uniform_real_distribution<> percentDist(-0.1, 0.1); // -10% to +10%
                    attribute = static_cast<int>(attribute * (1.0 + percentDist(gen)));
                    break;
                }
                case 3:
                { // Random set within a new range
                    std::uniform_int_distribution<> newValueDist(1, 100);
                    attribute = newValueDist(gen);
                    break;
                }
                }

                // Ensure attribute stays within reasonable bounds
                attribute = std::max(1, attribute);
                attribute = std::min(100, attribute);
                return attribute;
            };

            for (int i = 0; i < static_cast<int>(HeritableType::LAST_HERITABLE); ++i)
            {
                short gene = (dist(gen) == 1) ? father._heritables.at(i) : mother._heritables.at(i);
                if (mutationChance(gen) < mutationRate)
                {
                    gene = applyComplexMutation(gene);
                }
                _heritables.at(i) = gene;
            }
        }
    };
}
#endif // __HERITABLES_H__
