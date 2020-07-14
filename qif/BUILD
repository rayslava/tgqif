cc_library(
    name = "qif",
    srcs = ["qif.cpp"],
    hdrs = ["qif.hpp"],
    visibility = ["//main:__pkg__"],
    linkstatic = True,
)

cc_test(
    name = "qif_test",
    srcs = ["qif_test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "@gtest//:gtest_main"
    ],
)
