cmake_minimum_required(VERSION 2.8)
set(target plugin_info)

add_executable(${target} 
main.cpp 
)

add_definitions(
)

if(WIN32)
add_definitions(
-DWIN32
)

target_link_libraries (${target} 
rrplugins_api
)
endif()

if(UNIX)
add_definitions(
-DPOCO_STATIC
)
target_link_libraries (${target} 
rrplugins_api
#nleq-static.a
#rr-libstruct-static
lapack
blas
f2c
#sbml-static.a
#xml2
#sundials_nvecserial.a
#sundials_cvode.a
#pthread
poco_foundation-static.a
#dl
)
endif()


install (TARGETS ${target} 		
DESTINATION bin 
COMPONENT apps
)
