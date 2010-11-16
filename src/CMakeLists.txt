# cmake list for src/

include_directories(
    ${PROJECT_SOURCE_DIR}/external/glfw/include
    ${PROJECT_SOURCE_DIR}/external/gl3w/include
    ${PROJECT_SOURCE_DIR}/external/glm/include
    ${PROJECT_SOURCE_DIR}/external/opencollada/COLLADAFramework/include
    ${PROJECT_SOURCE_DIR}/external/opencollada/COLLADABaseUtils/include
    ${PROJECT_SOURCE_DIR}/external/opencollada/COLLADASaxFrameworkLoader/include)

link_directories(
    ${PROJECT_SOURCE_DIR}/external/glfw/lib
    ${PROJECT_SOURCE_DIR}/external/opencollada/lib
    )

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

target_link_libraries(
    kocmoc-demo
    glfw
    OpenCOLLADAFramework
    OpenCOLLADASaxFrameworkLoader
    OpenCOLLADABaseUtils
    GeneratedSaxParser
    pcre
    UTF
    buffer
    ftoa
    MathMLSolver
    OpenCOLLADAStreamWriter
    xml2)
