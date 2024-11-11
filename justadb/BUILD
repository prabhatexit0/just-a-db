load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "justadb",
    srcs = ["main.cpp"],
    copts = ["-fsanitize=address"],
    linkopts = ["-fsanitize=address"],
    deps = [
        "//justadb/qp",
        "//justadb/storage",
    ],
)
