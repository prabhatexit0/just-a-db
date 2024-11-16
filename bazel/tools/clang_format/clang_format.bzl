def clang_format(name="empty"):
    native.py_binary(
        name = "clang_format",
        srcs = ["//bazel/tools/clang_format:clang_format_runner.py"],
        main = "clang_format_runner.py",
    )