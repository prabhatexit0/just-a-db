#pragma once
#include <optional>
#include <string>

namespace JustADb {

class Error {
public:
  enum class Kind {
    NoColumnFound,
    Unknown,
  };

  explicit Error(Kind kind = Kind::Unknown) : kind_(kind) {}

  explicit Error(const std::string &message, Kind kind = Kind::Unknown)
      : message_(message), kind_(kind) {}

  [[nodiscard]] auto message() const -> std::string {
    if (message_.has_value()) {
      return message_.value();
    } else {
      return "Unknown error";
    }
  }
  [[nodiscard]] auto kind() const {
    return kind_;
  }

private:
  std::optional<std::string> message_;
  Kind kind_;
};

template <typename T> class Result {
public:
  Result(T value) : value_(value) {}
  Result(Error error) : error_(error) {}

  [[nodiscard]] auto value() const -> T {
    return value_.value();
  }

  [[nodiscard]] auto error() const -> Error {
    return error_.value();
  }

  [[nodiscard]] auto has_value() const -> bool {
    return value_.has_value();
  }

  [[nodiscard]] auto has_error() const -> bool {
    return error_.has_value();
  }

private:
  std::optional<T> value_;
  std::optional<Error> error_;
};

} // namespace JustADb
