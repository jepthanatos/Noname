#include <gtest/gtest.h>

#include "Ranking.h"
#include "Utils.h"

using namespace noname;
using namespace testing;

class TestRanking : public Test
{
};

TEST_F(TestRanking, startUp)
{
    RANKING_CLUB.startUp();
    EXPECT_EQ(true, RANKING_CLUB.isStarted());
}

TEST_F(TestRanking, shutDown)
{
    RANKING_CLUB.shutDown();
    EXPECT_EQ(false, RANKING_CLUB.isStarted());
}

TEST_F(TestRanking, addPlayer)
{
    RANKING_CLUB.startUp();
    Player player{};
    RANKING_CLUB.addPlayer(player);
    std::vector<Player> ranking{RANKING_CLUB.getRanking()};
    std::vector<Player> myRanking{std::move(player)};
    ASSERT_EQ(myRanking, ranking);
    RANKING_CLUB.shutDown();
}

TEST_F(TestRanking, add100Players)
{
    LM.startUp();
    RANKING_CLUB.startUp();
    for (int i = 0; i < 100; ++i)
    {
        Player player{};
        player.equipWeapon("Club");
        player.addExperience(Character::getExpForLevel(Utils::rollDie(1, 100)));
        for (int j = 1; j < Utils::rollDie(1, 10000); ++j)
            player.attack();
        RANKING_CLUB.addPlayer(std::move(player));
    }
    RANKING_CLUB.printRanking();
    RANKING_CLUB.shutDown();
    LM.shutDown();
}