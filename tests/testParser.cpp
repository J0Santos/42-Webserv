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

class testParseLine : public ::testing::Test {
    protected:

        testParseLine(void) {
            system("touch test2.conf");
            system("echo \"server {\n"
                   "  listen      8080;\n"
                   "  location / {\n"
                   "    root          ./websites/cgi;\n"
                   "    allow_methods GET POST;\n"
                   "    fastcgi       .py;\n"
                   "    index         ./websites/index.html;\n"
                   "    autoindex     on;\n"
                   "  }\n"
                   "  root          ./websites/;\n"
                   "  server_name   domain.com;\n"
                   "  allow_methods GET;\n"
                   "  location /python {\n"
                   "    root          ./websites/;\n"
                   "    autoindex     on;\n"
                   "  }\n"
                   "}\n"
                   "\n"
                   "server {\n"
                   "  listen         8081:localhost;\n"
                   "  root           ./websites/;\n"
                   "  server_name    domain.net;\n"
                   "  error_page 404 ./websites/errors/404.html;\n"
                   "}\" > test2.conf");
        }

        virtual ~testParseLine(void) { system("rm test2.conf"); }

        virtual void SetUp(void) {}

        virtual void TearDown(void) {}
};

TEST_F(testParseLine, testParseLineStatusManagement) {
    config::Parser parser("test2.conf");
    ASSERT_THROW(parser.parseLine<config::Listen>({"listen", "80"}),
                 config::Parser::InvalidSyntaxException);

    ASSERT_NO_THROW(parser.parseLine<config::Block>({"server", "{"}));
    ASSERT_THROW(parser.parseLine<config::Block>({"server", "{"}),
                 config::Parser::InvalidSyntaxException);

    ASSERT_THROW(parser.parseLine<config::Listen>({"listen", "80", "80"}),
                 config::Parser::InvalidSyntaxException);
    ASSERT_NO_THROW(parser.parseLine<config::Listen>({"listen", "80"}));

    ASSERT_NO_THROW(parser.parseLine<config::Route>({"location", "/", "{"}));
    ASSERT_THROW(parser.parseLine<config::Route>({"location", "/", "{"}),
                 config::Parser::InvalidSyntaxException);

    ASSERT_NO_THROW(parser.parseLine<config::End>({"}"}));
    ASSERT_NO_THROW(parser.parseLine<config::End>({"}"}));
    ASSERT_THROW(parser.parseLine<config::End>({"}"}),
                 config::Parser::InvalidSyntaxException);
}

TEST_F(testParseLine, testParseFunctionWithParseLine) {
    config::Parser             parser("test2.conf");
    std::vector<config::block> blocks = config::parse("test2.conf");
    EXPECT_EQ(blocks[0].m_port, "8080");
    EXPECT_EQ(blocks[0].m_root, "./websites/");
    EXPECT_EQ(blocks[0].m_server_name, "domain.com");
    EXPECT_EQ(blocks[0].m_allowed_methods, std::vector<std::string>({"GET"}));
    EXPECT_EQ(blocks[0].m_routes[0].m_target, "/");
    EXPECT_EQ(blocks[0].m_routes[0].m_root, "./websites/cgi");
    EXPECT_EQ(blocks[0].m_routes[0].m_allowed_methods,
              std::vector<std::string>({"GET", "POST"}));
    EXPECT_EQ(blocks[0].m_routes[0].m_cgi_extension, ".py");
    EXPECT_EQ(blocks[0].m_routes[0].m_index, "./websites/index.html");
    EXPECT_EQ(blocks[0].m_routes[0].m_autoindex, true);
    EXPECT_EQ(blocks[0].m_routes[0].m_closed, true);
    EXPECT_EQ(blocks[0].m_routes[1].m_target, "/python");
    EXPECT_EQ(blocks[0].m_routes[1].m_root, "./websites/");
    EXPECT_EQ(blocks[0].m_routes[1].m_autoindex, true);
    EXPECT_EQ(blocks[0].m_routes[1].m_closed, true);
    EXPECT_EQ(blocks[0].m_closed, true);
    EXPECT_EQ(blocks[1].m_port, "8081");
    EXPECT_EQ(blocks[1].m_host, "localhost");
    EXPECT_EQ(blocks[1].m_root, "./websites/");
    EXPECT_EQ(blocks[1].m_server_name, "domain.net");
    EXPECT_EQ(blocks[1].m_error_pages[404], "./websites/errors/404.html");
    EXPECT_EQ(blocks[1].m_closed, true);
}