#pragma once

#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// network version
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

extern "C" {
#include "nvme.h"
}

namespace sas {

#define TEST_ERROR_PRINT(fmt, args...)             \
  fprintf(stderr, "[ERROR]: %s:%d:%s():" fmt,      \
  __FILE__, __LINE__, __func__, ##args)            \


class SaS {
  private:
     std::string device;
     int fd;
     SQLITE_STAT stat;

  public:
    SaS() = default;
    SaS(std::string device): device(device) {};

    void open(std::string);
    void close();
    void executeQuery(std::string query);
};

// network version class
class SaSNetwork : public SaS {
  private:
    uint16_t server_port;
    std::string server_ip;
    int socket_option;
    int server_socket;
    int client_socket;
    int client_addr_size;
    bool is_connected;

    struct sockaddr_in server_addr; // server's socket struct (Cosmos+ board)

    char buff_recv[4096];
    char buff_send[4096];

   public:
    SaSNetwork(uint16_t server_port, std::string server_ip);

    inline bool IsConnected() {
      return is_connected;
    }

    void Connect();
    int SendQuery(std::string query);
    int RecvQuery();
};

}; // namespace sas
