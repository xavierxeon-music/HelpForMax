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
find_package(WaTools REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Svg Xml)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#
include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/Components
   ${CMAKE_CURRENT_SOURCE_DIR}/Graph
   ${CMAKE_CURRENT_SOURCE_DIR}/Package
   ${CMAKE_CURRENT_SOURCE_DIR}/Patch
   ${CMAKE_CURRENT_SOURCE_DIR}/PatchRef
   ${CMAKE_CURRENT_SOURCE_DIR}/Suggest     
   ${CMAKE_CURRENT_SOURCE_DIR}/Max
)

file(GLOB SOURCE_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/*.h
   ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/_Resources/*.qrc
)

file(GLOB_RECURSE UI_FILES
   ${CMAKE_CURRENT_SOURCE_DIR}/*.ui
)

if(APPLE)
   set(MACOSX_BUNDLE_ICON_FILE HelpForMax.icns)
   set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/_Resources/HelpForMax.icns)
   set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

   set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")

   if(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{HOME}/Applications)
   endif()

   qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${APP_ICON} ${UI_FILES} ${SOURCE_FILES})

elseif(UNIX)
   qt_add_executable(${PROJECT_NAME}  ${UI_FILES} ${SOURCE_FILES})

   if(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{HOME}/bin)
   endif()

elseif(WIN32)
   set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc")

   # see https://doc.qt.io/qt-6/appicon.html
   # RC_ICONS = $$PWD/icons/HelpForMax.ico
   if(CMAKE_BUILD_TYPE STREQUAL "Release")
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{LOCALAPPDATA}/${PROJECT_NAME})

      get_target_property(QMAKE_EXE Qt6::qmake IMPORTED_LOCATION)
      get_filename_component(QT_BIN_DIR "${QMAKE_EXE}" DIRECTORY)

      set(QT_QML_DIR ${QT_BIN_DIR}/../qml)
      find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
      message(STATUS "WINDEPLOY ${WINDEPLOYQT_EXECUTABLE}, QT_QML_DIR ${QT_QML_DIR}")

      add_custom_command(TARGET ${PROJECT_NAME}
         POST_BUILD
         COMMAND "${WINDEPLOYQT_EXECUTABLE}" --no-translations --no-system-d3d-compiler --compiler-runtime --qmldir ${QT_QML_DIR} \"$<TARGET_FILE:${PROJECT_NAME}>\"
      )
   endif()

   qt_add_executable(${PROJECT_NAME} WIN32 ${APP_ICON} ${UI_FILES} ${SOURCE_FILES})
endif()

add_subdirectory(DiscreteMaths)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/DiscreteMaths)

target_link_libraries(${PROJECT_NAME} PRIVATE DiscreteMaths Qt6::Widgets Qt6::Network Qt6::Svg Qt6::Xml)

include(${CMAKE_CURRENT_LIST_DIR}/Components/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Max/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Package/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Patch/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/PatchRef/build.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Suggest/build.cmake)
