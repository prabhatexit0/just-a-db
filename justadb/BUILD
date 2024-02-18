load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "ddl",
    hdrs = ["ddl.h"],
    srcs = ["ddl.cpp"],
    deps = [":utils"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "dml",
    hdrs = ["dml.h"],
    srcs = ["dml.cpp"],
    deps = [":ddl", ":utils"],
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
