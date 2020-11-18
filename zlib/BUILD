load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

configure_make(
    name = "zlib",
    lib_source = "@zlib//:all",
    headers_only = False,
    static_libraries = ["libz.a"],
    shared_libraries = ["libz.so"],
    visibility = ["//visibility:public"],
)
