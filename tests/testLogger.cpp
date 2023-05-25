#include "utils/Logger.hpp"

#include <gtest/gtest.h>

TEST(testLogger, getInstance) {
    LOG_D("Testing logger with numbers " << 09.2 << " and booleans: " << true);
}

TEST(testLogger, macros) {
    LOG_D("this is a test");
    LOG_I("this is a test");
    LOG_W("this is a test");
    LOG_E("this is a test");
    LOG_F("this is a test");
}