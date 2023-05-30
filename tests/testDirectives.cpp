#include "config/directives.hpp"

#include <gtest/gtest.h>

TEST(testListenDirective, testIsValid) {
    {
        config::DirectiveTypeTraits<config::Listen> directive;
        directive.parse({"listen", "443"});
        ASSERT_TRUE(directive.isValid());
    }
    {
        config::DirectiveTypeTraits<config::Listen> directive;
        directive.parse({"listen", "example"});
        EXPECT_TRUE(directive.isValid());
    }
    {
        config::DirectiveTypeTraits<config::Listen> directive;
        directive.parse({"listen", "443:example"});
        EXPECT_TRUE(directive.isValid());
    }
    {
        config::DirectiveTypeTraits<config::Listen> directive;
        directive.parse({"other", "443:example"});
        EXPECT_FALSE(directive.isValid());
    }
    {
        config::DirectiveTypeTraits<config::Listen> directive;
        directive.parse({"listen", "example:443:"});
        EXPECT_FALSE(directive.isValid());
    }
    {
        config::DirectiveTypeTraits<config::Listen> directive;
        directive.parse({"listen", ":example"});
        EXPECT_FALSE(directive.isValid());
    }
    {
        config::DirectiveTypeTraits<config::Listen> directive;
        directive.parse({"listen", "example", "443"});
        EXPECT_FALSE(directive.isValid());
    }
}

TEST(testListenDirective, testIsBlockAndIsRouteDirective) {
    config::DirectiveTypeTraits<config::Listen> directive;
    ASSERT_TRUE(directive.isBlockDirective());
    ASSERT_FALSE(directive.isRouteDirective());
}