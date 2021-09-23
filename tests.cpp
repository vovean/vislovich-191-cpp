//
// Created by Vislovich on 24.09.2021.
//

#include <gtest/gtest.h>
#include <sstream>
#include "BigInt.h"

TEST(Base, TestConstructor) {
    // positive cases
    ASSERT_TRUE(BigInt(123) == 123);
    ASSERT_TRUE(BigInt(-123) == -123);
    ASSERT_TRUE(BigInt("123") == 123);
    ASSERT_TRUE(BigInt("-123") == -123);
    ASSERT_NO_FATAL_FAILURE(BigInt("39486709436702986281634849124692356925692569285692385692356239569256123"));
    ASSERT_TRUE(BigInt({0, 1, 2, 3}, true) == BigInt(-123));
    ASSERT_TRUE(BigInt({0, 1, 2, 3}, false) == BigInt(123));

    // leading zeros
    std::stringstream ss;
    ss << BigInt("00012");
    ASSERT_TRUE(ss.str() == "12");
    ss.str("");
    ss << BigInt("-00012");
    ASSERT_TRUE(ss.str() == "-12");


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
}

TEST(Operators, EqGtGteLte) {
    // equal
    ASSERT_TRUE(BigInt("123") == 123);
    ASSERT_TRUE(BigInt("123") == BigInt("123"));
    ASSERT_FALSE(BigInt("-123") == 123);
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
}

TEST(Operators, Plus) {
    // simple cases
    ASSERT_TRUE(BigInt(123) + BigInt("123") == 246);
    ASSERT_TRUE(BigInt(123) + BigInt("23") == 146);
    ASSERT_TRUE(BigInt(123) + BigInt("1123") == 1246);
    // a + (-b)
    ASSERT_TRUE(BigInt(123) + BigInt("-122") == 1);
    ASSERT_TRUE(BigInt(123) + BigInt("-123") == 0);
    ASSERT_TRUE(BigInt(123) + BigInt("-124") == -1);
    // -a + b
    ASSERT_TRUE(BigInt(-123) + BigInt(122) == -1);
    ASSERT_TRUE(BigInt(-123) + BigInt(123) == 0);
    ASSERT_TRUE(BigInt(-123) + BigInt(124) == 1);
    // -a + (-b)
    ASSERT_TRUE(BigInt(-123) + BigInt(-122) == -245);
    ASSERT_TRUE(BigInt(-123) + BigInt(-123) == -246);
    ASSERT_TRUE(BigInt(-123) + BigInt(-124) == -247);
}

TEST(Operators, Minus) {
    // simple cases
    ASSERT_TRUE(BigInt(123) - BigInt("123") == 0);
    ASSERT_TRUE(BigInt(123) - BigInt("124") == -1);
    ASSERT_TRUE(BigInt(123) - BigInt("122") == 1);
    // advanced
    ASSERT_TRUE(BigInt(1000) - BigInt(1) == 999);
    ASSERT_TRUE(BigInt(1010) - BigInt(1) == 1009);
    // -a - (-b)
    ASSERT_TRUE(BigInt(-123) - BigInt("-122") == -1);
    ASSERT_TRUE(BigInt(-123) - BigInt("-123") == 0);
    ASSERT_TRUE(BigInt(-123) - BigInt("-124") == 1);
    // -a - b
    ASSERT_TRUE(BigInt(-123) - BigInt("122") == -245);
    ASSERT_TRUE(BigInt(-123) - BigInt("123") == -246);
    ASSERT_TRUE(BigInt(-123) - BigInt("124") == -247);
    // a - (-b)
    ASSERT_TRUE(BigInt(123) - BigInt("-122") == 245);
    ASSERT_TRUE(BigInt(123) - BigInt("-123") == 246);
    ASSERT_TRUE(BigInt(123) - BigInt("-124") == 247);

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
