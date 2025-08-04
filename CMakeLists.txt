cmake_minimum_required(VERSION 3.21)

project(dmshm)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/ModuleImport.cmake)
include(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

ModuleImportAll("thirdparty")

LibImport("dmshm" "src/dmshm")
ExeImport("test" "dmshm;dmcsv")

if(PROJECT_IS_TOP_LEVEL)
    ExeImport("test" "libdmmetainfo;dmtest")
    ExeImport("examples" "dmshm")

    add_custom_command(
    TARGET dmshmagent
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/config/dmshmagent.csv
    $<TARGET_FILE_DIR:dmshmagent>
    )

    add_custom_command(
    TARGET dmshmserver
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/config/dmshmserver.csv
    $<TARGET_FILE_DIR:dmshmserver>
    )

endif()

