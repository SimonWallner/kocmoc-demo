# root build file for the KOCMOC demo

cmake_minimum_required(VERSION 2.8)
project(city CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


# make platform dependent!!!
if(WIN32)
	add_subdirectory(external/gl3w)
endif(WIN32)

# add_subdirectory(external/glm)
add_subdirectory(external/opencollada)
add_subdirectory(src)

# copy and configure
message(STATUS "configuring files...")
set(MEDIA_PATH ${PROJECT_SOURCE_DIR}/media)
configure_file(media/kocmoc.properties.in ${EXECUTABLE_OUTPUT_PATH}/kocmoc.properties)

message(STATUS "copying files...")
if(WIN32)
	configure_file(${PROJECT_SOURCE_DIR}/external/devIL/lib/DevIL.dll ${EXECUTABLE_OUTPUT_PATH} COPYONLY)
	configure_file(${PROJECT_SOURCE_DIR}/external/devIL/lib/ILU.dll ${EXECUTABLE_OUTPUT_PATH} COPYONLY)
	configure_file(${PROJECT_SOURCE_DIR}/external/devIL/lib/ILUT.dll ${EXECUTABLE_OUTPUT_PATH} COPYONLY)
	configure_file(${PROJECT_SOURCE_DIR}/external/freealut/lib/alut.dll ${EXECUTABLE_OUTPUT_PATH} COPYONLY)
endif(WIN32)



# testing
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin-test)
# add_subdirectory(test)

# enable_testing()
# add_test(all-tests ${EXECUTABLE_OUTPUT_PATH}/all-tests)


# NOTES
# Visual Studio debug commands
# Command: $(OutDir)\$(TargetFileName)
# Working Direktory: C:\..\..\..\ \\\bin
