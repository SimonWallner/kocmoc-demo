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

# collect the files
file(GLOB HEADER_FILES_MSVC *.hpp)
file(GLOB SOURCE_FILES *.cpp)



if (WIN32)
	# join the header files so that the VS can display them
	SET(SOURCE_FILES ${SOURCE_FILES} ${HEADER_FILES_MSVC})

	# MSVC resources stuff
#	file(GLOB RESOURCES_SHADERS_MSVC ../media/shaders/*)
#	source_group(foo FILES ../media/shaders/base.frag)

endif (WIN32)

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
	ILU
	ILUT
    )





