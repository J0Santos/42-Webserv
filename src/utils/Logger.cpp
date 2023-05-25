#include "utils/Logger.hpp"

/* Logger */
Logger::Logger(LogLevel::level_t lvl) : m_lvl(lvl), m_ffile(false) {}

Logger::~Logger() {
    if (m_ffile) { m_file.close(); }
}

Logger& Logger::getInstance(void) {
    static Logger ist;
    return (ist);
}

void Logger::setLevel(LogLevel::level_t level) { m_lvl = level; }

int Logger::getLevel(void) const { return (m_lvl); }

void Logger::setFile(std::string filename) {
    m_file.open(filename.c_str());
    if (!m_file.good()) { throw std::exception(); }
    m_ffile = true;
}

smt::shared_ptr<Log> Logger::log(LogLevel::level_t lvl, std::string filename,
                                 int line) {
    if (lvl < m_lvl) {
        return (smt::make_shared(new Log(std::cout, LogLevel::None, "", 0)));
    }
    return (smt::make_shared(new Log(std::cout, lvl, filename, line)));
}

smt::shared_ptr<Log> Logger::flog(LogLevel::level_t lvl, std::string filename,
                                  int line) {
    if (lvl < m_lvl) {
        return (smt::make_shared(new Log(m_file, LogLevel::None, "", 0)));
    }
    return (smt::make_shared(new Log(m_file, lvl, filename, line)));
}

/* Log */
Log::Log(std::ostream& os, LogLevel::level_t level, std::string filename,
         int line)
    : m_lvl(level), m_out(os), m_file(filename), m_line(line) {
    switch (m_lvl) {
        case LogLevel::Debug: m_buf << debug(); break;
        case LogLevel::Info: m_buf << info(); break;
        case LogLevel::Warning: m_buf << warning(); break;
        case LogLevel::Error: m_buf << error(); break;
        case LogLevel::Fatal: m_buf << fatal(); break;
        default: break;
    }
}

Log::~Log() {
    if (m_lvl != LogLevel::None) { m_out << m_buf.str() << std::endl; }
}

std::string const Log::debug() const {
    std::ostringstream oss;
    oss << "[" << Color::Blue << "DEBUG" << Color::Reset << "]: (" << m_file
        << ":" << m_line << "): ";
    return oss.str();
}

std::string const Log::info() const {
    std::ostringstream oss;
    oss << "[" << Color::Green << "INFO" << Color::Reset << "]: (" << m_file
        << ":" << m_line << "): ";
    return oss.str();
}

std::string const Log::warning() const {
    std::ostringstream oss;
    oss << "[" << Color::Yellow << "WARNING" << Color::Reset << "]: (" << m_file
        << ":" << m_line << "): ";
    return oss.str();
}

std::string const Log::error() const {
    std::ostringstream oss;
    oss << "[" << Color::Red << "ERROR" << Color::Reset << "]: (" << m_file
        << ":" << m_line << "): ";
    return oss.str();
}

std::string const Log::fatal() const {
    std::ostringstream oss;
    oss << "[" << Color::Magenta << "FATAL" << Color::Reset << "]: (" << m_file
        << ":" << m_line << "): ";
    return oss.str();
}
