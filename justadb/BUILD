load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_library(
    name = "ddl",
    hdrs = ["ddl.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "dml",
    hdrs = ["dml.h"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "justadb",
    srcs = ["main.cpp"],
    deps = [":ddl", ":dml"],
)
