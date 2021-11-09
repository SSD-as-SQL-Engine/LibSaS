#include "sas.h"

namespace sas {

SaSNetwork::SaSNetwork(uint16_t server_port, std::string server_ip) {
  
  // initializer
  this->server_port = server_port;
  this->server_ip = server_ip;
  socket_option = 1;
  server_socket = 0;
  client_socket = 0;
  is_connected = false;
  
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;                     // server socket address structure
  server_addr.sin_port = htons(server_port);            // server port number
  server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());   // server address 32bit translation

  std::cerr << "SaSNetwork initialized ip: " << server_ip << " port: " << server_port << std::endl;

}

void SaSNetwork::Connect() {
  
  // create client socket 
  client_socket = socket(PF_INET, SOCK_STREAM, 0); // create TCP/IP socket
  setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &socket_option, sizeof(socket_option));
  if (client_socket == -1) {
    TEST_ERROR_PRINT("Failed to create client socket\n");
    exit(1);
  }
  
  // create request with the socket 
  if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    TEST_ERROR_PRINT("Failed to connect client socket\n");
    exit(1);
  }

}

int SaSNetwork::RecvQuery() {
  int recv_byte = 0;  
  if ((recv_byte = recv(client_socket, buff_recv, sizeof(buff_recv), 0)) <= 0) {
    TEST_ERROR_PRINT("Failed to recieve the data from server\n");
    return -1;
  } else {
    buff_recv[recv_byte] = '\0';
    std::cerr << "SaS Network received : " << buff_recv << std::endl;
  }

  // clear buffer
  memset(buff_recv, 0, sizeof(buff_recv));
  recv_byte = 0;
  
  // close socket 
  ::close(client_socket);

  return 0;
}

int SaSNetwork::SendQuery(std::string query) {
  void *buffer;
  int send_byte = 0; 
  
  posix_memalign(&buffer, 32, 4096);
  memcpy(buffer, query.c_str(), 4096);
  send_byte = send(client_socket, buffer, 4096, 0);

  free(buffer);

  if (send_byte != 4096) {
    TEST_ERROR_PRINT("Send SQL stmt failed query : %s\n", query.c_str());
    return -1;
  }


  // print received reuslt 
  RecvQuery();

  return 0;
}
}; // namesapce sas
