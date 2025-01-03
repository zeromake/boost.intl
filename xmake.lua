add_rules("mode.debug", "mode.release")

set_languages("c++14")
set_encodings("utf-8")

includes("package.lua")

if is_plat("windows") then
    add_cxxflags("/EHsc", {tools = {"clang_cl", "cl"}})
    add_defines("UNICODE", "_UNICODE")
end

target("intl")
    set_kind("static")
    add_files("src/boost/locale/*.cpp")
    add_packages("nonstd.string-view")
