add_compile_options(-std=c++11)
if (UNIX)
    set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-parameter -std=c++11 -Wsuggest-override -Wno-strict-aliasing")
endif (UNIX)

file(GLOB_RECURSE SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/include/Core/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/Extensions/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Extensions/*.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${URHO3D_HOME}/include/Urho3D/ThirdParty/PugiXml)

add_library(spark STATIC ${SRC_FILES})
