load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_test")

package(
    default_visibility = ["//visibility:public"],
    features = [
        "layering_check",
        "parse_headers",
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

cc_test(
    name = "schema_test",
    srcs = ["schema_test.cc"],
    deps = [
        ":pbrt_cc_proto",
        "//pbrt_proto/v1:v1_cc_proto",
        "//pbrt_proto/v2:v2_cc_proto",
        "//pbrt_proto/v3:v3_cc_proto",
        "//pbrt_proto/v4:v4_cc_proto",
        "@abseil-cpp//absl/strings",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@protobuf",
    ],
)
