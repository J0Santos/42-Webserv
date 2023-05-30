#include "cgi/Cgi.hpp"

#include "utils/Logger.hpp"

namespace cgi {

CgiHandler::CgiHandler(char* path, char** argv, char** envp,
                       std::string const& body)
    : m_path(path), m_argv(argv), m_envp(envp), m_body(body) {}

CgiHandler::~CgiHandler(void) {
    for (int i = 0; m_argv[i]; i++) { delete[] m_argv[i]; }
    delete[] m_argv;

    for (int i = 0; m_envp[i]; i++) { delete[] m_envp[i]; }
    delete[] m_envp;

    delete[] m_path;
}

std::string CgiHandler::run(void) const {
    // verify if script exists and binary exists
    return (runAsChildProcess());
}

std::string CgiHandler::runAsChildProcess(void) const {
    std::string resp;

    FILE* input = tmpfile();
    int   input_fd = fileno(input);

    FILE* output = tmpfile();
    int   output_fd = fileno(output);

    int stdin_reference = dup(STDIN_FILENO);
    int stdout_reference = dup(STDOUT_FILENO);

    if (!input || !output || stdin_reference < 0) {
        LOG_E("Failed to create temporary files.");
        return (resp);
    }
    if (input_fd < 0) {
        LOG_E("Failed to get temporary infile fd.");
        return (resp);
    }
    // check if request type is POST
    if (getMethod() == "POST") {
        write(input_fd, m_body.c_str(), m_body.size());
        rewind(input);
    }
    pid_t pid = fork();
    if (pid < 0) {
        LOG_E("Failed to spawn child process.");
        return (resp);
    }
    else if (pid == 0) {
        // Direct I/O to temporary file;
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        execve(m_path, m_argv, m_envp);
        perror("execve() failed");

        // Child process
        LOG_E("Failed call to execve in child process.");
        exit(500);
    }
    // From here on out we are always in parent cause child either executed or
    // exited
    int status;

    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        int  bytesRead = 0;
        char buf[2049];
        lseek(output_fd, 0, SEEK_SET);
        while ((bytesRead = read(output_fd, buf, 2048)) > 0) {
            buf[bytesRead] = '\0';
        }
        std::string body(buf);
        resp = body;
    }

    // Reset STDIN and STDOUT
    dup2(stdin_reference, STDIN_FILENO);
    dup2(stdout_reference, STDOUT_FILENO);

    // Close temporary files
    fclose(input);
    fclose(output);

    return (resp);
}

std::string CgiHandler::getMethod(void) const {
    std::string method = "GET";
    for (int i = 0; m_envp[i]; i++) {
        if (strncmp(m_envp[i], "REQUEST_METHOD=", 15) == 0) {
            method = m_envp[i] + 15;
            break;
        }
    }
    return (method);
}

std::vector<std::string> splitInfoFromPath(std::string const& path) {

    std::vector<std::string> subs(2);

    size_t pos = path.find(".");
    while (pos != std::string::npos) {
        std::string ext = path.substr(pos);
        ext = (ext.find("/") != std::string::npos)
                  ? ext.substr(0, ext.find("/"))
                  : ext;
        if (cgi::convertCgiExtension(ext) != cgi::Unknown) {
            subs[0] = path.substr(0, pos + ext.size());
            subs[1] = path.substr(pos + ext.size());
            break;
        }
        pos = path.find(".", pos + 1);
    }

    return (subs);
}

CgiType convertCgiExtension(std::string const& cgiExtension) {
    if (cgiExtension == ".py") { return (Py); }
    if (cgiExtension == ".php") { return (Php); }
    if (cgiExtension == ".cgi") { return (Cgi); }
    return (Unknown);
}

} // namespace cgi
