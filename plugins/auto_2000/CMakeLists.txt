cmake_minimum_required(VERSION 2.8)

link_directories(
${RRE_INSTALL_LIB_PATH}
${CMAKE_BINARY_DIR}/lib
)

add_subdirectory(libAuto)
add_subdirectory(rrAutoInterface)
add_subdirectory(source)

if(${INSTALL_EXAMPLES})
add_subdirectory(examples)
endif()


