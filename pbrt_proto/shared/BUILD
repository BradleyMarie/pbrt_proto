load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//pbrt_proto:__subpackages__"])

cc_library(
    name = "tokenizer",
    srcs = ["tokenizer.cc"],
    hdrs = ["tokenizer.h"],
)

cc_test(
    name = "tokenizer_test",
    srcs = ["tokenizer_test.cc"],
    deps = [
        ":tokenizer",
        "@googletest//:gtest_main",
    ],
)