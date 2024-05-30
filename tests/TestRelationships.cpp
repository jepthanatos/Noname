#include <gtest/gtest.h>

#include "Character.h"
#include "Relationships.h"

using namespace noname;
using namespace testing;

struct TestRelationships : Test
{
    Character parent1{"Husband"};
    Character parent2{"Wife"};
    Character child1{"Child1"};
    Character child2{"Child2"};
    Relationships<Character> relationships;

    void SetUp() override
    {
        LM.startUp();
        relationships.add_parent_and_child(parent1, child1);
        relationships.add_parent_and_child(parent1, child2);
        relationships.add_parent_and_child(parent2, child1);
        relationships.add_parent_and_child(parent2, child2);
        relationships.add_husband_and_wife(parent1, parent2);
    }
    void TearDown() override
    {
        LM.shutDown();
    }
};

TEST_F(TestRelationships, findAllChildrenOfParents)
{
    EXPECT_TRUE(relationships.find_all_children_of("Husband").size() > 0);

    EXPECT_TRUE(relationships.find_all_children_of("Wife").size() > 0);
}

TEST_F(TestRelationships, findAllChildrenOfChildrenShouldNotReturnAnything)
{
    auto children = relationships.find_all_children_of("Child1");
    EXPECT_TRUE(children.size() == 0);
}

TEST_F(TestRelationships, findParentsOfChild)
{
    EXPECT_TRUE(relationships.find_parents_of("Child1").size() == 2);
}

TEST_F(TestRelationships, findHusbandOf)
{
    EXPECT_TRUE(relationships.find_husband_of("Wife").size() == 1);
}

TEST_F(TestRelationships, findWifeOf)
{
    EXPECT_TRUE(relationships.find_wife_of("Husband").size() == 1);
}
