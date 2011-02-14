# testing with google C++ Test


include_directories(
	${PROJECT_SOURCE_DIR}/test/external/gtest/include
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/external/gl3w/include
	${PROJECT_SOURCE_DIR}/external/glfw/include
	${PROJECT_SOURCE_DIR}/external/glm/include
)

link_directories(
	${PROJECT_SOURCE_DIR}/test/external/gtest/lib
)


# collect the files
file(GLOB_RECURSE HEADER_FILES_MSVC *.hpp)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

if (MSVC)
	# join the header files so that the VS can display them
	set(SOURCE_FILES ${SOURCE_FILES} ${HEADER_FILES_MSVC})
endif (MSVC)


add_executable(all-tests ${SOURCE_FILES})

target_link_libraries(all-tests
	gtest_main
	gtest
)
