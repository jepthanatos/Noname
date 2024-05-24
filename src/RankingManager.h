#ifndef __RANKING_MANAGER_H__
#define __RANKING_MANAGER_H__

// Local includes
#include "Ranking.h"

// Two-letter acronym for easier access to manager
#define RM noname::RankingManager::getInstance()

namespace noname
{
    class RankingManager : public Manager, public Singleton<RankingManager>
    {
    public:
        void startUp()
        {
            Manager::setType("RankingManager");
            LM.writeLog(Level::Debug, "RankingManager::startUp");
            RANKING_FIST.startUp();
            RANKING_SWORD.startUp();
            RANKING_AXE.startUp();
            RANKING_CLUB.startUp();
            RANKING_DISTANCE.startUp();
            RANKING_MAGIC.startUp();
            Manager::startUp();
        }

        void shutDown()
        {
            RANKING_FIST.shutDown();
            RANKING_SWORD.shutDown();
            RANKING_AXE.shutDown();
            RANKING_CLUB.shutDown();
            RANKING_DISTANCE.shutDown();
            RANKING_MAGIC.shutDown();
            LM.writeLog(Level::Debug, "RankingManager::shutDown");
            Manager::shutDown();
        }

        void updateAllRankings()
        {
            RANKING_FIST.updateRanking();
            RANKING_SWORD.updateRanking();
            RANKING_AXE.updateRanking();
            RANKING_CLUB.updateRanking();
            RANKING_DISTANCE.updateRanking();
            RANKING_MAGIC.updateRanking();
        }

        void addPlayer(const Player &player)
        {
            RANKING_FIST.addPlayer(std::move(player));
            RANKING_SWORD.addPlayer(std::move(player));
            RANKING_AXE.addPlayer(std::move(player));
            RANKING_CLUB.addPlayer(std::move(player));
            RANKING_DISTANCE.addPlayer(std::move(player));
            RANKING_MAGIC.addPlayer(std::move(player));
        }

        short getPlayerRanking(int playerId, SkillType skill)
        {
            switch (skill)
            {
            case SkillType::FIST:
                return RANKING_FIST.getPlayerRanking(playerId);
                break;
            case SkillType::SWORD:
                return RANKING_SWORD.getPlayerRanking(playerId);
                break;
            case SkillType::AXE:
                return RANKING_AXE.getPlayerRanking(playerId);
                break;
            case SkillType::CLUB:
                return RANKING_CLUB.getPlayerRanking(playerId);
                break;
            case SkillType::DISTANCE:
                return RANKING_DISTANCE.getPlayerRanking(playerId);
                break;
            case SkillType::MAGIC:
                return RANKING_MAGIC.getPlayerRanking(playerId);
                break;
            default:
                return -1;
                break;
            }
        }

        void printAllRankings()
        {
            LM.writeLog(Level::Debug, "RankingManager::printAllRankings");
            updateAllRankings();
            RANKING_FIST.printRanking();
            RANKING_SWORD.printRanking();
            RANKING_AXE.printRanking();
            RANKING_CLUB.printRanking();
            RANKING_DISTANCE.printRanking();
            RANKING_MAGIC.printRanking();
        }
    };
}
#endif // __RANKING_MANAGER_H__
