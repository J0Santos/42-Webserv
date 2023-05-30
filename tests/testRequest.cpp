#include "http/Request.hpp"

#include <gtest/gtest.h>

TEST(testRequest, testConstructor) {
    std::string   req = "GET / HTTP/1.1\r\nHost: x\r\nContent-Length: "
                        "14\r\n\r\nThis is a body";
    http::Request request(req);
    EXPECT_EQ(request.getMethod(), http::MethodType::GET);
    EXPECT_EQ(request.getVersion(), http::Version::HTTP_1_1);
    EXPECT_EQ(request.getHeader("Host"), "x");
    EXPECT_EQ(request.getHeader("Content-Length"), "14");
    EXPECT_EQ(request.getBody(), "This is a body");

    // empty method
    ASSERT_THROW(http::Request(""), http::Request::MalformedRequestException);

    // no path
    ASSERT_THROW(http::Request("GET HTTP/1.1\r\nHost: x\r\nContent-Length: "
                               "15\r\n\r\nThis is a body"),
                 http::Request::MalformedRequestException);
    // no version
    ASSERT_THROW(http::Request("GET / \r\nHost: x\r\nContent-Length: "
                               "15\r\n\r\nThis is a body"),
                 http::Request::MalformedRequestException);

    // wrong first line
    ASSERT_THROW(
        http::Request(
            "GET somthing somethingelseHTTP/1.1\r\nHost: x\r\nContent-Length: "
            "15\r\n\r\nThis is a body"),
        http::Request::MalformedRequestException);

    // wrong first line
    ASSERT_THROW(
        http::Request("somthing GET / HTTP/1.1\r\nHost: x\r\nContent-Length: "
                      "15\r\n\r\nThis is a body"),
        http::Request::MalformedRequestException);

    // wrong header
    ASSERT_THROW(http::Request("GET / HTTP/1.1\r\nHost:\r\nContent-Length: "
                               "15\r\n\r\nThis is a body"),
                 http::Request::MalformedRequestException);
    // wrong body
    ASSERT_THROW(
        http::Request("GET / HTTP/1.1\r\nHost:x\r\nContent-Length: 15\r\n\r\n"),
        http::Request::MalformedRequestException);
}

TEST(testRequest, testComplexRequestString) {
    http::Request req(
        "GET /cgi-bin/myscript.cgi/path/to/script?param1=value1&param2=value2 "
        "HTTP/1.1\r\nHost: example.com\r\nUser-Agent: Mozilla/5.0 (Windows NT "
        "10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/96.0.4664.45 Safari/537.36\r\nAccept: "
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
        "avif,image/webp,image/apng,*/*;q=0.8,application/"
        "signed-exchange;v=b3;q=0.9\r\nAccept-Encoding: gzip, deflate, "
        "br\r\nAccept-Language: en-US,en;q=0.9\r\nConnection: "
        "keep-alive\r\n\r\n");

    EXPECT_EQ(req.getMethod(), http::MethodType::GET);
    EXPECT_EQ(req.getVersion(), http::Version::HTTP_1_1);
    EXPECT_EQ(req.getHeader("Host"), "example.com");
    EXPECT_EQ(req.getHeader("User-Agent"),
              "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
              "(KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36");
    EXPECT_EQ(req.getHeader("Accept"),
              "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
              "avif,image/"
              "webp,image/apng,*/*;q=0.8,application/"
              "signed-exchange;v=b3;q=0.9");
    EXPECT_EQ(req.getHeader("Accept-Encoding"), "gzip, deflate, br");
    EXPECT_EQ(req.getHeader("Accept-Language"), "en-US,en;q=0.9");
    EXPECT_EQ(req.getHeader("Connection"), "keep-alive");
    EXPECT_EQ(req.getBody(), "");
}

TEST(testRequest, testUriGetters) {
    std::string req = "GET /cgi-bin/python/me.py HTTP/1.1\r\nHost: x\r\n\r\n ";
    http::Request request(req);
    EXPECT_TRUE(request.getScheme().empty());
    EXPECT_TRUE(request.getHost().empty()) << request.getHost();
    EXPECT_TRUE(request.getPort().empty());
    EXPECT_EQ(request.getPath(), "/cgi-bin/python/me.py");
    EXPECT_TRUE(request.getQuery().empty());
}

TEST(testGetRequest, getOneRequest) {
    std::string req1 =
        "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nHost: "
        "www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";

    ASSERT_EQ(http::getRequest(req1), req1);
    // checking if there's any request left
    ASSERT_EQ(http::getRequest(), "");
    ASSERT_EQ(http::getRequest(req1), req1);
    // checking if there's any request left
    ASSERT_EQ(http::getRequest(), "");
}

TEST(testGetRequest, getMoreThanOneRequest) {

    std::string req1 =
        "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nHost: "
        "www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\r\nConnection: Keep-Alive\r\n\r\n";

    std::string req2 =
        "GET /hello.htm HTTP/1.0\r\nUser-Agent: Mozilla/4.0 (compatible; "
        "MSIE5.01; Windows NT)\r\nAccept-Language: en-us\r\nAccept-Encoding: "
        "gzip, deflate\nConnection: Keep-Alive\r\n\r\n";

    // receiving req1 - req2 stays in buffer
    ASSERT_EQ(http::getRequest(req1 + req2), req1);
    // receiving req2 - buffer is empty
    ASSERT_EQ(http::getRequest(), req2);
    // checking if there's any request left
    ASSERT_EQ(http::getRequest(), "");
    // receiving req1 - req2 stays in buffer
    ASSERT_EQ(http::getRequest(req1 + req2), req1);
    // receiving req2 - buffer is empty
    ASSERT_EQ(http::getRequest(), req2);
    // checking if there's any request left
    ASSERT_EQ(http::getRequest(), "");
}

TEST(testGetRequest, getRequestWithError) {
    std::string req3 = "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: "
                       "Mozilla/4.0 (compatible; MSIE5.01; Windows "
                       "NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: "
                       "text/xml; charset=utf-8\r\n";

    // receiving req1 - req2 is complete and stays in buffer, req3 is incomplete
    // and also stays in buffer
    ASSERT_EQ(http::getRequest(req3), req3);
    ASSERT_EQ(http::getRequest(), "");
}
