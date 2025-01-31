cmake_minimum_required(VERSION 3.20)
project(HelpForMax LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   add_compile_definitions(TEST_CLIENT_AVAILABLE)
endif()

#
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Svg Xml)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(WaLog REQUIRED)
find_package(WaQt REQUIRED)
find_package(WaTools REQUIRED)

#
file(GLOB SOURCE_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/*.h
   ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/*.ui
)

list(APPEND SOURCE_FILES
   ${WAQT_INCLUDE_DIRS}/MessageBar.h
   ${WAQT_INCLUDE_DIRS}/PopulatedMainWindow.h
)

qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_subdirs_files(${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
   WIN32_EXECUTABLE TRUE
   MACOSX_BUNDLE TRUE
)

set_standrard_release_output_path()
use_precompiled_headers()
set_application_icon(${CMAKE_CURRENT_SOURCE_DIR}/_Resources/${PROJECT_NAME})
run_qt_deploy()

add_subdirectory(DiscreteMaths)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/DiscreteMaths)

target_link_libraries(${PROJECT_NAME} PRIVATE DiscreteMaths Qt6::Widgets Qt6::Network Qt6::Svg Qt6::Xml)
