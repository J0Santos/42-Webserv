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

TEST(testRootDirective, testIsValidAndParse) {
    system("mkdir -p /tmp/testRootDirective");
    config::DirectiveTypeTraits<config::Root> directive;
    ASSERT_TRUE(directive.isBlockDirective());
    ASSERT_TRUE(directive.isRouteDirective());

    directive.parse({"root"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"root", "invalid/directory"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"other", "invalid/directory"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"root", "/tmp/testRootDirective", "invalid"});
    ASSERT_FALSE(directive.isValid());

    {
        config::DirectiveTypeTraits<config::Root> directive;
        directive.parse({"root", "/tmp/testRootDirective"});
        ASSERT_TRUE(directive.isValid());
    }
    system("rm -rf /tmp/testRootDirective");
}

TEST(testErrorPageDirective, testIsValidAndParse) {
    system("touch /tmp/testErrorPageDirective");
    config::DirectiveTypeTraits<config::ErrorPage> directive;
    ASSERT_TRUE(directive.isBlockDirective());
    ASSERT_TRUE(directive.isRouteDirective());

    directive.parse({"error_page"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"error_page", "400"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"other", "400", "/tmp/testRootDirective"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"error_page", "/tmp/testRootDirective", "400"});
    ASSERT_FALSE(directive.isValid());

    {
        config::DirectiveTypeTraits<config::ErrorPage> directive;
        directive.parse({"error_page", "404", "/tmp/testErrorPageDirective"});
        ASSERT_TRUE(directive.isValid());
    }
    {
        config::DirectiveTypeTraits<config::ErrorPage> directive;
        directive.parse(
            {"error_page", "404", "405", "/tmp/testErrorPageDirective"});
        ASSERT_TRUE(directive.isValid());
    }
    system("rm -f /tmp/testErrorPageDirective");
}

TEST(testMaxBodySizeDirective, testIsValidAndParse) {
    config::DirectiveTypeTraits<config::MaxBodySize> directive;
    ASSERT_TRUE(directive.isBlockDirective());
    ASSERT_TRUE(directive.isRouteDirective());

    directive.parse({"client_max_body_size"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"client_max_body_size", "400", "other"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"other", "400"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"client_max_body_size", "invalid"});
    ASSERT_FALSE(directive.isValid());

    {
        config::DirectiveTypeTraits<config::MaxBodySize> directive;
        directive.parse({"client_max_body_size", "8080808080"});
        ASSERT_TRUE(directive.isValid());
    }
}

TEST(testAllowMethodDirective, testIsValidAndParse) {
    config::DirectiveTypeTraits<config::AllowMethods> directive;
    ASSERT_TRUE(directive.isBlockDirective());
    ASSERT_TRUE(directive.isRouteDirective());

    directive.parse({"allow_methods"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"other", "GET"});
    ASSERT_FALSE(directive.isValid());

    directive.parse({"allow_methods", "invalid"});
    ASSERT_FALSE(directive.isValid());
    {
        config::DirectiveTypeTraits<config::AllowMethods> directive;
        directive.parse({"allow_methods", "GET"});
        ASSERT_TRUE(directive.isValid());
    }
    {
        config::DirectiveTypeTraits<config::AllowMethods> directive;
        directive.parse({"allow_methods", "GET", "POST", "DELETE"});
        ASSERT_TRUE(directive.isValid());
    }
}