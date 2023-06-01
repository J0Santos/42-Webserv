#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils/smt.hpp"

#include <cstring>
#include <map>
#include <sys/epoll.h>
#include <unistd.h>

namespace webserv {

#ifndef BACKLOG
# define BACKLOG 42
#endif

class Server {

    private:

        typedef enum s_state { Ready, Started, Running, Stopped } t_state;

    public:

        ~Server(void);

        static smt::shared_ptr<Server> getInstance(void);

        static void start(void);
        static void run(void);
        static void stop(void);

        struct EpollCreateException : public std::exception {
                char const* what(void) const throw();
        };

        struct EpollAddException : public std::exception {
                char const* what(void) const throw();
        };

        struct EpollRemoveException : public std::exception {
                char const* what(void) const throw();
        };

        struct EpollWaitException : public std::exception {
                char const* what(void) const throw();
        };

    private:

        void startServer(void);
        void runServer(void);
        void stopServer(void);

        Server(void);

        void epollAdd(int fd, int events = EPOLLIN | EPOLLET);
        void epollRemove(int fd);

        t_state m_state;
        int     m_epollFd;
};

} // namespace webserv

#endif /* SERVER_HPP */