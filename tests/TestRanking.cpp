#include <gtest/gtest.h>

// Local includes
#include "Ranking.h"
#include "Utils.h"
#include "GameManager.h"

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
    WM.startUp();
    RANKING_CLUB.startUp();
    for (int i = 0; i < 100; ++i)
    {
        Player player{};
        std::shared_ptr<Weapon> club = WM.getWeapon("Club");
        player.equipWeapon(club);
        player.gainExperience(GM.getExpForLevel(Utils::rollDie(1, 100)));
        auto attackNumber{Utils::rollDie(1, 1000)};
        // for (int j = 1; j < attackNumber; ++j)
        // player.attack();
        RANKING_CLUB.addPlayer(std::move(player));
    }
    RANKING_CLUB.printRanking();
    RANKING_CLUB.shutDown();
    WM.shutDown();
    LM.shutDown();
}