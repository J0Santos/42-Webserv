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
    int         pipeFd[2];
    int         pipeOut[2];

    // creating stdin ref
    int stdinRef = dup(STDIN_FILENO);
    if (stdinRef < 0) {
        LOG_E("Failed to create a STDIN reference");
        return (resp);
    }

    // creating stdout ref
    int stdoutRef = dup(STDOUT_FILENO);
    if (stdoutRef < 0) {
        LOG_E("Failed to create a STDOUT reference");
        return (resp);
    }

    // TODO: check if an empty response throws
    if (pipe(pipeFd) == -1) {
        LOG_E("Failed to create a pipe or request");
        return (resp);
    }
    if (pipe(pipeOut) == -1) {
        LOG_E("Failed to create a pipe of body");
        return (resp);
    }

    // writing body into pipe
    if (m_envp.get("REQUEST_METHOD") == "POST") {
        int bytes = write(pipeFd[1], m_body.c_str(), m_body.size());
        if (bytes < 0) {
            perror("Failed to write body into script");
            return (resp);
        }
    }
    close(pipeFd[1]);

    // convert argv and envp to char**
    char** argv = m_argv;
    char** envp = m_envp;

    pid_t pid = fork();
    if (pid < 0) {
        LOG_E("Failed to fork");
        return (resp);
    }
    else if (pid == 0) { // child process

        // closing write end of pipe
        close(pipeOut[0]);

        // closing read end of pipe
        if (dup2(pipeOut[1], STDOUT_FILENO) == -1) {
            perror("Failed to redirect out end of pipeOut");
            exit(EXIT_FAILURE);
        }
        close(pipeOut[1]);

        if (m_envp.get("REQUEST_METHOD") == "POST") {
            if (dup2(pipeFd[0], STDIN_FILENO)) {
                // TODO
                exit(EXIT_FAILURE);
            }
            close(pipeFd[0]);
        }
        chdir(std::string(m_cgiDir).c_str());
        execve(argv[0], argv, envp);
        perror("Failed to execute script");
        exit(EXIT_FAILURE);
    }
    else { // parent process

        // closing read end of pipe
        close(pipeFd[0]);

        // closing write end of pipe
        close(pipeFd[1]);

        int status;
        waitpid(pid, &status, 0);
        close(pipeFd[0]);
        close(pipeOut[1]);

        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // reading response
            int  bytes = 0;
            char buf[4096];
            while ((bytes = read(pipeOut[0], buf, 4096)) > 0) {
                buf[bytes] = '\0';
                resp += std::string(buf, bytes);
            }
            std::string body(buf);
            resp = body;
        }
        else {
            LOG_E("Child process finished with an error");
            ft::array::erase(argv);
            ft::array::erase(envp);
            return (resp);
        }
    }

    // freeing envp and argc
    ft::array::erase(argv);
    ft::array::erase(envp);

    close(pipeOut[0]);

    // restoring pipes
    if (dup2(stdinRef, STDIN_FILENO) < 0) {
        LOG_E("Failed to restore STDIN");
        return ("");
    }
    if (dup2(stdoutRef, STDOUT_FILENO) < 0) {
        LOG_E("Failed to restore STDOUT");
        return ("");
    }

    close(stdinRef);
    close(stdoutRef);

    return (resp);

    // std::string resp;
    // FILE* input = tmpfile();
    // int   input_fd = fileno(input);

    // FILE* output = tmpfile();
    // int   output_fd = fileno(output);

    // int stdin_reference = dup(STDIN_FILENO);
    // int stdout_reference = dup(STDOUT_FILENO);

    // if (!input || !output || stdin_reference < 0) {
    //     LOG_E("Failed to create temporary files.");
    //     return (resp);
    // }
    // if (input_fd < 0) {
    //     LOG_E("Failed to get temporary infile fd.");
    //     return (resp);
    // }

    // if (m_envp.get("REQUEST_METHOD") == "POST") {
    //     LOG_I("Writing body to temporary file. " << m_body);
    //     write(input_fd, m_body.c_str(), m_body.size());
    //     rewind(input);
    // }
    // // convert argv and envp to char**
    // char** argv = m_argv;
    // char** envp = m_envp;

    // pid_t pid = fork();
    // if (pid < 0) {
    //     LOG_E("Failed to spawn child process.");
    //     return (resp);
    // }
    // else if (pid == 0) {
    //     // Direct I/O to temporary file;
    //     dup2(input_fd, STDIN_FILENO);
    //     dup2(output_fd, STDOUT_FILENO);

    // chdir(std::string(m_cgiDir).c_str());
    // execve(argv[0], argv, envp);
    // perror("execve() failed");
    // ft::array::erase(argv);
    // ft::array::erase(envp);

    // // Child process
    // LOG_E("Failed call to execve in child process.");
    // exit(500);
    // }
    // // else {
    // //     if (m_envp.get("REQUEST_METHOD") == "POST") {
    // //         LOG_I("Writing body to temporary file. " << m_body);
    // //         write(input_fd, m_body.c_str(), m_body.size());
    // //         rewind(input);
    // //     }
    // // }
    // // From here on out we are always in parent cause child either executed
    // or
    // // exited
    // int status;

    // waitpid(pid, &status, 0);
    // if (WIFEXITED(status)) {
    //     int  bytesRead = 0;
    //     char buf[2049];
    //     lseek(output_fd, 0, SEEK_SET);
    //     while ((bytesRead = read(output_fd, buf, 2048)) > 0) {
    //         buf[bytesRead] = '\0';
    //     }
    //     std::string body(buf);
    //     resp = body;
    // }

    // // Free memory
    // ft::array::erase(argv);
    // ft::array::erase(envp);

    // // Reset STDIN and STDOUT
    // // Close temporary files
    // fclose(input);
    // fclose(output);

    // return (resp);
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
