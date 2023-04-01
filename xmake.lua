set_project("penis")
set_version("1.1.0")
set_xmakever("2.7.0")

-- set_warnings("all", "error")
set_languages("c++20")

add_rules("mode.debug", "mode.release")

add_includedirs("include")

target("penis-lib")
  set_kind("static")
  add_files("src/**/*.cpp")

target("penis-lib-test")
  set_kind("binary")
  add_files("example/*.cpp")
  add_deps("penis-lib")

add_installfiles("(include/**)", {prefixdir = ""})
