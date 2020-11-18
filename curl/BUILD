load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

configure_make(
    name = "curl",
    lib_source = "@curl//:all",
    out_include_dir = "include/curl",
    headers_only = False,
    static_libraries = ["libcurl.a"],
    shared_libraries = ["libcurl.so"],
    visibility = ["//visibility:public"],
    deps = [
        "//zlib:zlib",
        "//ssl:ssl",
    ],
)
