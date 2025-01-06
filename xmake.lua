add_rules("mode.debug", "mode.release")

set_encodings("utf-8")
option("unit")
    set_default(false)
    set_showmenu(true)
    set_description("Build unit tests")
option_end()

includes("package.lua")

if has_config("unit") then
    add_requires("gtest")
end

if is_plat("windows") then
    add_cxxflags("/EHsc", {tools = {"clang_cl", "cl"}})
    add_defines("UNICODE", "_UNICODE")
end

target("intl")
    set_kind("static")
    set_languages("c++14")
    add_includedirs("include", {public = true})
    add_files("src/boost/locale/*.cpp")
    add_packages("nonstd.string-view", {public = true})
    add_headerfiles("include/(boost/locale/*.hpp)")
    add_headerfiles("include/(boost/locale/*.h)")

target("tests")
    set_default(false)
    add_includedirs("include")
    -- add_files("tests/test_libintl.cpp")
    add_files("tests/*.cpp|main.cpp")
    add_files("tests/main.cpp")
    add_tests("default")
    add_packages("gtest")
    set_languages("c++17")
    add_deps("intl")
    on_config(function (target)
        local dir = path.absolute(os.projectdir(), '')
        dir = dir:gsub("\\", "/")
        target:add("defines", "BOOST_LOCALE_TEST_DATA=\"" .. dir .. "/tests/data\"")
    end)
