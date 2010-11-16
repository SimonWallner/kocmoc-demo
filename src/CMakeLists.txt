# cmake list for src/

include_directories(
    ../external/glfw/include
    ../external/gl3w/include
    ../external/glm/include
    ../external/opencollada/framework/include
    ../external/opencollada/baseUtils/include
    ../external/opencollada/saxFWLoader/include)

add_executable(kocmoc-demo
    main
    utility
    shader
    Context
    PropertiesFileParser
    Exception
    kocmoc
    KocmocLoader
    PolyMesh
    Scene
    KocmocColladaImporter
    KocmocColladaErrorHandler)

link_directories(../external/glfw/lib
    ../external/opencollada/lib)

target_link_libraries(kocmoc-demo glfw )
