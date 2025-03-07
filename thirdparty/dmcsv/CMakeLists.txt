cmake_minimum_required(VERSION 3.21)

PROJECT(dmcsv)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

LibImport("dmcsv" "src")

if (PROJECT_IS_TOP_LEVEL)
    ExeImport("test" "dmcsv")
endif(PROJECT_IS_TOP_LEVEL)

