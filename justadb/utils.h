#pragma once
#include <string>

namespace JustADb {

class Error {
public:
  enum class Kind {
    NoColumnFound,
    Unknown,
  };

  Error(Kind kind = Kind::Unknown) : kind_(kind) {}

  Error(const std::string &message, Kind kind = Kind::Unknown)
      : message_(message), kind_(kind) {}

  auto message() const -> std::string {
    if (message_.has_value()) {
      return message_.value();
    } else {
      return "Unknown error";
    }
  }
  auto kind() const { return kind_; }

private:
  std::optional<std::string> message_;
  Kind kind_;
};

template <typename T>
class Result {
  public:
  Result(T value) : value_(value) {}
  Result(Error error) : error_(error) {}

  auto value() const -> T {
    return value_.value();
  }

  auto error() const -> Error {
    return error_.value();
  }

  auto has_value() const -> bool {
    return value_.has_value();
  } 

  auto has_error() const -> bool {
    return error_.has_value();
  } 

private:
  std::optional<T> value_;
  std::optional<Error> error_;
};

} // namespace JustADb
