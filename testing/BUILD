load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "ddl_test",
  size = "small",
  srcs = ["ddl_test.cpp"],
  deps = ["@googletest//:gtest_main", "//justadb:ddl"],
)

cc_test(
  name = "dml_test",
  size = "small",
  srcs = ["dml_test.cpp"],
  deps = ["@googletest//:gtest_main", "//justadb:dml"],
)
