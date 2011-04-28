# cmake list for src/

# general stuff
add_definitions(-DBOOST_DATE_TIME_NO_LIB -DBOOST_REGEX_NO_LIB)


include_directories(
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/external/glfw/include
	${PROJECT_SOURCE_DIR}/external/gl3w/include
	${PROJECT_SOURCE_DIR}/external/glm/include
	${PROJECT_SOURCE_DIR}/external/opencollada/COLLADAFramework/include
	${PROJECT_SOURCE_DIR}/external/opencollada/COLLADABaseUtils/include
	${PROJECT_SOURCE_DIR}/external/opencollada/COLLADASaxFrameworkLoader/include
	${PROJECT_SOURCE_DIR}/external/devIL/include
	${PROJECT_SOURCE_DIR}/external/boost/include
	${PROJECT_SOURCE_DIR}/external/freealut/include
	${PROJECT_SOURCE_DIR}/external/libogg/include
	${PROJECT_SOURCE_DIR}/external/libvorbis/include
	${PROJECT_SOURCE_DIR}/external/openal/include
)

link_directories(
	${PROJECT_SOURCE_DIR}/external/glfw/lib
	${PROJECT_SOURCE_DIR}/external/opencollada/lib
	${PROJECT_SOURCE_DIR}/external/libxml/
	${PROJECT_SOURCE_DIR}/external/devil/lib
	${PROJECT_SOURCE_DIR}/external/boost/lib
	${PROJECT_SOURCE_DIR}/external/freealut/lib
	${PROJECT_SOURCE_DIR}/external/libogg/lib
	${PROJECT_SOURCE_DIR}/external/libvorbis/lib
	${PROJECT_SOURCE_DIR}/external/openal/lib
)

# collect the files
file(GLOB_RECURSE HEADER_FILES_MSVC *.hpp)
file(GLOB_RECURSE SOURCE_FILES *.cpp)



if (WIN32)
	# join the header files so that the VS can display them
	SET(SOURCE_FILES ${SOURCE_FILES} ${HEADER_FILES_MSVC})

	# MSVC resources stuff
#	file(GLOB RESOURCES_SHADERS_MSVC ../media/shaders/*)
#	source_group(foo FILES ../media/shaders/base.frag)

endif (WIN32)


add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
add_executable(RTVIS ${SOURCE_FILES})

set (TARGET_LINK_LIBRARIES 
	RTVIS
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
	ILU
	ILUT
	alut
	libogg_static
	libvorbis_static
	libvorbisfile_static
	OpenAL32
)

# platform dependent linking
if (WIN32)
	set(TARGET_LINK_LIBRARIES ${TARGET_LINK_LIBRARIES}
	gl3w
	openGL32
	libXML
	devIL

)
else (WIN32)
	set(TARGET_LINK_LIBRARIES ${TARGET_LINK_LIBRARIES}
	xml2
	IL
)
endif (WIN32)


target_link_libraries(${TARGET_LINK_LIBRARIES})





