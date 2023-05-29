#include "http/methods.hpp"

#include <gtest/gtest.h>

TEST(testConvert, testConvertMethod) {
    ASSERT_EQ(http::convertMethod("GET"), http::Method::GET);
    ASSERT_EQ(http::convertMethod("HEAD"), http::Method::HEAD);
    ASSERT_EQ(http::convertMethod("POST"), http::Method::POST);
    ASSERT_EQ(http::convertMethod("PUT"), http::Method::PUT);
    ASSERT_EQ(http::convertMethod("DELETE"), http::Method::DELETE);
    ASSERT_EQ(http::convertMethod("CONNECT"), http::Method::CONNECT);
    ASSERT_EQ(http::convertMethod("OPTIONS"), http::Method::OPTIONS);
    ASSERT_EQ(http::convertMethod("TRACE"), http::Method::TRACE);
    ASSERT_EQ(http::convertMethod("PATCH"), http::Method::PATCH);
    ASSERT_EQ(http::convertMethod("INVALID"), http::Method::INVALID);
    ASSERT_EQ(http::convertMethod("other"), http::Method::UNKNOWN_METHOD);
}

TEST(testConvert, testConvertVersion) {
    ASSERT_EQ(http::convertVersion("HTTP/1.1"), http::Version::HTTP_1_1);
    ASSERT_EQ(http::convertVersion("HTTP/1.0"), http::Version::UNKNOWN_VERSION);
    ASSERT_EQ(http::convertVersion("HTTP/2.0"), http::Version::UNKNOWN_VERSION);
}