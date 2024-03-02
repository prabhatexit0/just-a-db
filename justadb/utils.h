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

} // namespace JustADb
