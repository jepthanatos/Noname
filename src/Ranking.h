#ifndef __RANKING_H__
#define __RANKING_H__

// Local includes
#include "Singleton.h"
#include "Manager.h"
#include "Skill.h"
#include "Player.h"
#include "LogManager.h"
#include "FileManager.h"
#include "HtmlBuilder.h"

// System includes
#include <vector>
#include <algorithm>

// Acronyms for easier access to rankings
#define RANKING_FIST noname::Ranking<SkillType::FIST>::getInstance()
#define RANKING_SWORD noname::Ranking<SkillType::SWORD>::getInstance()
#define RANKING_AXE noname::Ranking<SkillType::AXE>::getInstance()
#define RANKING_CLUB noname::Ranking<SkillType::CLUB>::getInstance()
#define RANKING_DISTANCE noname::Ranking<SkillType::DISTANCE>::getInstance()
#define RANKING_SHIELDING noname::Ranking<SkillType::SHIELDING>::getInstance()

namespace noname
{
    template <SkillType _skill>
    class Ranking : public Manager, public Singleton<Ranking<_skill>>
    {

    private:
        std::vector<Player> _players_list;
        FileManager rankingFile;

    public:
        void startUp() noexcept
        {
            Manager::setType("Ranking-" + SkillToString(_skill));
            LM.writeLog(Level::Debug, std::string(Manager::getType()) + "::startUp");
            Manager::startUp();
            rankingFile.initOutputFile("Ranking-" + SkillToString(_skill) + ".html");
            rankingFile.startUp();
        }

        void shutDown() noexcept
        {
            _players_list.clear();
            rankingFile.shutDown();
            Manager::shutDown();
            LM.writeLog(Level::Debug, std::string(Manager::getType()) + "::shutDown");
        }

        void updateRanking()
        {
            std::sort(_players_list.begin(), _players_list.end(), [](const Player &a, const Player &b)
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
            LM.writeLog(Level::Debug, title);

            HtmlBuilder rankingTable{"table"};
            rankingTable.add_child("caption", title);

            rankingTable.add_child(HtmlBuilder{"tr"}.add_child("th", "Player ID").add_child("th", "Level").add_child("th", "Skill"));
            for (auto &player : _players_list)
            {
                player.getInfoForRanking(rankingTable, _skill);
            }
            rankingFile.write(rankingTable.str());
        }
    };
}
#endif // __RANKING_H__
