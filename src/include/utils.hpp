#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
template <typename T> inline auto getSubArray(T *arr, int start, int end) {
  int j = 0;
  std::vector<T> subArr;
  for (int i = start; i <= end; i++) {
    subArr[j++] = arr[i];
  }
  return subArr;
}

inline bool is_request_supported(int min, int max, int requested) {
  std::cout << requested << '\n';
  return (requested <= max && requested >= min);
}

template <typename T>
std::vector<T> slice(int begin, int end, const std::vector<T> &data) {
  std::vector<T> sliced_vector;
  for (auto i = begin; i < end; i++) {
    sliced_vector.push_back(data[i]);
  }
  return sliced_vector;
};

template <typename... Vectors>
uint32_t get_vectors_total_size(Vectors &&...vectors) {
  return (vectors.size() + ...);
}

template <typename T, typename... Vectors>
std::unique_ptr<std::vector<T>> join_vectors(Vectors &&...vectors) {
  size_t total_size = (vectors.size() + ... + 0);
  auto result{std::make_unique<std::vector<T>>()};

  result->reserve(total_size);

  (result->insert(result->end(), std::make_move_iterator(vectors.begin()),
                  std::make_move_iterator(vectors.end())),
   ...);

  return result;
}
