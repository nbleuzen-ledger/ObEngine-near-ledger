cmake_minimum_required(VERSION 3.16)

set(PluginName "NearLedgerPlugin")

project(${PluginName})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE PLUGIN_HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE PLUGIN_SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PluginName} SHARED ${PLUGIN_HEADERS} ${PLUGIN_SOURCES})

target_include_directories(${PluginName}
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

set(BUILD_PLAYER OFF CACHE BOOL "")
set(BUILD_DEV OFF CACHE BOOL "")
set(BUILD_TOOLKIT OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

add_subdirectory(extlibs/near-ledger)
target_link_libraries(${PluginName} ledgercpp)

add_custom_command(
    TARGET ${PluginName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo
    $<TARGET_FILE:${PluginName}>
    > plugin_location
)

add_subdirectory(ObEngine)

target_link_libraries(${PluginName} ObEngineCore)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_EXTENSIONS OFF)
