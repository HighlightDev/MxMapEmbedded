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

add_executable(${PROJECT_NAME} src/main.cpp src/watchers/GpsDeviceWatcher.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

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
