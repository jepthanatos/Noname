#ifndef __RANKING_MANAGER_H__
#define __RANKING_MANAGER_H__

// Local includes
#include "Ranking.h"

namespace noname
{
    class RankingManager : public Singleton<RankingManager>
    {
    public:
        void startUp()
        {
            LM.writeLog(Level::Debug, "RankingManager::startUp");
            RANKING_FIST.startUp();
            RANKING_SWORD.startUp();
            RANKING_AXE.startUp();
            RANKING_CLUB.startUp();
            RANKING_DISTANCE.startUp();
            RANKING_MAGIC.startUp();
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
            auto title{"RankingManager of: " + SkillToString(_skill)};
            std::string text = title + "\n";
            for (auto player : _players_list)
            {
                text += player.getInfoForRanking(_skill);
            }
            LM.writeLog(Level::Debug, text);
        }
    };
}
#endif // __RANKING_MANAGER_H__
