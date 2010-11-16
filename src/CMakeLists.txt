# cmake list for src/

include_directories(
    ${PROJECT_SOURCE_DIR}/external/glfw/include
    ${PROJECT_SOURCE_DIR}/external/gl3w/include
    ${PROJECT_SOURCE_DIR}/external/glm/include
    ${PROJECT_SOURCE_DIR}/external/opencollada/COLLADAFramework/include
    ${PROJECT_SOURCE_DIR}/external/opencollada/COLLADABaseUtils/include
    ${PROJECT_SOURCE_DIR}/external/opencollada/COLLADASaxFrameworkLoader/include
	${PROJECT_SOURCE_DIR}/external/devIL/include
)

link_directories(
    ${PROJECT_SOURCE_DIR}/external/glfw/lib
    ${PROJECT_SOURCE_DIR}/external/opencollada/lib
    ${PROJECT_SOURCE_DIR}/external/libxml/
    ${PROJECT_SOURCE_DIR}/external/devil/lib
    )

SET(HEADER_FILES_MSVC 
    utility.hpp
    shader.hpp
    Context.hpp
    PropertiesFileParser.hpp
    Exception.hpp
    kocmoc.hpp
    KocmocLoader.hpp
    PolyMesh.hpp
    Scene.hpp
    KocmocColladaImporter.hpp
    KocmocColladaErrorHandler.hpp
    ImageLoader.hpp
	)

SET(SOURCE_FILES
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
    KocmocColladaErrorHandler
    ImageLoader
)

# SET(SOURCE_FILES ${HEADER_FILES_MSVC})

add_executable(kocmoc-demo ${SOURCE_FILES})

target_link_libraries(
    kocmoc-demo
    glfw
    gl3w
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
    # xml2
    openGL32
    libXML
	devIL
    )





