#include "Socket.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket() : _fd(-1) {}

Socket::~Socket() {
  if (_fd != -1) {
    close(_fd);
  }

  freeaddrinfo(_res);
}

void Socket::connect(const char *hostname, const char *port) {
  struct addrinfo hints, *p;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(hostname, port, &hints, &_res) != 0) {
    throw SocketException("Could not get address info");
  }

  for (p = _res; p != NULL; p = p->ai_next) {
    _fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (_fd < 0) {
      continue;
    }

    if (::connect(_fd, p->ai_addr, p->ai_addrlen) < 0) {
      close(_fd);
      continue;
    }

    break;
  }

  if (p == NULL) {
    throw SocketException("Could not connect to server");
  }
}
