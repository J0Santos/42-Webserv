#include "config/directives.hpp"

#include <gtest/gtest.h>

TEST(testListenDirective, testIsValid) {
    EXPECT_TRUE(directive::Listen({"listen", "443"}).isValid());
    EXPECT_TRUE(directive::Listen({"listen", "example"}).isValid());
    EXPECT_TRUE(directive::Listen({"listen", "443:example"}).isValid());

    EXPECT_FALSE(directive::Listen({"other", "443:example"}).isValid());
    EXPECT_FALSE(directive::Listen({"listen", "example:443:"}).isValid());
    EXPECT_FALSE(directive::Listen({"listen", ":example"}).isValid());
    EXPECT_FALSE(directive::Listen({"listen", "example", "443"}).isValid());
}

TEST(testListenDirective, testExtract) {}