load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "pbrt_proto_converter",
    srcs = ["pbrt_proto_converter.cc"],
    deps = [
        "//pbrt_proto/v3:convert",
        "//pbrt_proto/v3:pbrt_cc_proto",
        "@abseil-cpp//absl/container:flat_hash_set",
        "@abseil-cpp//absl/flags:flag",
        "@abseil-cpp//absl/flags:parse",
        "@abseil-cpp//absl/status:statusor",
        "@abseil-cpp//absl/strings:string_view",
    ],
)
