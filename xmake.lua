set_languages("c++20")
add_rules("mode.debug", "mode.release")

add_includedirs("include")

target("penis-lib")
  set_kind("static")
  add_files("src/**/*.cpp")
