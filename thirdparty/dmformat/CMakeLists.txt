cmake_minimum_required(VERSION 3.12)

PROJECT(dmformat)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

ADD_DEFINITIONS(-DUSE_DMFORMAT)

if ( MSVC )
    ADD_COMPILE_OPTIONS("/std:c++17")
endif()

InterfaceImport("dmformat" "include" "")

if (PROJECT_IS_TOP_LEVEL)
    ExeImport("test" "dmformat")
endif(PROJECT_IS_TOP_LEVEL)
