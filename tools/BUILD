load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_test")

package(
    default_visibility = ["//visibility:public"],
    features = [
        "layering_check",
        "parse_headers",
    ],
)

filegroup(
    name = "test_scenes",
    srcs = glob([
        "test_data/pbrt-v3-scenes/**/*.pbrt",
    ]),
)

cc_binary(
    name = "pbrt_proto_converter",
    srcs = ["pbrt_proto_converter.cc"],
    deps = [
        "//pbrt_proto/v3:convert",
        "//pbrt_proto/v3:pbrt_cc_proto",
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/status:status",
        "@abseil-cpp//absl/strings:string_view",
        "@protobuf",
        "@protobuf//src/google/protobuf/io",
    ],
)

cc_test(
    name = "pbrt_proto_converter_test",
    size = "large",
    srcs = ["pbrt_proto_converter_test.cc"],
    data = [
        ":pbrt_proto_converter",
        ":test_scenes",
    ],
    deps = [
        "@bazel_tools//tools/cpp/runfiles",
        "@googletest//:gtest_main",
    ],
)
