load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(
    default_visibility = ["//pbrt_proto:__subpackages__"],
    features = [
        "layering_check",
        "parse_headers",
    ],
)

cc_library(
    name = "accelerators",
    srcs = ["accelerators.cc"],
    hdrs = ["accelerators.h"],
    deps = [
        ":parser",
        "//pbrt_proto:pbrt_cc_proto",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/strings:string_view",
    ],
)

cc_test(
    name = "accelerators_test",
    srcs = ["accelerators_test.cc"],
    deps = [
        ":accelerators",
        ":parser",
        "//pbrt_proto:pbrt_cc_proto",
        "//pbrt_proto/testing:proto_matchers",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/strings:string_view",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "area_light_sources",
    srcs = ["area_light_sources.cc"],
    hdrs = ["area_light_sources.h"],
    deps = [
        ":common",
        ":parser",
        "//pbrt_proto:pbrt_cc_proto",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/strings:string_view",
    ],
)

cc_test(
    name = "area_light_sources_test",
    srcs = ["area_light_sources_test.cc"],
    deps = [
        ":area_light_sources",
        ":parser",
        "//pbrt_proto:pbrt_cc_proto",
        "//pbrt_proto/testing:proto_matchers",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/strings:string_view",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "common",
    srcs = ["common.cc"],
    hdrs = ["common.h"],
    deps = [
        ":parser",
        "//pbrt_proto:pbrt_cc_proto",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/functional:function_ref",
        "@abseil-cpp//absl/strings:string_view",
    ],
)

proto_library(
    name = "common_test_proto",
    srcs = ["common_test.proto"],
    deps = [
        "//pbrt_proto",
    ],
)

cc_proto_library(
    name = "common_test_cc_proto",
    deps = [":common_test_proto"],
)

cc_test(
    name = "common_test",
    srcs = ["common_test.cc"],
    deps = [
        ":common",
        ":common_test_cc_proto",
        ":parser",
        "//pbrt_proto:pbrt_cc_proto",
        "//pbrt_proto/testing:proto_matchers",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/strings:string_view",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        ":tokenizer",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/container:inlined_vector",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/strings",
        "@abseil-cpp//absl/strings:string_view",
        "@abseil-cpp//absl/types:span",
    ],
)

cc_test(
    name = "parser_test",
    srcs = ["parser_test.cc"],
    deps = [
        ":parser",
        "@abseil-cpp//absl/container:inlined_vector",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest_main",
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
