load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//pbrt_proto:__subpackages__"])

cc_library(
    name = "descriptors",
    testonly = 1,
    srcs = ["descriptors.cc"],
    hdrs = ["descriptors.h"],
    deps = [
        "//pbrt_proto:pbrt_cc_proto",
        "//pbrt_proto/v1:v1_cc_proto",
        "//pbrt_proto/v2:v2_cc_proto",
        "//pbrt_proto/v3:v3_cc_proto",
        "//pbrt_proto/v4:v4_cc_proto",
        "@abseil-cpp//absl/base:nullability",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/functional:function_ref",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/strings",
        "@protobuf",
    ],
)

cc_library(
    name = "field_compatibility",
    testonly = 1,
    srcs = ["field_compatibility.cc"],
    hdrs = ["field_compatibility.h"],
    deps = [
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:string_view",
        "@protobuf",
    ],
)

cc_library(
    name = "field_numbering",
    testonly = 1,
    srcs = ["field_numbering.cc"],
    hdrs = ["field_numbering.h"],
    deps = [
        "@abseil-cpp//absl/log:check",
        "@protobuf",
    ],
)

cc_library(
    name = "message_compatibility",
    testonly = 1,
    srcs = ["message_compatibility.cc"],
    hdrs = ["message_compatibility.h"],
    deps = [
        ":field_compatibility",
        "@abseil-cpp//absl/log:check",
        "@protobuf",
    ],
)

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
