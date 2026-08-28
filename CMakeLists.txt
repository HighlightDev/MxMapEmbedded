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

message(STATUS "*******************************************")
message(STATUS "*           MxMAP CONFIGURATION           *")
message(STATUS "*******************************************")

target_compile_features(sdbus-c++-objlib PRIVATE cxx_std_20)
target_compile_features(sdbus-c++ PRIVATE cxx_std_20)

target_compile_options(sdbus-c++-objlib PRIVATE -fcoroutines)
target_compile_options(sdbus-c++ PRIVATE -fcoroutines)

set(DBUS_XML_SCHEMA_PROXY_SOURCES org.bluez.LEAdvertisingManager1.xml
                                  org.bluez.GattManager1.xml)

set(DBUS_XML_SCHEMA_ADAPTOR_SOURCES
    org.bluez.LEAdvertisement1.xml org.bluez.GattCharacteristic1.xml
    org.bluez.GattService1.xml)

list(APPEND DBUS_GENERATED_SOURCES "")

message("DBus xml schema sources: ")
foreach(FILE IN LISTS DBUS_XML_SCHEMA_PROXY_SOURCES)
  message(STATUS "${FILE}")
  string(REGEX REPLACE "\\.[^.]*$" "" FILE_WITHOUT_EX ${FILE})

  set(PROXY_SOURCE_ABS_PATH
      "${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/generated_interfaces/${FILE_WITHOUT_EX}_proxy.hpp"
  )
  list(APPEND DBUS_GENERATED_SOURCES "${PROXY_SOURCE_ABS_PATH}")

  add_custom_command(
    OUTPUT "${PROXY_SOURCE_ABS_PATH}"
    COMMAND $<TARGET_FILE:sdbus-c++-xml2cpp>
            "${PROJECT_SOURCE_DIR}/src/ipc/dbus_schema/${FILE}"
            --proxy="${PROXY_SOURCE_ABS_PATH}"
    DEPENDS sdbus-c++-xml2cpp
            "${PROJECT_SOURCE_DIR}/src/ipc/dbus_schema/${FILE}"
    COMMENT "Generating Proxy D-Bus interfaces for ${PROJECT_NAME}.")
endforeach()

foreach(FILE IN LISTS DBUS_XML_SCHEMA_ADAPTOR_SOURCES)
  message(STATUS "${FILE}")
  string(REGEX REPLACE "\\.[^.]*$" "" FILE_WITHOUT_EX ${FILE})

  set(ADAPTER_SOURCE_ABS_PATH
      "${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/generated_interfaces/${FILE_WITHOUT_EX}_adapter.hpp"
  )
  list(APPEND DBUS_GENERATED_SOURCES "${ADAPTER_SOURCE_ABS_PATH}")

  add_custom_command(
    OUTPUT "${ADAPTER_SOURCE_ABS_PATH}"
    COMMAND $<TARGET_FILE:sdbus-c++-xml2cpp>
            "${PROJECT_SOURCE_DIR}/src/ipc/dbus_schema/${FILE}"
            --adapter="${ADAPTER_SOURCE_ABS_PATH}"
    DEPENDS sdbus-c++-xml2cpp
            "${PROJECT_SOURCE_DIR}/src/ipc/dbus_schema/${FILE}"
    COMMENT "Generating Adaptor D-Bus interfaces for ${PROJECT_NAME}.")
endforeach()

add_custom_target(generate_sdbus_sources DEPENDS ${DBUS_GENERATED_SOURCES})

set(SDBUS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/submodules/sdbus-cpp/include/)
set(DBUS_GENERATED_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/ipc/dbus_generated/)

set(SOURCES
    src/main.cpp
    src/device_reader/DeviceDataReader.cpp
    src/data_dump/DataDumper.cpp
    src/data_dump/SerializerBase.cpp
    src/data_dump/BytesSerializer.cpp
    src/ipc/bluetooth/proxies/LEAdvertisingManager1Proxy.cpp
    src/ipc/bluetooth/proxies/ObjectManagerProxy.cpp
    src/ipc/bluetooth/proxies/GattManager1Proxy.cpp
    src/ipc/bluetooth/adapters/LEAdvertisement1Adapter.cpp
    src/ipc/bluetooth/adapters/ObjectManagerAdapter.cpp
    src/ipc/bluetooth/adapters/GpsDataGattServiceAdapter.cpp
    src/ipc/bluetooth/adapters/GeolocationGattCharacteristicAdapter.cpp)

set(HEADERS
    src/device_reader/DeviceDataReader.hpp
    src/data_dump/DataDumper.hpp
    src/data_dump/SerializerBase.hpp
    src/data_dump/BytesSerializer.hpp
    src/ipc/bluetooth/proxies/LEAdvertisingManager1Proxy.hpp
    src/ipc/bluetooth/proxies/ObjectManagerProxy.hpp
    src/ipc/bluetooth/proxies/GattManager1Proxy.hpp
    src/ipc/bluetooth/adapters/LEAdvertisement1Adapter.hpp
    src/ipc/bluetooth/adapters/ObjectManagerAdapter.hpp
    src/ipc/bluetooth/adapters/GpsDataGattServiceAdapter.hpp
    src/ipc/bluetooth/adapters/GeolocationGattCharacteristicAdapter.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})

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
  COMMAND clang-format -i -style=file ${SOURCES} ${HEADERS}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Formatting source files...")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(STATUS "64-bit architecture detected")
else()
  message(STATUS "32-bit architecture detected")
endif()

add_dependencies(${PROJECT_NAME} generate_sdbus_sources)
