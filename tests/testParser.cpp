#include "config/Parser.hpp"

#include <gtest/gtest.h>

class testParser : public ::testing::Test {
    protected:

        testParser(void) {
            system("touch test.conf");
            system("echo -n \"hello world\" > test.conf");
        }

        virtual ~testParser(void) { system("rm test.conf"); }

        virtual void SetUp(void) {}

        virtual void TearDown(void) {}
};

TEST_F(testParser, testFileOpen) {
    ASSERT_THROW(config::Parser(ft::file("file.conf")),
                 ft::InvalidFileException);

    ASSERT_NO_THROW(config::Parser("test.conf"));
}

TEST_F(testParser, testNextLine) {
    config::Parser parser("test.conf");

    ASSERT_TRUE(parser.nextLine());
    ASSERT_EQ(parser.getLine(), "hello world");
    ASSERT_EQ(parser.getPosition(), 1);
    ASSERT_FALSE(parser.nextLine());
    ASSERT_EQ(parser.getLine(), "hello world");
    ASSERT_EQ(parser.getPosition(), 1);
}

TEST_F(testParser, testError) {
    config::Parser parser("test.conf");

    parser.nextLine();
    ASSERT_THROW(parser.error(), config::Parser::InvalidSyntaxException);
}

TEST_F(testParser, testParseLine) {
    config::Parser parser("test.conf");
    ASSERT_THROW(parser.parseLine<config::Listen>({"listen", "80"}),
                 config::Parser::InvalidSyntaxException);

    ASSERT_NO_THROW(parser.parseLine<config::Block>({"server", "{"}));
    ASSERT_THROW(parser.parseLine<config::Block>({"server", "{"}),
                 config::Parser::InvalidSyntaxException);

    ASSERT_THROW(parser.parseLine<config::Listen>({"listen", "80", "80"}),
                 config::Parser::InvalidSyntaxException);
    ASSERT_NO_THROW(parser.parseLine<config::Listen>({"listen", "80"}));
}