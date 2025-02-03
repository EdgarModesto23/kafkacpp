#pragma once

#include "buffer.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <utility>
#include <vector>

// Might need to use function to "flat" objects
template <typename T> auto serialize(const T &val) noexcept {
  std::vector<uint8_t> buff(sizeof(T));
  std::memcpy(buff.data(), &val, sizeof(T));

  for (auto i = 0; i < sizeof(T) / 2; ++i) {
    std::swap(buff[i], buff[sizeof(T) - 1 - i]);
  }
  return buff;
}

// Might need to use function to translate from struct to object
template <typename T>
auto de_serialize(const std::vector<uint8_t> &data, int offset, int length) {
  static_assert(std::is_trivially_copyable_v<T>,
                "T must be trivially copyable");

  if (offset + length > data.size()) {
    throw std::out_of_range("Read range exceeds the size of the vector");
  }

  // if (length != sizeof(T)) {
  //   throw std::invalid_argument(
  //       "Length must be the same as the size of the type T");
  // }

  T obj{};
  for (auto i = 0; i < sizeof(T); ++i) {
    reinterpret_cast<uint8_t *>(&obj)[i] = data[offset + (sizeof(T) - 1 - i)];
  }
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

template <typename T> void execute_key(const T &api_key) { api_key.execute(); };
