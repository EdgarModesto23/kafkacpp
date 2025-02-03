#pragma once

#include "buffer.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <vector>

// Might need to use function to "flat" objects
template <typename T> auto serialize(const T &val) noexcept {
  std::vector<uint8_t> buff(sizeof(T));
  std::memcpy(buff.data(), &val, sizeof(T));
  return buff;
}

// Might need to use function to translate from struct to object
template <typename T> auto de_serialize(const std::vector<std::byte> &data) {
  if (data.size() != sizeof(T)) {
    throw std::invalid_argument(
        "Byte sequence size does not match the object size.");
  }
  T obj;
  std::memcpy(&obj, data.data(), sizeof(T));
  return obj;
};

class Request {
private:
  int size;
  R_header headers;
  std::vector<std::byte> body;

public:
  int get_size();
  auto get_headers();
  auto get_body();
  Request(const std::vector<std::byte> r)
      : size(r.size()), headers(parse_headers(r)), body(){};
  Request(Request &&request) noexcept
      : size(request.size), headers(std::move(request.headers)),
        body(std::move(request.body)) {}
};

template <typename T>
void execute_key(const T &api_key, const Request &request) {
  api_key.execute(request);
};
