set_languages("c++20")
add_rules("mode.debug", "mode.release")

local libs = { "gtest" }

add_includedirs("include")
add_requires(table.unpack(libs))

target("penis-lib")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(libs))

target("test")
  set_kind("binary")
  add_files("test/*.cpp")
  add_packages(table.unpack(libs))
  add_deps("penis-lib")
