#ifndef __RANKING_H__
#define __RANKING_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"
#include "Skill.h"
#include "Player.h"
#include "LogManager.h"

// System includes
#include <vector>
#include <algorithm>

// Acronyms for easier access to managers
#define RANKING_FIST noname::Ranking<SkillType::FIST>::getInstance()
#define RANKING_SWORD noname::Ranking<SkillType::SWORD>::getInstance()
#define RANKING_AXE noname::Ranking<SkillType::AXE>::getInstance()
#define RANKING_CLUB noname::Ranking<SkillType::CLUB>::getInstance()
#define RANKING_DISTANCE noname::Ranking<SkillType::DISTANCE>::getInstance()
#define RANKING_MAGIC noname::Ranking<SkillType::MAGIC>::getInstance()

namespace noname
{
    template <SkillType _skill>
    class Ranking : public Manager, public Singleton<Ranking<_skill>>
    {

    private:
        std::vector<Player> _players_list;
        std::ofstream file;
        std::string rankingFile;

    public:
        void startUp()
        {
            Manager::setType("Ranking-" + SkillToString(_skill));
            LM.writeLog(Level::Debug, Manager::getType() + "::startUp");
            Manager::startUp();
            try
            {
                rankingFile = "Ranking-" + SkillToString(_skill) + ".txt";
                file.open(rankingFile, std::ofstream::out);
            }
            catch (std::ofstream::failure e)
            {
                std::cerr << "Ranking-" << SkillToString(_skill) << "file creation failed." << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }

        void shutDown()
        {
            _players_list.clear();
            file.close();
            Manager::shutDown();
            LM.writeLog(Level::Debug, Manager::getType() + "::shutDown");
        }

        void updateRanking()
        {
            std::sort(_players_list.begin(), _players_list.end(), [](Player a, Player b)
                      { return a.getSkill(_skill) > b.getSkill(_skill); });
        }

        void addPlayer(const Player &player)
        {
            _players_list.emplace_back(player);
            updateRanking();
        }

        std::vector<Player> getRanking() const
        {
            return _players_list;
        }

        short getPlayerRanking(int playerId)
        {
            for (int i = 0; i < _players_list.size(); i++)
            {
                if (_players_list[i].getId() == playerId)
                {
                    return i;
                }
            }
            return -1;
        }

        void printRanking()
        {
            auto title{"Ranking of: " + SkillToString(_skill)};
            std::string text = title + "\n";
            for (auto player : _players_list)
            {
                text += player.getInfoForRanking(_skill);
            }
            file << text << std::endl;
            file.flush();
        }
    };
}
#endif // __RANKING_H__
