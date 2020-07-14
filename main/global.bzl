COPTS = [
    "--std=c++2a",
    "-Wall",
    "-Wno-unknown-pragmas",
    "-Wno-sign-compare",
    "-Woverloaded-virtual",
    "-Wwrite-strings",
    "-Wno-unused",
    "-Wextra",
    "-pedantic",
    "-Werror",
    "-pthread"
]

LINKOPTS = [
    "-pthread",
]

DEFINES = [
    'VERSION=\\"version\\"',
    'BUILDDATE=\\"none\\"',
]
