//
// Created by Vislovich on 24.09.2021.
//

#include <gtest/gtest.h>
#include <sstream>
#include "BigInt.h"

TEST(Base, Base) {
    ASSERT_EQ(
            BigInt("221736452374532874523853295623956283753825328456329568325829356") +
            BigInt("82735421834523856239563290563285632854238452387453854823582358325823619643109652935"),
            BigInt("82735421834523856239785027015660165728762305683077811107336183654279949211435482291")
    );
}

TEST(Base, TestConstructor) {
    // positive cases
    ASSERT_EQ(BigInt(123), 123);
    ASSERT_EQ(BigInt(-123), -123);
    ASSERT_EQ(BigInt("123"), 123);
    ASSERT_EQ(BigInt("-123"), -123);
    ASSERT_NO_FATAL_FAILURE(BigInt("39486709436702986281634849124692356925692569285692385692356239569256123"));
    ASSERT_EQ(BigInt({0, 1, 2, 3}, true), BigInt(-123));
    ASSERT_EQ(BigInt({0, 1, 2, 3}, false), BigInt(123));

    // leading zeros
    std::stringstream ss;
    ss << BigInt("00012");
    ASSERT_EQ(ss.str(), "12");
    ss.str("");
    ss << BigInt("-00012");
    ASSERT_EQ(ss.str(), "-12");


    // negative cases
    ASSERT_EXIT(
            BigInt("-12854-9476346"),
            ::testing::ExitedWithCode(1),
            "Invalid value for integer: -12854-9476346"
    );
    ASSERT_EXIT(
            BigInt("000-9476346"),
            ::testing::ExitedWithCode(1),
            "Invalid value for integer: 000-9476346"
    );
    ASSERT_EXIT(
            BigInt("-000-9476346"),
            ::testing::ExitedWithCode(1),
            "Invalid value for integer: -000-9476346"
    );
    ASSERT_EXIT(
            BigInt("--9476346"),
            ::testing::ExitedWithCode(1),
            "Invalid value for integer: --9476346"
    );
}

TEST(Operators, Less) {
    // equal
    ASSERT_FALSE(BigInt(123) < BigInt(123));
    // different negativity
    ASSERT_FALSE(BigInt(123) < BigInt(-123));
    ASSERT_TRUE(BigInt(-123) < BigInt(123));
    // different digits amount
    ASSERT_TRUE(BigInt(-1234) < BigInt(-123));
    ASSERT_FALSE(BigInt(-123) < BigInt(-1234));
    ASSERT_FALSE(BigInt(1234) < BigInt(123));
    ASSERT_TRUE(BigInt(123) < BigInt(1234));
    // equal digits count, equal negativity
    ASSERT_FALSE(BigInt("112") < BigInt("111"));
    ASSERT_FALSE(BigInt("-111") < BigInt("-112"));
    ASSERT_TRUE(BigInt("-112") < BigInt("-111"));
    ASSERT_TRUE(BigInt("111") < BigInt("112"));
    // nums
    ASSERT_LT(BigInt(112), 113);
}

TEST(Operators, EqGtGteLte) {
    // equal
    ASSERT_EQ(BigInt("123"), 123);
    ASSERT_EQ(BigInt("123"), BigInt("123"));
    ASSERT_NE(BigInt("-123"), 123);
    // greater
    ASSERT_TRUE(BigInt("123") > BigInt(122));
    ASSERT_FALSE(BigInt("123") > BigInt(123));
    ASSERT_FALSE(BigInt("123") > BigInt(124));
    // greater or equal
    ASSERT_TRUE(BigInt("123") >= BigInt(122));
    ASSERT_TRUE(BigInt("123") >= BigInt(123));
    ASSERT_FALSE(BigInt("123") >= BigInt(124));
    // less or equal
    ASSERT_FALSE(BigInt("123") <= BigInt(122));
    ASSERT_TRUE(BigInt("123") <= BigInt(123));
    ASSERT_TRUE(BigInt("123") <= BigInt(124));
    // nums
    ASSERT_GT(BigInt(112), 11);
    ASSERT_GE(BigInt(112), 112);
    ASSERT_GE(BigInt(113), 112);
    ASSERT_LE(BigInt(112), 1112);
    ASSERT_LE(BigInt(1112), 1112);
}

TEST(Operators, Plus) {
    // simple cases
    ASSERT_EQ(BigInt(123) + BigInt("123"), 246);
    ASSERT_EQ(BigInt(123) + BigInt("23"), 146);
    ASSERT_EQ(BigInt(123) + BigInt("1123"), 1246);
    ASSERT_EQ(BigInt(999) + BigInt("2"), 1001);
    // advanced case
    ASSERT_EQ(BigInt(163) + BigInt("1169"), 1332);
    // a + (-b)
    ASSERT_EQ(BigInt(123) + BigInt("-122"), 1);
    ASSERT_EQ(BigInt(123) + BigInt("-123"), 0);
    ASSERT_EQ(BigInt(123) + BigInt("-124"), -1);
    // -a + b
    ASSERT_EQ(BigInt(-123) + BigInt(122), -1);
    ASSERT_EQ(BigInt(-123) + BigInt(123), 0);
    ASSERT_EQ(BigInt(-123) + BigInt(124), 1);
    // -a + (-b)
    ASSERT_EQ(BigInt(-123) + BigInt(-122), -245);
    ASSERT_EQ(BigInt(-123) + BigInt(-123), -246);
    ASSERT_EQ(BigInt(-123) + BigInt(-124), -247);
    // nums
    ASSERT_EQ(BigInt(123) + 10, 133);
    // +=
    BigInt a(10);
    a += 10;
    ASSERT_EQ(a, 20);
}

TEST(Operators, Minus) {
    // simple cases
    ASSERT_EQ(BigInt(123) - BigInt("123"), 0);
    ASSERT_EQ(BigInt(123) - BigInt("124"), -1);
    ASSERT_EQ(BigInt(123) - BigInt("122"), 1);
    // advanced
    ASSERT_EQ(BigInt(1000) - BigInt(1), 999);
    ASSERT_EQ(BigInt(1010) - BigInt(1), 1009);
    // -a - (-b)
    ASSERT_EQ(BigInt(-123) - BigInt("-122"), -1);
    ASSERT_EQ(BigInt(-123) - BigInt("-123"), 0);
    ASSERT_EQ(BigInt(-123) - BigInt("-124"), 1);
    // -a - b
    ASSERT_EQ(BigInt(-123) - BigInt("122"), -245);
    ASSERT_EQ(BigInt(-123) - BigInt("123"), -246);
    ASSERT_EQ(BigInt(-123) - BigInt("124"), -247);
    // a - (-b)
    ASSERT_EQ(BigInt(123) - BigInt("-122"), 245);
    ASSERT_EQ(BigInt(123) - BigInt("-123"), 246);
    ASSERT_EQ(BigInt(123) - BigInt("-124"), 247);
    // nums
    ASSERT_EQ(BigInt(-123) - 10, -133);
    // -=
    BigInt a(10);
    a -= 5;
    ASSERT_EQ(a, 5);
}

TEST(Operators, IncDec) {
    BigInt a(199);
    // post inc
    ASSERT_EQ(a++, 199);
    ASSERT_EQ(a, 200);
    // pre inc
    ASSERT_EQ(++a, 201);
    // post dec
    ASSERT_EQ(a--, 201);
    ASSERT_EQ(a, 200);
    // pre dec
    ASSERT_EQ(--a, 199);
}

TEST(Operators, Equality) {
    // equality
    ASSERT_TRUE(BigInt(123) == 123);
    ASSERT_TRUE(BigInt(123) == BigInt(123));
    // inequality
    ASSERT_TRUE(BigInt(123) != 122);
    ASSERT_TRUE(BigInt(123) != BigInt(122));
}

TEST(Operators, Unary) {
    ASSERT_EQ(-BigInt(123), -123);
    ASSERT_EQ(-BigInt(-123), 123);
    ASSERT_EQ(-(-BigInt(-123)), -123);
}

TEST(Operators, Multiply) {
    // simple cases
    ASSERT_EQ(BigInt(2) * BigInt(2), 4);
    ASSERT_EQ(BigInt(2) * BigInt(-2), -4);
    ASSERT_EQ(BigInt(-2) * BigInt(2), -4);
    ASSERT_EQ(BigInt(-2) * BigInt(-2), 4);
    // nums
    ASSERT_EQ(BigInt(2) * 2, 4);
    ASSERT_EQ(BigInt(2) * -2, -4);
    ASSERT_EQ(BigInt(-2) * 2, -4);
    ASSERT_EQ(BigInt(-2) * -2, 4);
    // advanced case
    ASSERT_EQ(BigInt("32465825956543") * BigInt("9546754906740"), BigInt("309943283251993739257799820"));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
