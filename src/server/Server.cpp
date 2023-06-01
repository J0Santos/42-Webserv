#include "server/Server.hpp"

namespace webserv {

Server::Server(void) : m_state(Ready) {}

Server::~Server(void) { close(m_epollFd); }

void Server::startServer(void) { m_state = Started; }

void Server::runServer(void) {
    m_state = Running;
    while (m_state == Running) { ; }
}

void Server::stopServer(void) { m_state = Stopped; }

void Server::epollAdd(int fd, int events) {
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = events;
    event.data.fd = fd;

    if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &event) < 0)
        throw(EpollAddException());
}

void Server::epollRemove(int fd) {

    if (epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, NULL) < 0)
        throw(EpollRemoveException());
}

smt::shared_ptr<Server> Server::getInstance(void) {
    static smt::shared_ptr<Server> ist(new Server());
    return (ist);
}

void Server::start(void) { getInstance()->startServer(); }

void Server::run(void) { getInstance()->runServer(); }

void Server::stop(void) { getInstance()->stopServer(); }

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

} // namespace webserv
