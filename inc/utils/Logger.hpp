#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "utils/smt.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace Color {

const std::string Reset = "\033[0m";
const std::string Red = "\033[31m";
const std::string Green = "\033[32m";
const std::string Yellow = "\033[33m";
const std::string Blue = "\033[34m";
const std::string Magenta = "\033[35m";

} // namespace Color

namespace LogLevel {

enum level_t { Debug, Info, Warning, Error, Fatal, None };

}

class Log {
    public:

        Log(std::ostream& os, LogLevel::level_t level = LogLevel::Debug,
            std::string filename = __FILE__, int line = __LINE__);
        ~Log(void);

        template<typename T>
        Log& operator<<(T const& value) {
            if (m_lvl == LogLevel::None) { return (*this); }
            m_buf << value;
            return (*this);
        }

    private:

        std::string const debug(void) const;
        std::string const info(void) const;
        std::string const warning(void) const;
        std::string const error(void) const;
        std::string const fatal(void) const;

        LogLevel::level_t  m_lvl;
        std::ostream&      m_out;
        std::string        m_file;
        int                m_line;
        std::ostringstream m_buf;
};

class Logger {
    public:

        static Logger& getInstance(void);

        int  getLevel(void) const;
        void setLevel(LogLevel::level_t level);
        void setFile(std::string filename);

        smt::shared_ptr<Log> log(LogLevel::level_t lvl, std::string filename,
                                 int line);

        smt::shared_ptr<Log> flog(LogLevel::level_t lvl, std::string filename,
                                  int line);

    private:

        Logger(LogLevel::level_t lvl = LogLevel::Debug);
        ~Logger(void);

        LogLevel::level_t m_lvl;
        bool              m_ffile;
        std::ofstream     m_file;
};

#define LOG_LVL(L) Logger::getInstance().setLevel(L)

#define LOG_D *(Logger::getInstance().log(LogLevel::Debug, __FILE__, __LINE__))
#define LOG_I *(Logger::getInstance().log(LogLevel::Info, __FILE__, __LINE__))
#define LOG_W                                                                  \
    *(Logger::getInstance().log(LogLevel::Warning, __FILE__, __LINE__))
#define LOG_E *(Logger::getInstance().log(LogLevel::Error, __FILE__, __LINE__))
#define LOG_F *(Logger::getInstance().log(LogLevel::Fatal, __FILE__, __LINE__))

#define FLOG_D                                                                 \
    *(Logger::getInstance().flog(LogLevel::Debug, __FILE__, __LINE__))
#define FLOG_I *(Logger::getInstance().flog(LogLevel::Info, __FILE__, __LINE__))
#define FLOG_W                                                                 \
    *(Logger::getInstance().flog(LogLevel::Warning, __FILE__, __LINE__))
#define FLOG_E                                                                 \
    *(Logger::getInstance().flog(LogLevel::Error, __FILE__, __LINE__))
#define FLOG_F                                                                 \
    *(Logger::getInstance().flog(LogLevel::Fatal, __FILE__, __LINE__))

#endif /* LOGGER_HPP */
