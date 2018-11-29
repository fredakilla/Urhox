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

# Find URHO3D_HOME path
set(URHO3D_HOME CACHE PATH "Urho3D Build directory")
if ("${URHO3D_HOME}" STREQUAL "")
    message(FATAL_ERROR "URHO3D_HOME variable is empty. Please set Urho3D build directory path.")
endif()
message(STATUS "Urho build directory : ${URHO3D_HOME}")

# include shared cmake macro
include(CMake/global.cmake)

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


set(BUILD_TARGETS
    imgui
    imguizmo
    spark
    urhoxcore
)

# amalgamate libraries
if(LINUX)
    set(URHOX_LIB_NAME libUrhox.a)
    set(TARGET_CMD ${CMAKE_CURRENT_SOURCE_DIR}/CMake/gnu-amalgamate.sh
        ${CMAKE_AR}
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${OUT_DIR_LIB} ${URHOX_LIB_NAME}
    )
elseif(EMSCRIPTEN)
    set(URHOX_LIB_NAME libUrhox.a)
    set(TARGET_CMD ${CMAKE_CURRENT_SOURCE_DIR}/CMake/gnu-amalgamate.sh
        $(EMSCRIPTEN)/emar
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${OUT_DIR_LIB} ${URHOX_LIB_NAME}
    )
elseif(WINDOWS)
    set(URHOX_LIB_NAME Urhox.lib)
    set(TARGET_CMD ${CMAKE_CURRENT_SOURCE_DIR}/CMake/msvc-amalgamate.bat
        ${CMAKE_AR}
        ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} ${OUT_DIR_LIB} ${URHOX_LIB_NAME}
    )
endif()


add_custom_command(
    OUTPUT ${URHOX_LIB_NAME}
    COMMAND ${TARGET_CMD}
    #DEPENDS ${BUILD_TARGETS}
    COMMENT "Amalgamate external dependencies..."
)

add_custom_target(URHOX_LIB ALL DEPENDS ${URHOX_LIB_NAME})
add_dependencies(URHOX_LIB ${BUILD_TARGETS})


# Copy public headers to the output directory
COPY_HEADERS(Sources/ThirdParty/ImGui/ ${OUT_DIR_INCLUDE}/Urhox/ThirdParty/ImGui)
COPY_HEADERS(Sources/ThirdParty/ImGuizmo/ ${OUT_DIR_INCLUDE}/Urhox/ThirdParty/ImGuizmo)
COPY_HEADERS(Sources/ThirdParty/Spark/include/ ${OUT_DIR_INCLUDE}/Urhox/ThirdParty/Spark)
COPY_HEADERS(Sources/Urhox/ ${OUT_DIR_INCLUDE}/Urhox/Urhox)


