#include "config/directives.hpp"

#include <gtest/gtest.h>

TEST(testBlockDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::Block> blockDirective({});
    EXPECT_FALSE(blockDirective.isBlockDirective());
    EXPECT_FALSE(blockDirective.isRouteDirective());
}

TEST(testBlockDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Block>({"server", "}"}).isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Block>({"server"}).isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Block>({"other", "{"}).isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::Block>({"server", "{"}).isValid());
}

TEST(testRouteDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::Route> blockDirective({});
    EXPECT_TRUE(blockDirective.isBlockDirective());
    EXPECT_FALSE(blockDirective.isRouteDirective());
}

TEST(testRouteDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Route>({"location", "/", "}"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Route>({"location"}).isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::Route>({"location", "{"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::Route>({"other", "/", "{"})
                     .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::Route>({"location", "/", "{"})
            .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::Route>(
                    {"location", "/python/", "{"})
                    .isValid());
}

TEST(testListenDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::Listen> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_FALSE(directive.isRouteDirective());
}

TEST(testListenDirective, testIsValid) {

    EXPECT_TRUE(config::DirectiveTypeTraits<config::Listen>({"listen", "443"})
                    .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::Listen>({"listen", "example"})
            .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::Listen>({"listen", "443:example"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Listen>({"other", "443:example"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Listen>({"listen", "example:443:"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Listen>({"listen", ":example"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::Listen>(
                     {"listen", "example", "443"})
                     .isValid());
}

TEST(testServerNameDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::ServerName> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_FALSE(directive.isRouteDirective());
}

TEST(testServerNameDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::ServerName>({"server_name"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::ServerName>({"other", "domain.com"})
            .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::ServerName>(
                    {"server_name", "domain.com"})
                    .isValid());
}

TEST(testRootDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::Root> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testRootDirective, testIsValid) {
    system("mkdir -p /tmp/testRootDirective");
    EXPECT_FALSE(config::DirectiveTypeTraits<config::Root>({"root"}).isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Root>({"root", "invalid/directory"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::Root>(
                     {"other", "invalid/directory"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::Root>(
                     {"root", "/tmp/testRootDirective", "invalid"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::Root>(
                    {"root", "/tmp/testRootDirective"})
                    .isValid());
    system("rm -rf /tmp/testRootDirective");
}

TEST(testErrorPageDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::ErrorPage> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testErrorPageDirective, testIsValid) {
    system("touch /tmp/testErrorPageDirective");

    EXPECT_FALSE(config::DirectiveTypeTraits<config::ErrorPage>({"error_page"})
                     .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::ErrorPage>({"error_page", "400"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::ErrorPage>(
                     {"other", "400", "/tmp/testRootDirective"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::ErrorPage>(
                     {"error_page", "/tmp/testRootDirective", "400"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::ErrorPage>(
                    {"error_page", "404", "/tmp/testErrorPageDirective"})
                    .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::ErrorPage>(
                    {"error_page", "404", "405", "/tmp/testErrorPageDirective"})
                    .isValid());
    system("rm -f /tmp/testErrorPageDirective");
}

TEST(testMaxBodySizeDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::MaxBodySize> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testMaxBodySizeDirective, testIsValid) {
    EXPECT_FALSE(config::DirectiveTypeTraits<config::MaxBodySize>(
                     {"client_max_body_size"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::MaxBodySize>(
                     {"client_max_body_size", "400", "other"})
                     .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::MaxBodySize>({"other", "400"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::MaxBodySize>(
                     {"client_max_body_size", "invalid"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::MaxBodySize>(
                    {"client_max_body_size", "8080808080"})
                    .isValid());
}

TEST(testAllowMethodDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::AllowMethods> directive({});
    EXPECT_TRUE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testAllowMethodDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::AllowMethods>({"allow_methods"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::AllowMethods>({"other", "GET"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::AllowMethods>(
                     {"allow_methods", "invalid"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::AllowMethods>(
                    {"allow_methods", "GET"})
                    .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::AllowMethods>(
                    {"allow_methods", "GET", "POST", "DELETE"})
                    .isValid());
}

TEST(testIndexDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::Index> directive({});
    EXPECT_FALSE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testIndexDirective, testIsValid) {
    system("touch /tmp/testIndexDirective");

    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Index>({"index"}).isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::Index>(
                     {"other", "/tmp/testIndexDirective"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::Index>(
                     {"index", "/tmp/testIndexDirective", "other"})
                     .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::Index>({"index", "invalid/file"})
            .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::Index>(
                    {"index", "/tmp/testIndexDirective"})
                    .isValid());

    system("rm -f /tmp/testIndexDirective");
}

TEST(testAutoIndexDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::AutoIndex> directive({});
    EXPECT_FALSE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testAutoIndexDirective, testIsValid) {

    EXPECT_FALSE(config::DirectiveTypeTraits<config::AutoIndex>({"autoindex"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::AutoIndex>({"other", "on"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::AutoIndex>(
                     {"autoindex", "on", "other"})
                     .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::AutoIndex>({"autoindex", "invalid"})
            .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::AutoIndex>({"autoindex", "on"})
            .isValid());
    EXPECT_TRUE(
        config::DirectiveTypeTraits<config::AutoIndex>({"autoindex", "off"})
            .isValid());
}

TEST(testCgiExtensionDirective, testIsDirectiveOrRoute) {
    config::DirectiveTypeTraits<config::CgiExtension> directive({});
    EXPECT_FALSE(directive.isBlockDirective());
    EXPECT_TRUE(directive.isRouteDirective());
}

TEST(testCgiExtensionDirective, testIsValid) {
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::CgiExtension>({"fastcgi_pass"})
            .isValid());
    EXPECT_FALSE(
        config::DirectiveTypeTraits<config::CgiExtension>({"other", ".py"})
            .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::CgiExtension>(
                     {"fastcgi_pass", ".py", "other"})
                     .isValid());
    EXPECT_FALSE(config::DirectiveTypeTraits<config::CgiExtension>(
                     {"fastcgi_pass", ".tmp"})
                     .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::CgiExtension>(
                    {"fastcgi_pass", ".py"})
                    .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::CgiExtension>(
                    {"fastcgi_pass", ".php"})
                    .isValid());
    EXPECT_TRUE(config::DirectiveTypeTraits<config::CgiExtension>(
                    {"fastcgi_pass", ".cgi"})
                    .isValid());
}