# cmake list for /src

include_directories(
    ../external/glfw/include
    ../external/gl3w/include
    ../external/glm/include)

add_executable(kocmoc-demo
    main
    utility
    shader
    Context
    PropertiesFileParser
    Exception
    kocmoc)

link_directories(/home/simon/Daten/Studium/kocmoc/repo/external/glfw/lib)

target_link_libraries(kocmoc-demo glfw )
