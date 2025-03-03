load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//pbrt_proto:__subpackages__"])

cc_library(
    name = "proto_matchers",
    testonly = 1,
    hdrs = ["proto_matchers.h"],
    deps = [
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/memory:memory",
        "@googletest//:gtest",
        "@protobuf",
    ],
)
