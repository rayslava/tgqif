load("//main:global.bzl", "COPTS", "DEFINES", "LINKOPTS")

cc_binary(
    name = "tgqif",
    srcs = [ "main.cpp" ],
    deps = [
        "//json:json",
        "//qif:qif",
        "//tg:tg",
        "//ssl:ssl",
        '@boost//:system',
        '@boost//:log',
        '@boost//:program_options',
    ],
    copts = COPTS,
    local_defines = DEFINES,
    linkopts = LINKOPTS,
)
