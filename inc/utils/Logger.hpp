#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>

// TODO: add colors to logs
class Logger {

    public:

        static std::ostream& getInstance(std::string filename = "");
        static std::string   getFilename(std::string filename);

        struct InvalidFileException : std::exception {
                char const* what(void) const throw();
        };

    private:

        Logger(std::string filename);
        ~Logger(void);

        std::ofstream m_file;
};

#define LOGGER(F) Logger::getInstance(F)

#define LOG_D(M)                                                               \
    Logger::getInstance() << "[DEBUG]: " << Logger::getFilename(__FILE__)      \
                          << "[:" << __LINE__ << "]: " << M << std::endl

#define LOG_I(M)                                                               \
    Logger::getInstance() << "[INFO]: " << Logger::getFilename(__FILE__)       \
                          << "[:" << __LINE__ << "]: " << M << std::endl

#define LOG_W(M)                                                               \
    Logger::getInstance() << "[WARNING]: " << Logger::getFilename(__FILE__)    \
                          << "[:" << __LINE__ << "]: " << M << std::endl

#define LOG_E(M)                                                               \
    Logger::getInstance() << "[ERROR]: " << Logger::getFilename(__FILE__)      \
                          << "[:" << __LINE__ << "]: " << M << std::endl

#define LOG_F(M)                                                               \
    Logger::getInstance() << "[FATAL]: " << Logger::getFilename(__FILE__)      \
                          << "[:" << __LINE__ << "]: " << M << std::endl

#endif /* LOGGER_HPP */