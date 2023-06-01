#include "server/Server.hpp"

namespace net {

Server::Server(void) : m_state(Ready) {}

Server::~Server(void) { close(m_epollFd); }

void Server::startSocket(int port, std::string const& host) {
    // initializing socket from address
    smt::shared_ptr<ServerSocket> sock;
    sock = smt::make_shared(new ServerSocket(port, host));

    sock->socket();

    // setting socket options
    int enable = 1;
    sock->setsockopt(SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    // // TODO: check siege with this
    // struct timeval timeout;
    // timeout.tv_sec = 10;
    // timeout.tv_usec = 0;
    // sock->setsockopt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct
    // timeval));

    // binding and listening
    sock->bind();
    sock->listen();

    // adding socket to epoll list
    epollAdd(sock->getSockFd(), EPOLLIN);

    // adding socket to sockets list
    m_sockets[sock->getSockFd()] = sock;
}

void Server::epollAdd(int fd, int events) {
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.fd = fd;

    if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event) < 0)
        throw(EpollAddException());
}

void Server::epollRemove(int fd) {

    if (epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, NULL) < 0) {
        throw(EpollRemoveException());
    }
}

smt::shared_ptr<Server> Server::getInstance(void) {
    static smt::shared_ptr<Server> ist(new Server());
    return (ist);
}

void Server::start(void) {
    smt::shared_ptr<Server> srv = getInstance();

    // creating an epoll instance
    if ((srv->m_epollFd = epoll_create(EP_MAX_EVENTS)) < 0) {
        throw(EpollCreateException());
    }

    // initializing sockets
    std::set< std::pair<int, std::string> > socketOpts =
        config::Options::getSocketOptions();

    std::set< std::pair<int, std::string> >::iterator it;
    for (it = socketOpts.begin(); it != socketOpts.end(); it++) {
        srv->startSocket((*it).first, (*it).second);
    }

    // updating state
    srv->m_state = Started;
}

void Server::run(void) {
    smt::shared_ptr<Server> srv = getInstance();

    srv->m_state = Running;
    while (srv->m_state == Running) { ; }
}

void Server::stop(void) {
    smt::shared_ptr<Server> srv = getInstance();
    srv->m_state = Stopped;
}

char const* Server::EpollCreateException::what(void) const throw() {
    return ("webserv::Server epoll_create system call failure");
}

char const* Server::EpollAddException::what(void) const throw() {
    return ("webserv::Server epoll_ctl (add) system call failure");
}

char const* Server::EpollRemoveException::what(void) const throw() {
    return ("webserv::Server epoll_ctl (remove) system call failure");
}

char const* Server::EpollWaitException::what(void) const throw() {
    return ("webserv::Server epoll_wait system call failure");
}

} // namespace net
