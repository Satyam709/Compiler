//
// Created by satya on 26-02-2025.
//

#include "LexerTests.h"

#include <string>
#include <gtest/gtest.h>  // Ensure GTest is included

// Basic Math Test
TEST(MathTest, Addition) {
    EXPECT_EQ(2 + 3, 5);
    EXPECT_NE(2 + 3, 6);
}

// String Test
TEST(StringTest, Compare) {
    std::string str1 = "hello";
    std::string str2 = "hello";
    std::string str3 = "world";

    EXPECT_EQ(str1, str2);
    EXPECT_NE(str1, str3);
}

// Fixture-Based Test (Reusing Setup)
class SampleFixture : public ::testing::Test {
protected:
    void SetUp() override {
        data = 10;
    }
    int data;
};

TEST_F(SampleFixture, CheckData) {
    EXPECT_EQ(data, 10);
}

// Parameterized Test
class ParamTest : public ::testing::TestWithParam<int> {};

TEST_P(ParamTest, IsEven) {
    int value = GetParam();
    EXPECT_EQ(value % 2, 0);
}

INSTANTIATE_TEST_SUITE_P(Values, ParamTest, ::testing::Values(2, 4, 6, 8));
