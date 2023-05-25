#include "utils/Logger.hpp"
#include "utils/smt.hpp"

#include <gtest/gtest.h>
#include <sstream>

TEST(testLogger, testConstructor) {
    Logger& logger = Logger::getInstance();

    EXPECT_EQ(logger.getLevel(), LogLevel::Debug);
}

TEST(testLogger, testSetLevel) {
    Logger& logger = Logger::getInstance();

    ASSERT_THROW(logger.setFile("path/does/not/exist.log"),
                 Logger::InvalidFileException);
    ASSERT_NO_THROW(logger.setFile("tests/logs/test.log"));
}

TEST(testLogger, testLog) {
    Logger& logger = Logger::getInstance();

    *(logger.log(LogLevel::Debug, __FILE__, __LINE__))
        << "This is a debug message";
    *(logger.log(LogLevel::Info, __FILE__, __LINE__))
        << "This is an info message";
    *(logger.log(LogLevel::Warning, __FILE__, __LINE__))
        << "This is a warning message";
    *(logger.log(LogLevel::Error, __FILE__, __LINE__))
        << "This is an error message";
    *(logger.log(LogLevel::Fatal, __FILE__, __LINE__))
        << "This is a fatal message";
}

TEST(testLogger, testFLog) {

    Logger& logger = Logger::getInstance();
    logger.setFile("tests/logs/test.log");

    *(logger.flog(LogLevel::Debug, __FILE__, __LINE__))
        << "This is a debug message";
    *(logger.flog(LogLevel::Info, __FILE__, __LINE__))
        << "This is an info message";
    *(logger.flog(LogLevel::Warning, __FILE__, __LINE__))
        << "This is a warning message";
    *(logger.flog(LogLevel::Error, __FILE__, __LINE__))
        << "This is an error message";
    *(logger.flog(LogLevel::Fatal, __FILE__, __LINE__))
        << "This is a fatal message";
}

TEST(testLogger, testLogLevels) {
    Logger& logger = Logger::getInstance();
    EXPECT_EQ(logger.getLevel(), LogLevel::Debug);

    logger.setLevel(LogLevel::Info);
    EXPECT_EQ(logger.getLevel(), LogLevel::Info);

    logger.setLevel(LogLevel::Warning);
    EXPECT_EQ(logger.getLevel(), LogLevel::Warning);

    logger.setLevel(LogLevel::Error);
    EXPECT_EQ(logger.getLevel(), LogLevel::Error);

    logger.setLevel(LogLevel::Fatal);
    EXPECT_EQ(logger.getLevel(), LogLevel::Fatal);

    logger.setLevel(LogLevel::Debug);
    EXPECT_EQ(logger.getLevel(), LogLevel::Debug);
}

TEST(testLogger, testMacros) {
    LOG_D << "This is a debug message";
    LOG_I << "This is an info message";
    LOG_W << "This is a warning message";
    LOG_E << "This is an error message";
    LOG_F << "This is a fatal message";

    FLOG_D << "This is a debug message";
    FLOG_I << "This is an info message";
    FLOG_W << "This is a warning message";
    FLOG_E << "This is an error message";
    FLOG_F << "This is a fatal message";

    LOG_LVL(LogLevel::Fatal);
    LOG_D << "This message should not appear";
}

TEST(testLog, testConstructor) {
    Log* log = new Log(std::cout, LogLevel::Info, "test.cpp", 42);
    (*log) << "Log message";
    delete log;
}

TEST(testLog, testLoggerWithSmt) {
    *(smt::make_shared(new Log(std::cout, LogLevel::Info, "test.cpp", 42)))
        << "Testing with smts";

    Log log(std::cout, LogLevel::Info, "test.cpp", 42);
    log << "supposed to happen after";
    std::cout << "Supposed to happen before the log" << std::endl;
}

TEST(testLog, testDebug) {
    std::ostringstream oss;

    Log* log = new Log(oss, LogLevel::Debug, "test.cpp", 42);
    (*log) << "Log message";
    delete log;

    EXPECT_NE(oss.str().find("DEBUG"), std::string::npos);
    EXPECT_NE(oss.str().find("test.cpp"), std::string::npos);
    EXPECT_NE(oss.str().find("Log message"), std::string::npos);
    EXPECT_NE(oss.str().find("42"), std::string::npos);
}

TEST(testLog, testInfo) {
    std::ostringstream oss;

    Log* log = new Log(oss, LogLevel::Info, "test.cpp", 42);
    (*log) << "Log message";
    delete log;

    EXPECT_NE(oss.str().find("INFO"), std::string::npos);
    EXPECT_NE(oss.str().find("test.cpp"), std::string::npos);
    EXPECT_NE(oss.str().find("Log message"), std::string::npos);
    EXPECT_NE(oss.str().find("42"), std::string::npos);
}

TEST(testLog, testWarning) {
    std::ostringstream oss;

    Log* log = new Log(oss, LogLevel::Warning, "test.cpp", 42);
    (*log) << "Log message";
    delete log;

    EXPECT_NE(oss.str().find("WARNING"), std::string::npos);
    EXPECT_NE(oss.str().find("test.cpp"), std::string::npos);
    EXPECT_NE(oss.str().find("Log message"), std::string::npos);
    EXPECT_NE(oss.str().find("42"), std::string::npos);
}

TEST(testLog, testError) {
    std::ostringstream oss;

    Log* log = new Log(oss, LogLevel::Error, "test.cpp", 42);
    (*log) << "Log message";
    delete log;

    EXPECT_NE(oss.str().find("ERROR"), std::string::npos);
    EXPECT_NE(oss.str().find("test.cpp"), std::string::npos);
    EXPECT_NE(oss.str().find("Log message"), std::string::npos);
    EXPECT_NE(oss.str().find("42"), std::string::npos);
}

TEST(testLog, testFatal) {
    std::ostringstream oss;

    Log* log = new Log(oss, LogLevel::Fatal, "test.cpp", 42);
    (*log) << "Log message";
    delete log;

    EXPECT_NE(oss.str().find("FATAL"), std::string::npos);
    EXPECT_NE(oss.str().find("test.cpp"), std::string::npos);
    EXPECT_NE(oss.str().find("Log message"), std::string::npos);
    EXPECT_NE(oss.str().find("42"), std::string::npos);
}
