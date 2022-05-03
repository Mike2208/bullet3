set(PROJECT_NAME "MusicIntegration")
set(HEADER_DIRECTORY "music_integration")

set(NAMESPACE_NAME "${PROJECT_NAME}")

set(LIBRARY_NAME "${PROJECT_NAME}")
set(EXECUTABLE_NAME "${PROJECT_NAME}Exec")
set(TEST_NAME "${PROJECT_NAME}Tests")

set(LIB_EXPORT_NAME "${LIBRARY_NAME}Targets")
set(LIB_CONFIG_NAME "${LIBRARY_NAME}Config")
set(LIB_VERSION_NAME "${LIB_CONFIG_NAME}Version")

cmake_minimum_required(VERSION 3.16)
project("${PROJECT_NAME}" VERSION 1.0.0)

include(GNUInstallDirs)

option(ENABLE_TESTING "Compile tests" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(MUSIC REQUIRED)

# List library files here
set(LIB_SRC_FILES
    music_integration/music_integration.cpp
)

# List executable source files here
set(EXEC_SRC_FILES
)

# List testing files here
set(TEST_SRC_FILES
)


##########################################
## Library
add_library("${LIBRARY_NAME}" SHARED ${LIB_SRC_FILES})
add_library("${NAMESPACE_NAME}::${LIBRARY_NAME}" ALIAS ${LIBRARY_NAME})
target_compile_options(${LIBRARY_NAME} PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall -Wextra>)

target_include_directories(${LIBRARY_NAME}
	PUBLIC 
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"

	PRIVATE
)

target_link_libraries(${LIBRARY_NAME}
	PUBLIC
            BulletRoboticsGUI
            BulletExampleBrowserLib
            BulletRobotics
            BulletFileLoader
            BulletWorldImporter
            BulletSoftBody
            BulletDynamics
            BulletCollision
            BulletInverseDynamicsUtils
            BulletInverseDynamics
            LinearMath
            OpenGLWindow
            gwen
            BussIK
            Bullet3Common

            MUSIC::MUSIC
	PRIVATE
)


##########################################
## Executable
if(NOT "${EXEC_SRC_FILES}" STREQUAL "")
	add_executable("${EXECUTABLE_NAME}" ${EXEC_SRC_FILES})
	target_link_libraries("${EXECUTABLE_NAME}" "${LIBRARY_NAME}")
endif()


##########################################
## Tests
if(${ENABLE_TESTING} AND NOT "${TEST_SRC_FILES}" STREQUAL "")
	enable_testing()

	add_executable(${TEST_NAME} ${TEST_SRC_FILES})
	target_link_libraries(${TEST_NAME}
		PUBLIC
	)

	gtest_discover_tests(${TEST_NAME}
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
		EXTRA_ARGS -VV)
endif()


##########################################
## Install library files
set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
install(TARGETS ${LIBRARY_NAME}
        #EXPORT ${LIB_EXPORT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_DIRECTORY})

# Install headers
install(DIRECTORY ${HEADER_DIRECTORY}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/bullet"
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp")

# Export target
#install(EXPORT ${LIB_EXPORT_NAME}
#	DESTINATION ${INSTALL_CONFIGDIR}
#	FILE "${LIB_EXPORT_NAME}.cmake"
#	NAMESPACE "${NAMESPACE_NAME}::")

## create version and config files
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${LIB_VERSION_NAME}.cmake"
#	VERSION ${PROJECT_VERSION}
#	COMPATIBILITY AnyNewerVersion)

#configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/cmake/ProjectConfig.cmake.in"
#	"${CMAKE_CURRENT_BINARY_DIR}/${LIB_CONFIG_NAME}.cmake"
#	INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

### Install all the helper files
#install(FILES	"${CMAKE_CURRENT_BINARY_DIR}/${LIB_CONFIG_NAME}.cmake"
#		"${CMAKE_CURRENT_BINARY_DIR}/${LIB_VERSION_NAME}.cmake"
#	DESTINATION ${INSTALL_CONFIGDIR})


##########################################
## Install executable
if(TARGET ${EXECUTABLE_NAME})
	install(TARGETS ${EXECUTABLE_NAME}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

