cmake_minimum_required(VERSION 3.1)
project(UrhoxLib)

set(CMAKE_CXX_STANDARD 14)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(URHO3D_HOME CACHE PATH "Urho3D Build directory")
if ("${URHO3D_HOME}" STREQUAL "")
	message(FATAL_ERROR "URHO3D_HOME variable is empty. Please set Urho3D build directory path.")
endif()
message(STATUS "Urho build directory : ${URHO3D_HOME}")

include(global.cmake)

# set global settings
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_DEBUG_POSTFIX "")

# set output directories
set(OUT_DIR_BIN ${CMAKE_SOURCE_DIR}/output/bin)
set(OUT_DIR_LIB ${CMAKE_SOURCE_DIR}/output/lib)
set(OUT_DIR_INCLUDE ${CMAKE_SOURCE_DIR}/output/include)

# set cmake output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR_LIB})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR_BIN})
if(WINDOWS)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()


add_subdirectory(Sources)
#add_subdirectory(Samples)


# Copy public headers to output directory
COPY_HEADERS(Sources/ThirdParty/ImGui/ ${OUT_DIR_INCLUDE}/Urhox/ThirdParty/ImGui)
COPY_HEADERS(Sources/ThirdParty/ImGuizmo/ ${OUT_DIR_INCLUDE}/Urhox/ThirdParty/ImGuizmo)
COPY_HEADERS(Sources/ThirdParty/Spark/include/ ${OUT_DIR_INCLUDE}/Urhox/ThirdParty/Spark)
COPY_HEADERS(Sources/Urhox/ ${OUT_DIR_INCLUDE}/Urhox/Urhox)


