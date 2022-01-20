#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdexcept>
#include <thread>
#include <vector>

/**
 * The AF_UNIX (also known as AF_LOCAL) socket family is used to communicate between processes on
 * the same machine efficiently. Traditionally, UNIX domain sockets can be either unnamed, or bound
 * to a filesystem pathname (marked as being of type socket). Linux also supports an abstract
 * namespace which is independent of the filesystem.
 *
 * Valid socket types in the UNIX domain are:
 *     - SOCK_STREAM, for a stream-oriented socket;
 *     - SOCK_DGRAM, for a datagram-oriented socket that preserves message boundaries (as on most
 * UNIX implementations, UNIX domain datagram sockets are always reliable and don't reorder
 * datagrams);
 *     - SOCK_SEQPACKET(since Linux 2.6.4), for a sequenced-packet socket that is
 * connection-oriented, preserves message boundaries, and delivers messages in the order that they
 * were sent.
 *
 * UNIX domain sockets support passing file descriptors or process credentials to other processes
 * using ancillary data.
 */
namespace uds {
namespace internal {
void throw_errno(const char* s) {
  throw std::runtime_error(std::string(s) + strerror(errno));
}
}  // namespace internal

class UnixDomainSocketClient {
 public:
  UnixDomainSocketClient(char const* socket_path) {
    // Creates a new socket
    if ((file_descriptor_ = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
      internal::throw_errno("socket error: ");
    }

    // Constructs the socket address object with the socket_path string
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if (*socket_path == '\0') {
      *addr.sun_path = '\0';
      strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
    } else {
      strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
    }

    // Open a connection on socket FD to peer at ADDR (which LEN bytes long).
    // For connectionless socket types, just set the default address to send to
    // and the only address from which to accept transmissions.
    if (connect(file_descriptor_, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
      internal::throw_errno("connect error: ");
    }
  }

  void send(char const* buf) {
    int n = std::char_traits<char>::length(buf);
    if (write(file_descriptor_, buf, n) != n) {
      if (n > 0) {
        fprintf(stderr, "partial write");

      } else {
        internal::throw_errno("write error: ");
      }
    }
  }

 private:
  int file_descriptor_;
};

class UnixDomainSocketServer {
 public:
  UnixDomainSocketServer(char const* socket_path) {
    // Creates a new socket
    if ((file_descriptor_ = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
      internal::throw_errno("socket error: ");
    }

    // Constructs the socket address object with the socket_path string
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    if (*socket_path == '\0') {
      *addr.sun_path = '\0';
      strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
    } else {
      strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
      unlink(socket_path);
    }

    // Gives the socket FD the local address
    if (bind(file_descriptor_, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
      internal::throw_errno("bind error: ");
    }

    // Prepares to accept connections on socket FD
    const int queue_size = 5;
    if (listen(file_descriptor_, queue_size) == -1) {
      internal::throw_errno("listen error: ");
    }
  }

  void accept() {
    std::vector<std::thread> threads;
    while (true) {
      int socket_descriptor;

      // Await a connection on socket FD.
      // When a connection arrives, open a new socket to communicate with it,
      // set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
      // peer and *ADDR_LEN to the address's actual length, and return the
      // new socket's descriptor, or -1 for errors.
      if ((socket_descriptor = ::accept(file_descriptor_, NULL, NULL)) == -1) {
        internal::throw_errno("accept error: ");
      }
      printf("Connected.\n");
      threads.emplace_back(&UnixDomainSocketServer::readStream, socket_descriptor);
    }
  }

 private:
  int file_descriptor_;

  static void readStream(int socket_descriptor) {
    char buf[100];
    int read_count;
    while ((read_count = read(socket_descriptor, buf, sizeof(buf))) > 0) {
      printf("Received %u bytes: %.*s\n", read_count, read_count, buf);
    }
    if (read_count == -1) {
      internal::throw_errno("read error: ");
    } else if (read_count == 0) {
      close(socket_descriptor);
      printf("Disconnected.\n");
    }
  }
};
}  // namespace uds
