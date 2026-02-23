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
        ":v3_cc_proto",
        "//pbrt_proto/shared:accelerators",
        "//pbrt_proto/shared:area_light_sources",
        "//pbrt_proto/shared:cameras",
        "//pbrt_proto/shared:common",
        "//pbrt_proto/shared:films",
        "//pbrt_proto/shared:parser",
        "//pbrt_proto/shared:pixel_filters",
        "@abseil-cpp//absl/container:flat_hash_map",
        "@abseil-cpp//absl/functional:function_ref",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings:string_view",
        "@abseil-cpp//absl/types:span",
    ],
)

cc_test(
    name = "convert_test",
    srcs = ["convert_test.cc"],
    deps = [
        ":convert",
        ":v3_cc_proto",
        "//pbrt_proto/testing:proto_matchers",
        "@abseil-cpp//absl/status:status_matchers",
        "@googletest//:gtest_main",
    ],
)

proto_library(
    name = "defaults_proto",
    srcs = ["defaults.proto"],
    deps = [
        ":v3_proto",
        "//pbrt_proto",
    ],
)

cc_proto_library(
    name = "defaults_cc_proto",
    deps = [":defaults_proto"],
)

filegroup(
    name = "defaults_txtpb",
    srcs = [
        ":defaults.txtpb",
    ],
    visibility = ["//visibility:public"],
)

genrule(
    name = "defaults_binpb",
    srcs = [
        ":defaults.txtpb",
    ],
    outs = ["defaults.binpb"],
    cmd = "cat $(execpath :defaults.txtpb) | " +
          "$(execpath @protobuf//:protoc) " +
          "--encode=pbrt_proto.v3.Defaults " +
          "--deterministic_output " +
          "--descriptor_set_in=$(execpath :defaults_proto):$(execpath :v3_proto):$(execpath //pbrt_proto) " +
          "> $@",
    tools = [
        ":defaults_proto",
        ":v3_proto",
        "//pbrt_proto",
        "@protobuf//:protoc",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "defaults_test",
    srcs = ["defaults_test.cc"],
    data = [
        ":defaults.binpb",
    ],
    deps = [
        ":defaults_cc_proto",
        ":v3_cc_proto",
        "//pbrt_proto/testing:proto_matchers",
        "@abseil-cpp//absl/log:check",
        "@abseil-cpp//absl/status:status_matchers",
        "@abseil-cpp//absl/strings:string_view",
        "@bazel_tools//tools/cpp/runfiles",
        "@googletest//:gtest_main",
    ],
)

proto_library(
    name = "v3_proto",
    srcs = ["v3.proto"],
    deps = [
        "//pbrt_proto",
    ],
)

cc_proto_library(
    name = "v3_cc_proto",
    deps = [":v3_proto"],
)
