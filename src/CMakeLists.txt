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
	main.cpp
    utility.cpp
    shader.cpp
    Context.cpp
    PropertiesFileParser.cpp
    Exception.cpp
    kocmoc.cpp
    KocmocLoader.cpp
    PolyMesh.cpp
    Scene.cpp
    KocmocColladaImporter.cpp
    KocmocColladaErrorHandler.cpp
    ImageLoader.cpp
)

# SET(SOURCE_FILES ${HEADER_FILES_MSVC})

# add_executable(kocmoc-demo ${HEADER_FILES_MSVC} ${SOURCE_FILES})
add_executable(kocmoc-demo
	common.hpp
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
	main.cpp
    utility.cpp
    shader.cpp
    Context.cpp
    PropertiesFileParser.cpp
    Exception.cpp
    kocmoc.cpp
    KocmocLoader.cpp
    PolyMesh.cpp
    Scene.cpp
    KocmocColladaImporter.cpp
    KocmocColladaErrorHandler.cpp
    ImageLoader.cpp
    Timer.cpp
    Timer.hpp
    Camera.hpp
    Camera.cpp
    FilmCamera.cpp
    FilmCamera.hpp
)

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
	ILU
	ILUT
    )





