cmake_minimum_required(VERSION 2.8)
set(target bistable)

include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

include_directories(
${C_API_INCLUDE_ROOT}
${RR_ROOT}/source
../../../source
${THIRD_PARTY_INSTALL_FOLDER}/include
)

add_executable(${target} 
main.cpp 
)

add_definitions(
-DSTATIC_PLUGIN_API
-DPOCO_STATIC
-DLIBSBML_STATIC 
-DSTATIC_LIBSTRUCT
-DLIBLAX_STATIC  
-DSTATIC_NLEQ
)

target_link_libraries (${target} 
roadrunner
rrplugins_api-static
libsbml-static
poco_foundation-static
rr-libstruct-static
sundials_cvode
sundials_nvecserial
nleq-static
blas
lapack
libf2c
)

install(
TARGETS ${target} 		
DESTINATION bin 
#COMPONENT examples
)
