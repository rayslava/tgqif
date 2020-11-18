load("//main:global.bzl", "COPTS", "DEFINES", "LINKOPTS")

cc_library(
    name = "tg",
    srcs = ["tg.cpp"],
    hdrs = ["tg.hpp"],
    visibility = ["//main:__pkg__"],
    deps = [
        "//tgbot:tgbot",
    ],
    linkstatic = True,
    copts = COPTS,
    local_defines = DEFINES,
    linkopts = LINKOPTS,
)

cc_test(
    name = "tg_test",
    srcs = ["tg_test.cpp"],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main",
    ],
    copts = COPTS,
    local_defines = DEFINES,
    linkopts = LINKOPTS,
)
