#include "cgi/CgiHandler.hpp"

namespace cgi {

CgiHandler::CgiHandler(http::Request const& request, http::Route const& route)
    : m_request(request), m_envp(NULL), m_argv(NULL), m_path(NULL) {
    Envp     envp(request, route);
    ft::file script = envp.get("PATH_TRANSLATED");
    setPath(script.getDirectory());
    setEnvp(envp);
    setArgv(script.getDirectory(), script);
}

CgiHandler::~CgiHandler(void) {
    for (int i = 0; m_argv[i]; i++) { delete[] m_argv[i]; }
    delete[] m_argv;
    for (int i = 0; m_envp[i]; i++) { delete[] m_envp[i]; }
    delete[] m_envp;
    delete[] m_path;
}

std::pair<std::string, int> CgiHandler::run(void) {
    // validate path
    ft::directory path = getPath();
    if (!path.isValid()) {
        LOG_E("Invalid path: " << path);
        return (std::make_pair("", 404));
    }
    ft::file script = m_argv[1];
    if (!script.isValid()) {
        LOG_E("Invalid script: " << script);
        return (std::make_pair("", 404));
    }

    LOG_D("Executing CGI script");
    LOG_D("path: " << getPath());
    for (int i = 0; m_argv[i]; i++) {
        LOG_D("argv[" << i << "]: " << m_argv[i]);
    }
    for (int i = 0; m_envp[i]; i++) {
        LOG_D("envp[" << i << "]: " << m_envp[i]);
    }
    return (runAsChildProcess());
}

std::pair<std::string, int> CgiHandler::runAsChildProcess() {

    LOG_D("running child process");
    FILE* input = tmpfile();
    int   input_fd = fileno(input);

    FILE* output = tmpfile();
    int   output_fd = fileno(output);

    int stdin_reference = dup(STDIN_FILENO);
    int stdout_reference = dup(STDOUT_FILENO);

    if (!input || !output || stdin_reference < 0) {
        LOG_E("Failed to create tmp files");
    }
    if (input_fd < 0) { LOG_E("Failed to get temporary infile fd"); }
    // check if request type is POST
    if (m_request.getMethod() == "POST") {
        std::string req_content = m_request.getBody();
        write(input_fd, req_content.c_str(), req_content.size());
        rewind(input);
    }
    pid_t pid = fork();
    if (pid < 0) { LOG_E("Failed to spawn child process"); }
    else if (pid == 0) {
        // Direct I/O to temporary file;
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        execve(getPath(), getArgv(), getEnvp());
        perror("execve() failed");

        // Child process
        LOG_E("Failed call to execve in child process");
        exit(500);
    }
    // From here on out we are always in parent cause child either executed or
    // exited
    int                         status;
    std::pair<std::string, int> resp;

    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) { resp = std::make_pair("", 500); }
    else {
        int  bytesRead = 0;
        char buf[2049];
        lseek(output_fd, 0, SEEK_SET);
        while ((bytesRead = read(output_fd, buf, 2048)) > 0) {
            buf[bytesRead] = '\0';
        }
        std::string body(buf);
        resp = std::make_pair(body, 200);
    }

    // Reset STDIN and STDOUT
    dup2(stdin_reference, STDIN_FILENO);
    dup2(stdout_reference, STDOUT_FILENO);

    // Close temporary files
    fclose(input);
    fclose(output);

    return resp;
}

void CgiHandler::setPath(std::string const& path) {
    m_path = new char[path.size() + 1];
    strcpy(m_path, path.c_str());
}

void CgiHandler::setEnvp(Envp const& envp) {
    // if (m_envp) {
    //     for (int i = 0; m_envp[i]; i++) { delete m_envp[i]; }
    //     delete[] m_envp;
    // }

    std::map<std::string, std::string> env = envp.getMapEnvp();

    m_envp = new char*[env.size() + 1];
    int envpIndex = 0;
    for (std::map<std::string, std::string>::iterator it = env.begin();
         it != env.end(); it++) {
        std::string str = (it->first + "=" + it->second);
        m_envp[envpIndex] = new char[str.size() + 1];
        strcpy(m_envp[envpIndex], str.c_str());
        envpIndex++;
    }
    m_envp[envpIndex] = NULL;
}

void CgiHandler::setArgv(std::string const& path, std::string const& script) {
    // if (m_argv) {
    //     for (int i = 0; m_argv[i]; i++) { delete m_argv[i]; }
    //     delete[] m_argv;
    // }
    m_argv = new char*[3];
    m_argv[0] = new char[path.size() + 1];
    strcpy(m_argv[0], path.c_str());
    m_argv[1] = new char[script.size() + 1];
    strcpy(m_argv[1], script.c_str());
    m_argv[2] = NULL;
}

char* CgiHandler::getPath(void) const { return (m_path); }

char** CgiHandler::getEnvp(void) const { return (m_envp); }

char** CgiHandler::getArgv(void) const { return (m_argv); }

} // namespace cgi