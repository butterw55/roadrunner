cmake_minimum_required(VERSION 2.8)
project(rr_plugins)

if(${BUILD_PLUGINS} STREQUAL ON)

include_directories(
	../source
	../wrappers/C
)

add_subdirectory(source)

if(${RRP_BUILD_EXAMPLES} STREQUAL ON)
add_subdirectory(examples)
endif()

endif() 

add_subdirectory(apps)
add_subdirectory(wrappers)

install(
    FILES README.txt
    DESTINATION plugins
)
