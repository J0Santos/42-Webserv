#include "cgi/Cgi.hpp"

#include <gtest/gtest.h>

TEST(testCgiTypes, testConvertType) {
    EXPECT_EQ(cgi::convertCgiExtension(".py"), cgi::Py);
    EXPECT_EQ(cgi::convertCgiExtension(".php"), cgi::Php);
    EXPECT_EQ(cgi::convertCgiExtension(".cgi"), cgi::Cgi);
    EXPECT_EQ(cgi::convertCgiExtension(".other"), cgi::Unknown);
    EXPECT_EQ(cgi::convertCgiExtension("other"), cgi::Unknown);
}