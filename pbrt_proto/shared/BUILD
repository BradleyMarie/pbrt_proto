load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//pbrt_proto:__subpackages__"])

cc_library(
    name = "parser",
    #srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        ":tokenizer",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/container:inlined_vector",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:string_view",
    ],
)

cc_library(
    name = "tokenizer",
    srcs = ["tokenizer.cc"],
    hdrs = ["tokenizer.h"],
    deps = [
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/status:statusor",
    ],
)

cc_test(
    name = "tokenizer_test",
    srcs = ["tokenizer_test.cc"],
    deps = [
        ":tokenizer",
        "@googletest//:gtest_main",
    ],
)
