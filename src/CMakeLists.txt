# cmake list for /src

include_directories(
    ../external/glfw/include)
add_executable(kocmoc-demo main.cpp)

link_directories(/home/simon/Daten/Studium/kocmoc/repo/external/glfw/lib)

target_link_libraries(kocmoc-demo glfw)
