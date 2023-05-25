#include "utils/Logger.hpp"

Logger::Logger(std::string filename) {
    // if (filename.empty()) { filename = "logs/webserver.log"; }
    if (!filename.empty()) {
        m_file.open(filename.c_str());
        if (!m_file.good()) { throw InvalidFileException(); }
    }
}

Logger::~Logger(void) { m_file.close(); }

std::ostream& Logger::getInstance(std::string filename) {

    static Logger logger(filename);
    if (logger.m_file.good()) { return (logger.m_file); }
    else { return (std::cout); }
    return (logger.m_file);
}

std::string Logger::getFilename(std::string filename) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) { return (filename); }

    std::string fullPath = filename;
    std::string currentDir(cwd);

    std::size_t pos = fullPath.find(currentDir);
    if (!pos) { return (fullPath.substr(currentDir.length() + 1)); }
    return (fullPath);
}

char const* Logger::InvalidFileException::what(void) const throw() {
    return ("Invalid File Exception: failed to open logger file.");
}