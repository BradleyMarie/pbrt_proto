load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")

package(
    default_visibility = ["//visibility:public"],
    features = [
        "layering_check",
        "parse_headers",
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
