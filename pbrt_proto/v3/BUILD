load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "convert",
    srcs = ["convert.cc"],
    hdrs = ["convert.h"],
    deps = [
        ":pbrt_cc_proto",
        "//pbrt_proto/shared:parser",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/strings:string_view",
    ],
)

cc_test(
    name = "convert_test",
    srcs = ["convert_test.cc"],
    deps = [
        ":convert",
        ":pbrt_cc_proto",
        "//pbrt_proto/testing:proto_matchers",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "defaults",
    srcs = ["defaults.cc"],
    hdrs = ["defaults.h"],
    deps = [
        ":pbrt_cc_proto",
    ],
)

cc_test(
    name = "defaults_test",
    srcs = ["defaults_test.cc"],
    deps = [
        ":defaults",
        ":pbrt_cc_proto",
        "//pbrt_proto/testing:proto_matchers",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/strings:string_view",
        "@googletest//:gtest_main",
    ],
)

proto_library(
    name = "pbrt_proto",
    srcs = ["pbrt.proto"],
)

cc_proto_library(
    name = "pbrt_cc_proto",
    deps = [":pbrt_proto"],
)
