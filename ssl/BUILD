load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

configure_make(
    name = "ssl",
    lib_source = "@ssl//:all",
    static_libraries = ["libssl.a", "libcrypto.a"],
    shared_libraries = ["libssl.so", "libcrypto.so"],
    visibility = ["//visibility:public"],
    configure_command = "config",
)
