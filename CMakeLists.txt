cmake_minimum_required(VERSION 3.2)

PROJECT(dmshm)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

ADD_DEFINITIONS(-DUSE_GSHMLOG)
ModuleImport("dmformat" "thirdparty/dmformat")
LibImport("dmshm" "src/dmshm")
ExeImport("test" "dmshm")

ADD_CUSTOM_COMMAND(
TARGET dmshmagent
PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
${CMAKE_CURRENT_SOURCE_DIR}/config/dmshmagent.csv
$<TARGET_FILE_DIR:dmshmagent>
)

ADD_CUSTOM_COMMAND(
TARGET dmshmserver
PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
${CMAKE_CURRENT_SOURCE_DIR}/config/dmshmserver.csv
$<TARGET_FILE_DIR:dmshmserver>
)
