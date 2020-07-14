load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
    name = "rapidjson",
    lib_source = "@rapidjson//:all",
    out_include_dir = "include/rapidjson",
    headers_only = True,
    cache_entries = {
        "RAPIDJSON_BUILD_DOC": "OFF",
        "RAPIDJSON_BUILD_EXAMPLES": "OFF",
        "RAPIDJSON_BUILD_TESTS": "OFF",
    },
    visibility = ["//visibility:public"],
)
