#include "include/protocol.hpp"
#include <arpa/inet.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

ssize_t safe_recv(int socket, std::vector<uint8_t> &buffer) {
  size_t offset = 0;
  size_t buffer_size = buffer.size();

  while (true) {
    ssize_t bytes_received =
        recv(socket, buffer.data() + offset, buffer_size - offset, 0);

    if (bytes_received == -1) {
      // Handle error
      perror("recv failed");
      return -1;
    } else if (bytes_received == 0) {
      // Connection closed
      break;
    }

    offset += bytes_received;

    // If we've filled the buffer, grow it
    if (offset == buffer_size) {
      buffer_size *= 2; // Double the size (could be adjusted)
      buffer.resize(buffer_size);
    }
    // If the received data is less than the current buffer size, we can stop
    else if (bytes_received < buffer_size) {
      buffer.resize(offset); // Trim the buffer to the exact size
      break;
    }
  }

  return offset; // Return the number of bytes actually received
}

int main(int argc, char *argv[]) {
  // Disable output buffering
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket: " << std::endl;
    return 1;
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    close(server_fd);
    std::cerr << "setsockopt failed: " << std::endl;
    return 1;
  }

  struct sockaddr_in server_addr {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(9092);

  if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&server_addr),
           sizeof(server_addr)) != 0) {
    close(server_fd);
    std::cerr << "Failed to bind to port 9092" << std::endl;
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    close(server_fd);
    std::cerr << "listen failed" << std::endl;
    return 1;
  }

  std::cout << "Waiting for a client to connect...\n";

  struct sockaddr_in client_addr {};
  socklen_t client_addr_len = sizeof(client_addr);

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  std::cerr << "Logs from your program will appear here!\n";

  while (true) {
    int client_fd =
        accept(server_fd, reinterpret_cast<struct sockaddr *>(&client_addr),
               &client_addr_len);
    std::vector<uint8_t> buffer(1024);
    std::cout << "Client connected\n";
    auto bytes_read{safe_recv(client_fd, buffer)};
    buffer.resize(bytes_read);

    std::cout << "size of vector: " << buffer.size() << '\n';

    int32_t size{de_serialize<int32_t>(buffer, 0, 3)};

    auto request_api_key{de_serialize<int16_t>(buffer, 4, 2)};

    auto request_api_version{de_serialize<uint16_t>(buffer, 6, 2)};

    auto correlation_id{de_serialize<uint32_t>(buffer, 8, 4)};

    std::cout << size << '\n';

    std::cout << correlation_id << '\n';

    auto res_size{serialize<int32_t>(4)};

    auto res_data{serialize<int32_t>(correlation_id)};

    std::vector<uint8_t> response(res_size);

    response.insert(response.end(), res_data.begin(), res_data.end());

    ssize_t bytesSent = send(client_fd, response.data(), response.size(), 0);
    if (bytesSent == -1) {
      std::cerr << "Send failed!" << std::endl;
      close(client_fd);
      close(server_fd);
      return 1;
    }

    std::cout << "Sent " << bytesSent << " bytes." << std::endl;

    close(client_fd);
  };

  close(server_fd);
  return 0;
}
