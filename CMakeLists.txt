cmake_minimum_required(VERSION 3.20)
project(lingmo-ocr VERSION 1.0.0 DESCRIPTION "Lingmo OCR Service" LANGUAGES CXX)

#3rdparty
execute_process(COMMAND sh ${CMAKE_CURRENT_LIST_DIR}/build3rdparty.sh WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

option(DOTEST "option for test" OFF)

if(DOTEST)
    add_definitions(-DIN_TEST)
endif()

#lingmo-ocr
add_subdirectory(src)
