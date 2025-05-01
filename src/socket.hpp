#ifndef Socket_hpp
#define Socket_hpp

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>

namespace Catsock {
    class Socket {
        private: 
            struct sockaddr_in address;
            int sock_fd;
            int connection;
        public:
            Socket(int family, int service, int protocol, int port, u_long interface);
            virtual int establish_connection(int sock_fd, struct sockaddr_in address) = 0;
            void check_connection(int val);
            struct sockaddr_in get_address();
            int get_sock_fd();
            int get_connection();
            void set_connection(int target);
    };
    class CSocket : public Socket {
        public:
            CSocket(int family, int service, int protocol, int port, u_long interface);
            int establish_connection(int sock_fd, struct sockaddr_in address);
    };
    class BSocket : public Socket {
        public:
            BSocket(int family, int service, int protocol, int port, u_long interface);
            int establish_connection(int sock_fd, struct sockaddr_in address);
    };
}

#endif
