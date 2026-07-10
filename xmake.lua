set_project("cpu-emulator")


set_languages("c11")

target("cpu-emulator")
    set_kind("binary")

    add_files("src/**.c")
    add_includedirs("include")

    set_targetdir("build")

    add_cflags("-Wall", "-Wextra")
    add_links("m")
