#include "cgi/CgiHandler.hpp"

#include <gtest/gtest.h>

class testCgiHandler : public ::testing::Test {
    protected:

        testCgiHandler(void) {

            // Prepare the HTTP request
            std::string reqStr =
                "GET /cgi/python/me.py?param1=value1&param2=value2 "
                "HTTP/1.1\r\n"
                "Host: localhost\r\n"
                "User-Agent: Mozilla/5.0\r\n"
                "\r\n";

            // Call your CGI handler to process the request
            m_request = new http::Request(reqStr);
            m_route = new http::Route("/cgi/python", "./websites/cgi/python");

            // // Assert on the expected response
            // std::string expectedResponse = "Content-Type: text/html\r\n\r\n";
            // expectedResponse += "<html><body>\n";
            // expectedResponse += "<h1>Query Parameters:</h1>\n";
            // expectedResponse += "<ul>\n";
            // expectedResponse += "<li>param1=value1</li>\n";
            // expectedResponse += "<li>param2=value2</li>\n";
            // expectedResponse += "</ul>\n";
            // expectedResponse += "</body></html>\n";

            // EXPECT_EQ(response, expectedResponse);
        }

        ~testCgiHandler(void) {
            delete m_request;
            delete m_route;
        }

        void SetUp(void) {}

        void TearDown(void) {}

        http::Request* m_request;
        http::Route*   m_route;
};

TEST_F(testCgiHandler, testGetPath) {
    cgi::CgiHandler cgiHandler(*m_request, *m_route);
    ASSERT_EQ(std::string(cgiHandler.getPath()), "./websites/cgi/python/");
}

TEST_F(testCgiHandler, testGetEnvp) {
    cgi::CgiHandler cgiHandler(*m_request, *m_route);
    ASSERT_EQ(std::string(cgiHandler.getEnvp()[0]),
              "DOCUMENT_ROOT=./websites/cgi/python/");
}

TEST_F(testCgiHandler, testGetArgv) {
    cgi::CgiHandler cgiHandler(*m_request, *m_route);
    char**          argv = cgiHandler.getArgv();
    ASSERT_EQ(std::string(argv[0]), "./websites/cgi/python/") << argv[0];
    ASSERT_EQ(std::string(argv[1]), "./websites/cgi/python/me.py") << argv[0];
    ASSERT_TRUE(!argv[2]);
}

TEST_F(testCgiHandler, testRunChildProccess) {
    cgi::CgiHandler cgiHandler(*m_request, *m_route);
    auto            ret = cgiHandler.run();
    std::cout << ret.first << std::endl;
    ASSERT_EQ(ret.second, 200);
}