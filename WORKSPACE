workspace(name = "tgqif")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "bazel_skylib",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
    ],
    sha256 = "97e70364e9249702246c0e9444bccdc4b847bed1eb03c5a3ece4f83dfe6abc44",
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
bazel_skylib_workspace()

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "gtest",
    remote = "https://github.com/google/googletest",
    branch = "v1.10.x",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

# Rule repository
http_archive(
   name = "rules_foreign_cc",
   strip_prefix = "rules_foreign_cc-master",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

http_archive(
   name = "rapidjson",
   build_file_content = all_content,
   strip_prefix = "rapidjson-1.1.0",
   urls = ["https://github.com/Tencent/rapidjson/archive/v1.1.0.tar.gz"],
   sha256 = "bf7ced29704a1e696fbccf2a2b4ea068e7774fa37f6d7dd4039d0787f8bed98e",
)

http_archive(
   name = "tgbot",
   build_file_content = all_content,
   strip_prefix = "tgbot-cpp-1.2.1",
   urls = ["https://github.com/reo7sp/tgbot-cpp/archive/v1.2.1.tar.gz"],
   sha256 = "bcc82cc8c421e96098ba6ef350acc0c1ce9fbbc21a11b91eb8643324757d09f2",
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "1e3a69bf2d5cd10c34b74f066054cd335d033d71",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1591047380 -0700",
)

http_archive(
   name = "ssl",
   build_file_content = all_content,
   strip_prefix = "openssl-1.1.1g",
   urls = ["https://www.openssl.org/source/openssl-1.1.1g.tar.gz"],
   sha256 = "ddb04774f1e32f0c49751e21b67216ac87852ceb056b75209af2443400636d46",
)

http_archive(
   name = "curl",
   build_file_content = all_content,
   strip_prefix = "curl-7.73.0",
   urls = ["https://curl.haxx.se/download/curl-7.73.0.tar.bz2"],
   sha256 = "cf34fe0b07b800f1c01a499a6e8b2af548f6d0e044dca4a29d88a4bee146d131",
)

http_archive(
   name = "zlib",
   build_file_content = all_content,
   strip_prefix = "zlib-1.2.11",
   urls = ["https://zlib.net/zlib-1.2.11.tar.gz"],
   sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
)

# Required for compatibility with @boost//:asio_ssl
new_local_repository(
    name = "openssl",
    path = "ssl",
    build_file = "ssl/BUILD",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()
