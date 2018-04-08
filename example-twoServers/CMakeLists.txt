# This CMakeLists.txt is intended to be used with ofnode CMake build system for openFrameworks
# see https://github.com/ofnode/of

project(ofxOSCquery-twoServers)
set(APP ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.1)

set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../of/" CACHE PATH "The root directory of ofnode/of project.")
include(${OF_ROOT}/openFrameworks.cmake)

ofxaddon(ofxOscQuery)
# ofxaddon(ofxOsc)
ofxaddon(ofxGui)

option(COTIRE "Use cotire" ON)

set(SOURCES
    src/main.cpp
    src/ofApp.cpp
    src/CirclesRenderer.h
    src/CirclesRenderer.cpp
    ../src/ofxOssiaTypes.h
    ../src/ofxOscQueryServer.h
    ../src/ofxOssiaNode.h
    ../libs/ossia/include/ossia-cpp98.hpp
)

add_executable(
    ${APP}
    MACOSX_BUNDLE
    ${SOURCES}
    ${OFXADDONS_SOURCES}
)

target_link_libraries(
    ${APP}
    ${OPENFRAMEWORKS_LIBRARIES}
)

if(UNIX AND NOT APPLE)
  target_link_libraries(
    ${APP}
    avahi-client
    avahi-common
  )
endif()

set_target_properties(
    ${APP} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST
    "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set_target_properties( ${APP} PROPERTIES OUTPUT_NAME "${APP}-Debug")
endif()

if (CMAKE_CROSSCOMPILING)
    set_target_properties( ${APP} PROPERTIES OUTPUT_NAME
      "${APP}-${OF_PLATFORM}-${CMAKE_BUILD_TYPE}")
endif()

if (COTIRE)
    cotire(${APP})
endif()
