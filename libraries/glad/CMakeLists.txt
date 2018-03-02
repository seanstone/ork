set(LIBNAME "glad")

# Sources
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB SOURCE_FILES src/*.c)

# Create a static library
add_library(${LIBNAME} STATIC ${SOURCE_FILES})

link_directories(${CMAKE_CURRENT_BINARY_DIR})

# Install library
INSTALL(TARGETS ${LIBNAME} LIBRARY DESTINATION ${LIB_INSTALL_DIR} ARCHIVE       DESTINATION  ${LIB_INSTALL_DIR})
