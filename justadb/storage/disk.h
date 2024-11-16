#pragma once

#include <expected>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <string>
#include <utility>

enum class DiskError {
  IncompatibleSize,
  WriteError,
  ReadError,
  CanNotRead,
  CanNotOpen,
};

template <typename T> class Disk {
public:
  explicit Disk(std::string filePath) : filePath_(std::move(filePath)) {};

  auto readFromFile(const std::streampos &start, const std::streampos &end)
      -> std::expected<T, DiskError> {
    if (const size_t size = end - start; size != sizeof(T)) {
      return std::unexpected(DiskError::IncompatibleSize);
    }

    std::ifstream file(filePath_, std::ios::binary);
    file.seekg(start, std::ios::beg);

    T data;
    file.read(reinterpret_cast<char *>(&data), sizeof(T));
    if (file.fail()) {
      return std::unexpected(DiskError::ReadError);
    }
    file.close();

    return data;
  }

  auto writeToFile(const std::streampos &start, const T data)
      -> std::expected<bool, DiskError> {
    std::ofstream file;
    file.open(filePath_, std::ios::app);
    if (file.fail()) {
      return std::unexpected(DiskError::CanNotOpen);
    }

    file.write(reinterpret_cast<const char *>(&data), sizeof(T));
    if (file.fail()) {
      return std::unexpected(DiskError::WriteError);
    }
    file.close();

    return true;
  }

private:
  std::string filePath_;
};
