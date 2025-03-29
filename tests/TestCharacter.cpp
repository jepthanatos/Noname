#include <gtest/gtest.h>

// Local Includes
#include "Character.h"
#include "GameManager.h"
#include "WeaponsManager.h"
#include "LogManager.h"
#include "Creature.h"

// System includes
#include <cmath>
#include <math.h>

using namespace noname;
using namespace testing;

TEST(CharacterTest, DefaultConstructor)
{
    noname::Character character;
    EXPECT_EQ(character.getId(), 0);
    EXPECT_EQ(character.getName(), "Noname");
    EXPECT_FALSE(character.isDead());
}

TEST(CharacterTest, ParameterizedConstructor)
{
    std::string name = "Hero";
    noname::Character character{name};
    EXPECT_EQ(character.getName(), name);
}

TEST(CharacterTest, EqualityOperator)
{
    noname::Character character1("Hero1");
    noname::Character character2("Hero2");

    EXPECT_FALSE(character1 == character2);

    character2 = character1;

    EXPECT_TRUE(character1 == character2);
}

struct TestCharacter : Test
{
    Character character{"Noname"};

    void SetUp() override
    {
        LM.startUp();
        WM.startUp();
        character.respawn();
        std::shared_ptr<Weapon> club = WM.getWeapon("Club");
        character.equipWeapon(club);
    }
    void TearDown() override
    {
        WM.shutDown();
        LM.shutDown();
    }
};

TEST_F(TestCharacter, getName)
{
    EXPECT_EQ(character.getName(), "Noname");
}

TEST_F(TestCharacter, getExperience)
{
    EXPECT_EQ(character.getExperience(), 0);
}

TEST_F(TestCharacter, getManaWasted)
{
    EXPECT_EQ(character.getManaWasted(), 0);
}

TEST_F(TestCharacter, getLevel)
{
    EXPECT_EQ(character.getLevel(), 1);
}

TEST_F(TestCharacter, getMagicLevel)
{
    EXPECT_EQ(character.getMagicLevel(), 1);
}

TEST_F(TestCharacter, getSkill)
{
    EXPECT_EQ(character.getSkill(SkillType::CLUB), short(1));
}

TEST_F(TestCharacter, getHealth)
{
    EXPECT_EQ(character.getCurrentHealth(), character.getMaxHealth());
}

TEST_F(TestCharacter, getMaxHealthAtEachLevelFrom1To1000)
{
    Character newCharacter;
    for (int i = 1; i < 1001; i++)
    {
        newCharacter.gainExperience(GM.getExpForLevel(i) - GM.getExpForLevel(i - 1));
        EXPECT_EQ(newCharacter.getLevel(), i);

        ASSERT_TRUE(newCharacter.getMaxHealth() > newCharacter.getHeritable(HeritableType::CONSTITUTION) * newCharacter.getLevel());
    }
}

TEST_F(TestCharacter, getCurrentMana)
{
    EXPECT_TRUE(character.getCurrentMana() == character.getMaxMana());
}

TEST_F(TestCharacter, getMaxManaAtEachLevelFrom1To1000)
{
    Character newCharacter;
    for (int i = 1; i < 1001; i++)
    {
        newCharacter.gainExperience(GM.getExpForLevel(i) - GM.getExpForLevel(i - 1));
        EXPECT_EQ(newCharacter.getLevel(), i);

        ASSERT_TRUE(newCharacter.getMaxMana() > newCharacter.getHeritable(HeritableType::INTELLIGENCE) * newCharacter.getLevel());
    }
}

TEST_F(TestCharacter, getAttackDamage)
{
    EXPECT_GT(character.getAttackDamage(), 0);
}

TEST_F(TestCharacter, getWeapon)
{
    auto club{character.getWeapon()};
    EXPECT_EQ(club, character.getWeapon());
}

TEST_F(TestCharacter, gainExperience)
{
    character.gainExperience(1000);
    EXPECT_EQ(character.getExperience(), 1000);
}

TEST_F(TestCharacter, gainHealth)
{
    if (character.getCurrentHealth() == character.getMaxHealth())
    {
        character.takeDamage(1);
    }
    ASSERT_TRUE(character.getMaxHealth() > character.getCurrentHealth());
    character.gainHealth(character.getMaxHealth() - character.getCurrentHealth());
    EXPECT_EQ(character.getMaxHealth(), character.getCurrentHealth());
}

TEST_F(TestCharacter, takeDamage)
{
    short damage{character.getAttackDamage()};
    int initialHealth{character.getCurrentHealth()};
    character.takeDamage(damage);
    EXPECT_EQ(character.getCurrentHealth(), initialHealth - damage);
}

TEST_F(TestCharacter, takeDamageAndDie)
{
    character.gainExperience(GM.getExpForLevel(2));
    ASSERT_EQ(character.getLevel(), 2);
    ASSERT_EQ(character.getExperience(), GM.getExpForLevel(2));

    while (!character.isDead())
    {
        character.takeDamage(character.getAttackDamage());
    }
    ASSERT_TRUE(character.getCurrentHealth() <= 0);

    character.respawn();
    auto experience = GM.getExpForLevel(2) - ceil((GM.getExpForLevel(2) * 25) / 100);
    EXPECT_EQ(character.getExperience(), experience);
}

TEST_F(TestCharacter, takeDamageDieAndLoseLevel)
{
    character.gainExperience(GM.getExpForLevel(100));
    ASSERT_EQ(character.getLevel(), 100);
    ASSERT_EQ(character.getExperience(), GM.getExpForLevel(100));

    while (!character.isDead())
    {
        character.takeDamage(character.getAttackDamage());
    }
    ASSERT_TRUE(character.getCurrentHealth() <= 0);

    character.respawn();
    auto experience = GM.getExpForLevel(100) - ceil((GM.getExpForLevel(100) * 25) / 100);
    EXPECT_EQ(character.getExperience(), experience);
    EXPECT_EQ(character.getLevel(), 91);
}

TEST_F(TestCharacter, useMana)
{
    ASSERT_TRUE(character.getManaWasted() == 0);
    character.useMana(1);
    EXPECT_EQ(character.getManaWasted(), 1);
}

TEST_F(TestCharacter, gainMana)
{
    if (character.getCurrentMana() == character.getMaxMana())
    {
        character.useMana(1);
    }
    EXPECT_GT(character.getMaxMana(), character.getCurrentMana());
    character.gainMana(character.getMaxMana() - character.getCurrentMana());
    EXPECT_EQ(character.getMaxMana(), character.getCurrentMana());
}

TEST_F(TestCharacter, respawn)
{
    character.takeDamage(character.getCurrentHealth());

    ASSERT_TRUE(character.isDead());

    character.respawn();

    EXPECT_FALSE(character.isDead());
    EXPECT_GT(character.getCurrentHealth(), 0);
}

TEST_F(TestCharacter, attack)
{
    std::unique_ptr<Character> beast = std::make_unique<Creature>();
    beast->gainExperience(GM.getExpForLevel(1000));
    for (int i = 0; i < 1000; ++i)
    {
        character.attack(*beast);
    }
    Property<short> expectedSkill{1};
    ASSERT_TRUE(character.getSkill(character.getWeapon()->getSkillType()) > expectedSkill);
}

TEST_F(TestCharacter, defense)
{
    short initialHealth = character.getCurrentHealth();
    short damage = 20;

    character.defense(damage);

    EXPECT_GT(character.getCurrentHealth(), initialHealth - damage);
}

TEST_F(TestCharacter, PickItemStoresItemInInventory)
{
    std::unique_ptr<Item> amulet = std::make_unique<Item>("Amulet", ItemType::AMULET, ItemRank::NORMAL);
    character.pick(std::move(amulet), ItemSlotType::AMULET);
    const auto &slots = character.getInventorySlots();
    bool found = false;
    for (const auto &slot : slots)
    {
        if (slot.get() && slot->getName() == "Amulet")
        {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

TEST_F(TestCharacter, DropItemRemovesItemFromInventory)
{
    auto slotsBefore = character.getInventorySlots();
    std::unique_ptr<Item> amulet = std::make_unique<Item>("Amulet", ItemType::AMULET, ItemRank::NORMAL);
    character.pick(std::move(amulet), ItemSlotType::AMULET);
    auto slotsAfter = character.getInventorySlots();
    EXPECT_NE(slotsBefore, slotsAfter);
    character.drop(ItemSlotType::AMULET);
    auto slotsFinal = character.getInventorySlots();
    EXPECT_EQ(slotsBefore, slotsFinal);
}

TEST_F(TestCharacter, DropWeaponRemovesWeapon)
{
    character.drop(ItemSlotType::WEAPON);
    EXPECT_EQ(character.getWeapon()->getName(), "Fists");
    std::shared_ptr<Weapon> handaxe = WM.getWeapon("Handaxe");
    character.equipWeapon(handaxe);
    EXPECT_EQ(character.getWeapon()->getName(), "Handaxe");
    character.drop(ItemSlotType::WEAPON);
    EXPECT_EQ(character.getWeapon()->getName(), "Fists");
}