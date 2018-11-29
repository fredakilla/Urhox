
include_directories (
	../ThirdParty
	${URHO3D_BUILD_DIR}/include
	${URHO3D_BUILD_DIR}/include/Urho3D/ThirdParty
)

file(GLOB_RECURSE SRC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/SystemUI/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/SystemUI/*.cpp
)

add_library(urhoxcore STATIC ${SRC_FILES})
