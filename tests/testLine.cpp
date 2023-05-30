#include "config/Line.hpp"

#include <gtest/gtest.h>

TEST(testLine, testConstructorAndCopy) {
    config::Line line1("");
    config::Line line2(line1);
    config::Line line3("test");

    line3 = line2;

    EXPECT_EQ(line1.getLine(), "");
    EXPECT_EQ(line2.getLine(), "");
    EXPECT_EQ(line3.getLine(), "");
    EXPECT_EQ(line1.getType(), config::LineType::Empty);
    EXPECT_EQ(line2.getType(), config::LineType::Empty);
    EXPECT_EQ(line3.getType(), config::LineType::Empty);
}

TEST(testLine, testTypeAssertion) {
    EXPECT_EQ(config::Line("").getType(), config::LineType::Empty);
    EXPECT_EQ(config::Line("# comment").getType(), config::LineType::Empty);
    EXPECT_EQ(config::Line("server {").getType(), config::LineType::Block);
    EXPECT_EQ(config::Line("location {").getType(), config::LineType::Route);
    EXPECT_EQ(config::Line("}").getType(), config::LineType::End);
    EXPECT_EQ(config::Line("listen 8080;").getType(), config::LineType::Listen);
    EXPECT_EQ(config::Line("just some random text").getType(),
              config::LineType::Unknown);
}

TEST(testLine, testFormat) {

    EXPECT_EQ(config::Line("").format(), std::vector<std::string>());
    EXPECT_EQ(config::Line("# comment line").format(),
              std::vector<std::string>());
    EXPECT_EQ(config::Line("random text").format(), std::vector<std::string>());

    EXPECT_EQ(config::Line("server {").format(),
              std::vector<std::string>({"server", "{"}));
    EXPECT_EQ(config::Line("server { # comment").format(),
              std::vector<std::string>({"server", "{"}));

    EXPECT_EQ(config::Line("location {").format(),
              std::vector<std::string>({"location", "{"}));
    EXPECT_EQ(config::Line("location { # comment").format(),
              std::vector<std::string>({"location", "{"}));

    EXPECT_EQ(config::Line("}").format(), std::vector<std::string>({"}"}));
    EXPECT_EQ(config::Line("} # comment").format(),
              std::vector<std::string>({"}"}));

    EXPECT_EQ(config::Line("listen 8080;").format(),
              std::vector<std::string>({"listen", "8080"}));
    EXPECT_EQ(config::Line("listen 8080; # comment").format(),
              std::vector<std::string>({"listen", "8080"}));
}