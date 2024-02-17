#include "ddl.h"
#include "dml.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

int main() {
  JustADb::Value value(1);

  auto int_value = std::get<int>(value.value());
  std::cout << int_value << std::endl;

  JustADb::Tuple tuple;
  tuple.insert_value("id", value);

  return 0;
}
