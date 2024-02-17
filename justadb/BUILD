load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "ddl",
    hdrs = ["ddl.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "dml",
    hdrs = ["dml.h"],
    srcs = ["dml.cpp"],
    deps = [":ddl", ":database", ":utils"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "database",
    hdrs = ["database.h"],
    deps = [":ddl"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "utils",
    hdrs = ["utils.h"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "justadb",
    srcs = ["main.cpp"],
    deps = [":ddl", ":dml"],
)
