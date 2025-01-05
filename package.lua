set_policy("package.install_only", true)
add_repositories("zeromake https://github.com/zeromake/xrepo.git")

add_requires("nonstd.string-view")
add_requires("gtest")
