#ifndef __SKILL_MANAGER_H__
#define __SKILL_MANAGER_H__

#include "Singleton.h"
#include "Manager.h"
#include "Skill.h"
#include "Player.h"
#include "LogManager.h"

// System includes.
#include <vector>
#include <algorithm>
// #include "spdlog/sinks/basic_file_sink.h"

// Two-letter acronym for easier access to manager.
#define RM_SWORD noname::RankManager<SkillType::SWORD>::getInstance()

namespace noname
{
    template <SkillType _skill>
    class RankManager : public Manager, public Singleton<RankManager<_skill>>
    {

    private:
        std::vector<Player> _players_list;

    public:
        void startUp()
        {
            Manager::setType("RankManager-" + SkillToString(_skill));
            // LM.getLogger()->debug("{} RankManager::startUp", SkillToString(_skill));
            LM.writeLog(Level::Debug, SkillToString(_skill) + " RankManager::startUp");
            Manager::startUp();
        }

        void shutDown()
        {
            _players_list.clear();
            Manager::shutDown();
            // LM.getLogger()->debug("RankManager::shutDown");
            LM.writeLog(Level::Debug, "RankManager::shutDown");
        }

        void updateRanking()
        {
            std::sort(_players_list.begin(), _players_list.end(), [](Player a, Player b)
                      { return a.getSkill(_skill) > b.getSkill(_skill); });
        }

        void addPlayer(const Player &player)
        {
            // LM.writeLog(Level::Debug, "RankManager::addPlayer: " + std::to_string(player.getId()));
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
            auto title{"Ranking of:" + SkillToString(_skill)};
            std::string text = title + "\n";
            for (auto player : _players_list)
            {
                text += "Player ID: " + std::to_string(player.getId()) + " | Skill: " + std::to_string(player.getSkill(_skill)) + "\n";
            }
            LM.writeLog(Level::Debug, text);
            // auto ranking_logger = spdlog::basic_logger_mt("ranking_logger", "log/ranking.txt", true);
            // ranking_logger->set_level(spdlog::level::debug);
            // ranking_logger->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
            // ranking_logger->debug("Ranking of: {}", SkillToString(_skill));
            // for (auto player : _players_list)
            // {
            //     ranking_logger->debug("{} {}", player.getName(), player.getSkill(_skill));
            // }
        }
    };

}
//------------------------------------------------------------------------------
#endif // __SKILL_MANAGER_H__
