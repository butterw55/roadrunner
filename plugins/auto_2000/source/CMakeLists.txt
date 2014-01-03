cmake_minimum_required(VERSION 2.8)

set(target rrp_auto2000)

include_directories(
../../source   #RoadRunner Sources
../../../source   #RoadRunner Sources
../../wrappers/C
${CMAKE_INSTALL_PREFIX}/include
${THIRD_PARTY_INSTALL_FOLDER}/include
)

link_directories (
${CMAKE_BINARY_DIR}/lib
${CMAKE_INSTALL_PREFIX}/lib
)

set(sources
rrAutoPlugin
rrAutoWorker
)


add_definitions(
-DPOCO_STATIC
-DLIBSBML_STATIC
-DSTATIC_PLUGINS_API
-DSTATIC_AUTO
-DSTATIC_RR_AUTO
-DSTATIC_LIBSTRUCT
-DLIBLAX_STATIC
-DSTATIC_NLEQ
-DEXPORT_RR_PLUGIN
)

add_library(${target} MODULE ${sources})

if(WIN32)

target_link_libraries (${target}
roadrunner
rrAutoInterface-static
libAuto-static
#rrplugins_api-static
#poco_foundation-static
#libsbml-static
#libxml2
#rr-libstruct-static
#sundials_cvode
#sundials_nvecserial
#blas
#lapack
#libf2c
#nleq-static
)
endif()

install (TARGETS ${target}
DESTINATION plugins
COMPONENT plugins
)

