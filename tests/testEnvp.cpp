#include "cgi/Envp.hpp"

#include <gtest/gtest.h>

class testEnvp : public ::testing::Test {
    protected:

        testEnvp(void) : req1("") {
            std::string req =
                "GET "
                "/cgi/myscript.py/path/to/script?param1=value1&param2=value2 "
                "HTTP/1.1\r\n"
                "Host: example.com\r\n"
                "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 "
                "Safari/537.36\r\n"
                "Accept: "
                "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
                "avif,image/webp,image/apng,*/*;q=0.8,application/"
                "signed-exchange;v=b3;q=0.9\r\n"
                "Accept-Encoding: gzip, deflate, br\r\n"
                "Accept-Language: en-US,en;q=0.9\r\n"
                "Connection: keep-alive\r\n\r\n";
            req1 = http::Request(req);
        }

        ~testEnvp(void) override {}

        void SetUp(void) override {}

        void TearDown(void) override {}

        http::Request req1;
};

TEST_F(testEnvp, testBasic) {
    cgi::Envp envp(req1, http::Route("/cgi", "/var/www/html"));

    EXPECT_EQ(envp.get("CONTENT_LENGTH"), "");
    EXPECT_EQ(envp.get("CONTENT_TYPE"), "");
    EXPECT_EQ(envp.get("REQUEST_METHOD"), "GET");
    EXPECT_EQ(envp.get("SERVER_PROTOCOL"), "HTTP/1.1");

    EXPECT_EQ(envp.get("PATH_INFO"), "");
    EXPECT_EQ(envp.get("SCRIPT_NAME"), "");

    EXPECT_EQ(envp.get("QUERY_STRING"), "param1=value1&param2=value2");
    EXPECT_EQ(envp.get("USER_AGENT"), "Mozilla/5.0 (Windows NT 10.0; Win64; "
                                      "x64) AppleWebKit/537.36 (KHTML, like "
                                      "Gecko) Chrome/96.0.4664.45 "
                                      "Safari/537.36");

    EXPECT_EQ(envp.get("PATH_TRANSLATED"), "");
    EXPECT_EQ(envp.get("DOCUMENT_ROOT"), "");

    EXPECT_EQ(envp.get("SERVER_PORT"), "");
}