#include <gtest/gtest.h>

#include "RankManager.h"
#include "Utils.h"

using namespace noname;
using namespace testing;

class TestRankManager : public Test
{
};

TEST_F(TestRankManager, startUp)
{
    RM_CLUB.startUp();
    EXPECT_EQ(true, RM_CLUB.isStarted());
}

TEST_F(TestRankManager, shutDown)
{
    RM_CLUB.shutDown();
    EXPECT_EQ(false, RM_CLUB.isStarted());
}

TEST_F(TestRankManager, addPlayer)
{
    RM_CLUB.startUp();
    Player player{};
    RM_CLUB.addPlayer(player);
    std::vector<Player> ranking{RM_CLUB.getRanking()};
    std::vector<Player> myRanking{std::move(player)};
    ASSERT_EQ(myRanking, ranking);
    RM_CLUB.shutDown();
}

TEST_F(TestRankManager, add100Players)
{
    LM.startUp();
    RM_CLUB.startUp();
    for (int i = 0; i < 100; ++i)
    {
        Player player{};
        player.equipWeapon("Club");
        player.addExperience(Character::getExpForLevel(Utils::rollDice(1, 100)));
        for (int j = 1; j < Utils::rollDice(1, 10000); ++j)
            player.attack();
        RM_CLUB.addPlayer(std::move(player));
    }
    RM_CLUB.printRanking();
    RM_CLUB.shutDown();
    LM.shutDown();
}