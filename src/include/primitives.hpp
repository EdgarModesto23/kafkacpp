#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class Nullable_string {
private:
  int16_t size;
  std::string value;

public:
  int16_t get_size();
  std::string get_value();
  void set_value(const std::string &val);
};

template <typename T> class Compact_arr {
private:
  size_t size;
  std::vector<T> values;

public:
  size_t get_size();
  const std::vector<T> get_values();
  std::unique_ptr<std::vector<T>> get_values_ref();
  bool is_null();
};
