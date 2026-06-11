load("@protobuf//bazel:cc_proto_library.bzl", "cc_proto_library")
load("@protobuf//bazel:proto_library.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(
    default_visibility = ["//visibility:public"],
    features = [
        "layering_check",
        "parse_headers",
    ],
)

cc_library(
    name = "convert",
    srcs = ["convert.cc"],
    hdrs = ["convert.h"],
    deps = [
        ":v1_cc_proto",
        "//pbrt_proto/shared:accelerators",
        "//pbrt_proto/shared:area_light_sources",
        "//pbrt_proto/shared:cameras",
        "//pbrt_proto/shared:common",
        "//pbrt_proto/shared:films",
        "//pbrt_proto/shared:integrators",
        "//pbrt_proto/shared:light_sources",
        "//pbrt_proto/shared:materials",
        "//pbrt_proto/shared:media",
        "//pbrt_proto/shared:parser",
        "//pbrt_proto/shared:pixel_filters",
        "//pbrt_proto/shared:proto_parser",
        "//pbrt_proto/shared:samplers",
        "//pbrt_proto/shared:shapes",
        "//pbrt_proto/shared:textures",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/functional:function_ref",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings:string_view",
    ],
)

proto_library(
    name = "v1_proto",
    srcs = ["v1.proto"],
    deps = [
        "//pbrt_proto",
    ],
)

cc_proto_library(
    name = "v1_cc_proto",
    deps = [":v1_proto"],
)
