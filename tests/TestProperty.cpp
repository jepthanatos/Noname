#include <gtest/gtest.h>

#include "Property.h"

using namespace noname;
using namespace testing;

struct TestClassProperty : Test
{
    Property<int> intProperty{7};
};

TEST_F(TestClassProperty, operatorT)
{
    EXPECT_EQ(intProperty, 7);
}

TEST_F(TestClassProperty, operatorEqual)
{
    intProperty = 3;
    EXPECT_EQ(intProperty, 3);
}

TEST_F(TestClassProperty, operatoGreater)
{
    EXPECT_TRUE(intProperty > 1);
}

TEST_F(TestClassProperty, operatorPlusPlus)
{
    ++intProperty;
    EXPECT_EQ(intProperty, 8);
}

TEST_F(TestClassProperty, operatorMinusMinus)
{
    --intProperty;
    EXPECT_EQ(intProperty, 6);
}

TEST_F(TestClassProperty, operatorPlusEqual)
{
    int value{2};
    intProperty += value;
    EXPECT_EQ(intProperty, 9);
}

TEST_F(TestClassProperty, operatorMinusEqual)
{
    int value{4};
    intProperty -= value;
    EXPECT_EQ(intProperty, 3);
}

TEST_F(TestClassProperty, toString)
{
    EXPECT_EQ(intProperty.toString(), "7");
}