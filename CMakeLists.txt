project(Ork CXX C)
cmake_minimum_required(VERSION 2.6)
cmake_policy(VERSION 2.6)

SET(ORK_VERSION_MAJOR 3)
SET(ORK_VERSION_MINOR 2)
SET(ORK_VERSION "${ORK_VERSION_MAJOR}.${ORK_VERSION_MINOR}")

# Avoid source tree pollution
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)

# Add a sensible build type default and warning because empty means no optimization and no debug info.
if(NOT CMAKE_BUILD_TYPE)
	message("WARNING: CMAKE_BUILD_TYPE is not defined!\n         Defaulting to CMAKE_BUILD_TYPE=RelWithDebInfo. Use ccmake to set a proper value.")
	set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Common deps
find_package(PkgConfig)

if(NOT EMSCRIPTEN)
PKG_CHECK_MODULES(Glew glew REQUIRED)
include_directories(${Glew_INCLUDE_DIRS})
link_directories(${Glew_LIBRARY_DIRS})

PKG_CHECK_MODULES(GLFW glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

find_package(GLUT)
#find_package(OpenGL)
endif(NOT EMSCRIPTEN)

set(LIB_INSTALL_DIR "lib" CACHE STRING "Library directory name (lib/lib32/lib64)" )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_definitions("-DORK_API=" "-DTIXML_USE_STL" "-fPIC")

option(BUILD_SHARED      "Build shared library instead of static"   OFF)
option(BUILD_EXAMPLES    "Build examples"                           ON )
option(BUILD_TESTS       "Build tests"                              ON )
option(USE_SHARED_PTR	 "Use std::shared_ptr"			    		ON )
option(USE_FREEGLUT	 	 "Use freeglut"				    			ON )


# Emscripten
if(EMSCRIPTEN)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s NO_EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH=1 -s DEMANGLE_SUPPORT=1 -s ASSERTIONS=1")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s USE_SDL=2 -s USE_WEBGL2=1 -s FULL_ES2=1 -s FULL_ES3=1")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/examples@/examples")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglfw3 -lGL")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
	add_definitions(-DGL_GLEXT_PROTOTYPES=1)
	add_definitions(-DGL3_PROTOTYPES=1)
	add_definitions(-DGLFW_INCLUDE_GLU)
	#add_definitions(-DGLFW_INCLUDE_ES3)
	add_definitions(-DGLFW_INCLUDE_GLEXT)
	add_definitions(-DORK_NO_GLPROGRAMUNIFORM)
	set(USE_FREEGLUT OFF)
endif(EMSCRIPTEN)


if(USE_SHARED_PTR)
	add_definitions("-DUSE_SHARED_PTR")
endif(USE_SHARED_PTR)
if(USE_FREEGLUT)
	add_definitions("-DUSEFREEGLUT")
endif(USE_FREEGLUT)


# Sub dirs
add_subdirectory(libraries)
add_subdirectory(ork)

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif(BUILD_EXAMPLES)

if(BUILD_TESTS)
	add_subdirectory(test)
endif(BUILD_TESTS)
