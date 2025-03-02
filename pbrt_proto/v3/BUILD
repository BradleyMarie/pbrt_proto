load("@rules_cc//cc:defs.bzl", "cc_library", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

package(default_visibility = ["//visibility:public"])

proto_library(
    name = "pbrt_proto",
    srcs = ["pbrt.proto"],
)

cc_proto_library(
    name = "pbrt_cc_proto",
    deps = [":pbrt_proto"],
)

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
