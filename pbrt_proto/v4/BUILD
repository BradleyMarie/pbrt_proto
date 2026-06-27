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

cc_test(
    name = "schema_test",
    srcs = ["schema_test.cc"],
    deps = [
        "//pbrt_proto/shared:version",
        "//pbrt_proto/shared:version_set",
        "//pbrt_proto/testing:descriptors",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@protobuf",
    ],
)

proto_library(
    name = "v4_proto",
    srcs = ["v4.proto"],
    deps = [
        "//pbrt_proto",
    ],
)

cc_proto_library(
    name = "v4_cc_proto",
    deps = [":v4_proto"],
)
