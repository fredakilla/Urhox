
include_directories (
	../ThirdParty
	${URHO3D_HOME}/include
	${URHO3D_HOME}/include/Urho3D/ThirdParty
        ${OUT_DIR_INCLUDE}/Urhox/ThirdParty
)

file(GLOB_RECURSE SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/SystemUI/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/SystemUI/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Spark/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Spark/*.cpp
)

add_library(urhoxcore STATIC ${SRC_FILES})
