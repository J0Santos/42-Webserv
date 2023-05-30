#include "cgi/Cgi.hpp"

#include <gtest/gtest.h>

TEST(testCgiTypes, testConvertType) {
    EXPECT_EQ(cgi::convertCgiExtension(".py"), cgi::Py);
    EXPECT_EQ(cgi::convertCgiExtension(".php"), cgi::Php);
    EXPECT_EQ(cgi::convertCgiExtension(".cgi"), cgi::Cgi);
    EXPECT_EQ(cgi::convertCgiExtension(".other"), cgi::Unknown);
    EXPECT_EQ(cgi::convertCgiExtension("other"), cgi::Unknown);
}

TEST(testSplitInfoFromPath, testAllCases) {

    EXPECT_EQ(
        cgi::splitInfoFromPath("/path/to/script.cgi/param1/param2"),
        std::vector<std::string>({"/path/to/script.cgi", "/param1/param2"}));

    EXPECT_EQ(
        cgi::splitInfoFromPath("/path/to/script/param1/param2.py"),
        std::vector<std::string>({"/path/to/script/param1/param2.py", ""}));

    EXPECT_EQ(cgi::splitInfoFromPath("/path/to/script.conf/param1/param2.py"),
              std::vector<std::string>(
                  {"/path/to/script.conf/param1/param2.py", ""}));

    EXPECT_EQ(
        cgi::splitInfoFromPath("/path/to/script.conf/param2.py/file/info"),
        std::vector<std::string>(
            {"/path/to/script.conf/param2.py", "/file/info"}));
}