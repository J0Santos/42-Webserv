#include "config/directives.hpp"

#include <gtest/gtest.h>

TEST(testBlockDirective, testIsValidAndParse) {
    config::DirectiveTypeTraits<config::Block> blockDirective;
    ASSERT_FALSE(blockDirective.isBlockDirective());
    ASSERT_FALSE(blockDirective.isRouteDirective());

    blockDirective.parse({"server", "}"});
    ASSERT_FALSE(blockDirective.isValid());

    blockDirective.parse({"server"});
    ASSERT_FALSE(blockDirective.isValid());

    blockDirective.parse({"other", "{"});
    ASSERT_FALSE(blockDirective.isValid());

    blockDirective.parse({"server", "{"});
    ASSERT_TRUE(blockDirective.isValid());
}

TEST(testRouteDirective, testIsValidAndParse) {
    config::DirectiveTypeTraits<config::Route> blockDirective;
    ASSERT_TRUE(blockDirective.isBlockDirective());
    ASSERT_FALSE(blockDirective.isRouteDirective());

    blockDirective.parse({"location", "/", "}"});
    ASSERT_FALSE(blockDirective.isValid());

    blockDirective.parse({"location"});
    ASSERT_FALSE(blockDirective.isValid());

    blockDirective.parse({"location", "{"});
    ASSERT_FALSE(blockDirective.isValid());

    blockDirective.parse({"other", "/", "{"});
    ASSERT_FALSE(blockDirective.isValid());
    {
        config::DirectiveTypeTraits<config::Route> blockDirective;
        blockDirective.parse({"location", "/", "{"});
        ASSERT_TRUE(blockDirective.isValid());
    }
    {
        config::DirectiveTypeTraits<config::Route> blockDirective;
        blockDirective.parse({"location", "/python/", "{"});
        ASSERT_TRUE(blockDirective.isValid());
    }
}

TEST(testListenDirective, testIsValidAndParse) {
    config::DirectiveTypeTraits<config::Listen> directive;
    ASSERT_TRUE(directive.isBlockDirective());
    ASSERT_FALSE(directive.isRouteDirective());

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

    directive.parse({"other", "443:example"});
    EXPECT_FALSE(directive.isValid());

    directive.parse({"listen", "example:443:"});
    EXPECT_FALSE(directive.isValid());

    directive.parse({"listen", ":example"});
    EXPECT_FALSE(directive.isValid());

    directive.parse({"listen", "example", "443"});
    EXPECT_FALSE(directive.isValid());
}

TEST(testServerNameDirective, testIsValidAndParse) {
    config::DirectiveTypeTraits<config::ServerName> directive;
    ASSERT_TRUE(directive.isBlockDirective());
    ASSERT_FALSE(directive.isRouteDirective());

    directive.parse({"server_name"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"other", "domain.com"});
    ASSERT_FALSE(directive.isValid());

    {
        config::DirectiveTypeTraits<config::ServerName> directive;
        directive.parse({"server_name", "domain.com"});
        ASSERT_TRUE(directive.isValid());
    }
}