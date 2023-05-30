#include "cgi/Argc.hpp"

#include <gtest/gtest.h>

TEST(testArgc, testSimple) {
    cgi::Argc argc({"hello", "world"});
    char**    c_argc = argc;
    EXPECT_STREQ(c_argc[0], "hello");
    EXPECT_STREQ(c_argc[1], "world");
    EXPECT_TRUE(!c_argc[2]);
    ft::array::erase(c_argc);
}

TEST(testArgc, testCopy) {
    cgi::Argc argc({"hello", "world"});
    cgi::Argc argc2(argc);
    char**    c_argc = argc2;
    EXPECT_STREQ(c_argc[0], "hello");
    EXPECT_STREQ(c_argc[1], "world");
    EXPECT_TRUE(!c_argc[2]);
    ft::array::erase(c_argc);
}