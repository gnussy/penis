set_languages("c++20")
add_rules("mode.debug", "mode.release")

local libs = { "fmt", "gtest" }

add_includedirs("include")
add_requires(table.unpack(libs))

target("penis-lib")
  set_kind("static")
  add_files("source/**/*.cpp")
  add_packages(table.unpack(libs))

target("penis-test")
  set_kind("binary")
  add_files("standalone/main.cpp")
  add_packages(table.unpack(libs))
  add_deps("penis-lib")

target("test")
  set_kind("binary")
  add_files("test/*.cpp")
  add_packages(table.unpack(libs))
  add_deps("penis-lib")
