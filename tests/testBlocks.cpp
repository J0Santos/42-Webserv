#include "config/blocks/block.hpp"

#include <gtest/gtest.h>

TEST(testBlock, testConstructor) {
    config::block b1;
    config::block b2(b1);
    config::block b3;
    b3 = b1;

    EXPECT_EQ(b1.m_port, "");
    EXPECT_EQ(b1.m_host, "");
    EXPECT_EQ(b1.m_server_name, "");
    EXPECT_EQ(b1.m_root, "");
    EXPECT_EQ(b1.m_error_pages.size(), 0);
    EXPECT_EQ(b1.m_max_body_size, 0);
    EXPECT_EQ(b1.m_allowed_methods.size(), 0);
    EXPECT_EQ(b1.m_location.size(), 0);

    EXPECT_EQ(b1, b2);
    EXPECT_EQ(b1, b3);
}

TEST(testRoute, testConstructor) {
    config::route r1;
    config::route r2(r1);
    config::route r3;
    r3 = r1;

    EXPECT_EQ(r1.m_target, "/");
    EXPECT_EQ(r1.m_root, "");
    EXPECT_EQ(r1.m_error_pages.size(), 0);
    EXPECT_EQ(r1.m_max_body_size, 0);
    EXPECT_EQ(r1.m_allowed_methods.size(), 0);
    EXPECT_EQ(r1.m_autoindex, false);
    EXPECT_EQ(r1.m_index, "");
    EXPECT_EQ(r1.m_cgi_extension, "");

    EXPECT_EQ(r1, r2);
    EXPECT_EQ(r1, r3);
}