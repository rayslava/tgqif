load("//main:global.bzl", "COPTS")

cc_library(
    name = "json",
    srcs = ["json.cpp"],
    hdrs = ["json.hpp"],
    copts = COPTS,
    visibility = ["//main:__pkg__"],
    linkstatic = True,
    deps = [
        "//rapidjson:rapidjson",
    ]
)

cc_test(
    name = "json_test",
    srcs = ["json_test.cpp"],
    copts = COPTS,
    local_defines = [ "_UNIT_TEST_BUILD" ],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main",
        "//json:json",
    ],
    data = [ "//json:test.json" ],
)
