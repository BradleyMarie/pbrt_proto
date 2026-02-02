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
    name = "v2_proto",
    srcs = ["v2.proto"],
    deps = [
        "//pbrt_proto",
    ],
)

cc_proto_library(
    name = "v2_cc_proto",
    deps = [":v2_proto"],
)
