#include "cgi/CgiHandler.hpp"

#include "cgi/Argv.hpp"
#include "cgi/Envp.hpp"
#include "utils/ft_array.hpp"
#include "utils/ft_filesystem.hpp"
#include "utils/Logger.hpp"

namespace cgi {

CgiHandler::CgiHandler(smt::shared_ptr<http::Request> const request) {
    m_cgiDir = ft::file(request->routeRequest()).getDirectory();
    m_argv = Argv(ft::file(request->routeRequest()).getFilename());
    m_envp = Envp(request);
    m_body = request->getBody();
}

CgiHandler::~CgiHandler(void) {}

Argv const& CgiHandler::getArgv(void) const { return (m_argv); }

Envp const& CgiHandler::getEnvp(void) const { return (m_envp); }

std::string CgiHandler::run(void) const { return (runAsChildProcess()); }

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
    // TODO: check if request type is POST
    if (m_envp.get("REQUEST_METHOD") == "POST") {
        write(input_fd, m_body.c_str(), m_body.size());
        rewind(input);
    }

    // convert argv and envp to char**
    char** argv = m_argv;
    char** envp = m_envp;

    pid_t pid = fork();
    if (pid < 0) {
        LOG_E("Failed to spawn child process.");
        return (resp);
    }
    else if (pid == 0) {
        // Direct I/O to temporary file;
        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);

        chdir(std::string(m_cgiDir).c_str());
        execve(argv[0], argv, envp);
        perror("execve() failed");
        ft::array::erase(argv);
        ft::array::erase(envp);

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

    // Free memory
    ft::array::erase(argv);
    ft::array::erase(envp);

    // Reset STDIN and STDOUT
    dup2(stdin_reference, STDIN_FILENO);
    dup2(stdout_reference, STDOUT_FILENO);

    // Close temporary files
    fclose(input);
    fclose(output);

    return (resp);
}

std::string CgiHandler::get(std::string key) const {
    key += "=";
    std::string value;
    for (int i = 0; m_envp[i]; i++) {
        if (strncmp(m_envp[i], key.c_str(), key.size()) == 0) {
            value = m_envp[i] + key.size();
            break;
        }
    }
    return (value);
}

CgiType convertCgiExtension(std::string const& cgiExtension) {
    if (cgiExtension == ".py") { return (Py); }
    if (cgiExtension == ".php") { return (Php); }
    if (cgiExtension == ".cgi") { return (Cgi); }
    return (Unknown);
}

std::string runCgiScript(smt::shared_ptr<http::Request> const request) {
    // // TODO: check if its needed
    // (void)opts;
    // // TODO: get path in a smarter way
    // char* path = new char[17];
    // strcpy(path, "/usr/bin/python3");

    // std::vector<std::string> vArgv;
    // vArgv.push_back("/usr/bin/python3");
    // vArgv.push_back(request->routeRequest());

    // Argv argv();

    // Envp       envp(request);
    // CgiHandler cgi(path, argv, envp);
    // // TODO: the cgi returns a string instead of a response, check if its
    // // necesary to do a converted.
    // std::string resp = cgi.run();
    // LOG_I("Cgi response: " + resp);
    (void)request;
    return ("");
}

} // namespace cgi
