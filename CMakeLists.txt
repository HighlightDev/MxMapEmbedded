cmake_minimum_required(VERSION 3.16.3)

message(STATUS "CMake version: ${CMAKE_VERSION}")

set(PROJECT_NAME "MxMapEmbedded")

project(
  ${PROJECT_NAME}
  VERSION 1.0
  LANGUAGES CXX)

find_package(Threads REQUIRED)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++20" COMPILER_SUPPORTS_CXX20)
check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX20)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(
    STATUS
      "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler."
  )
endif()

set(SDBUSCPP_BUILD_CODEGEN
    ON
    CACHE BOOL "Override from main project" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/sdbus-cpp/)

target_compile_features(sdbus-c++-objlib PRIVATE cxx_std_20)
target_compile_features(sdbus-c++ PRIVATE cxx_std_20)

target_compile_options(sdbus-c++-objlib PRIVATE -fcoroutines)
target_compile_options(sdbus-c++ PRIVATE -fcoroutines)

add_custom_command(
  OUTPUT
    ${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/generated_interfaces/proxy.hpp
    ${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/generated_interfaces/adapter.hpp
  COMMAND
    ${CMAKE_CURRENT_BINARY_DIR}/submodules/sdbus-cpp/tools/sdbus-c++-xml2cpp
    ${PROJECT_SOURCE_DIR}/src/ipc/dbus_schema/org.sdbuscpp.ExampleManager.Planet1.xml
    --proxy=${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/generated_interfaces/proxy.hpp
    --adaptor=${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/generated_interfaces/adapter.hpp
  DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/submodules/sdbus-cpp/tools/sdbus-c++-xml2cpp
    ${PROJECT_SOURCE_DIR}/src/ipc/dbus_schema/org.sdbuscpp.ExampleManager.Planet1.xml
  COMMENT "Generating D-Bus interfaces for ${PROJECT_NAME}.")

add_custom_target(
  generate_sdbus_sources
  DEPENDS
    ${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/generated_interfaces/proxy.hpp
    ${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/generated_interfaces/adapter.hpp
)

set(SDBUS_INCLUDE_DIR ${RPOJECT_SOURCE_DIR}/submodules/sdbus-cpp/include/)
set(DBUS_GENERATED_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp src/device_reader/DeviceDataReader.cpp
  src/data_dump/DataDumper.cpp src/data_dump/SerializerBase.cpp
  src/data_dump/BytesSerializer.cpp)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Threads::Threads
  PRIVATE sdbus-c++)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${SDBUS_INCLUDE_DIR}
  PRIVATE ${DBUS_GENERATED_INCLUDE_DIR})

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE -Wall -Wextra -Wpedantic -Werror
  PRIVATE -fcoroutines)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(${PROJECT_NAME} PRIVATE -O2 -g -fno-omit-frame-pointer)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_custom_target(
  codeformat
  COMMAND clang-format
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Formatting source files...")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(STATUS "64-bit architecture detected")
else()
  message(STATUS "32-bit architecture detected")
endif()

add_dependencies(${PROJECT_NAME} generate_sdbus_sources)
