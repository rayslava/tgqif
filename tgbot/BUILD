load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
    name = "tgbot",
    lib_source = "@tgbot//:all",
    out_include_dir = "include/tgbot",
    headers_only = False,
    static_libraries = ["libTgBot.a"],
    visibility = ["//visibility:public"],
    cache_entries = {
        "CMAKE_INSTALL_LIBDIR": "lib",
        "CMAKE_INSTALL_FULL_LIBDIR": "/lib",
    },
    deps = [
        '@boost//:system',
        '@boost//:algorithm',
        '@boost//:asio_ssl',
    ],
)
