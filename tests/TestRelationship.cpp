#include <gtest/gtest.h>

#include "Relationship.h"

using namespace noname;
using namespace testing;

struct TestRelationship : Test
{
    Character parent{"Parent"};
    Character child1{"Child1"};
    Character child2{"Child2"};
    Relationships relationships;

    void SetUp() override
    {
        LM.startUp();
        relationships.add_parent_and_child(parent, child1);
        relationships.add_parent_and_child(parent, child2);
    }
    void TearDown() override
    {
        LM.shutDown();
    }
};

TEST_F(TestRelationship, findAllChildrenOfParent)
{
    auto children = relationships.find_all_children_of("Parent");
    EXPECT_TRUE(children.size() > 0);
}

TEST_F(TestRelationship, findAllChildrenOfChildren)
{
    auto children = relationships.find_all_children_of("Children1");
    EXPECT_FALSE(children.size() > 0);
}