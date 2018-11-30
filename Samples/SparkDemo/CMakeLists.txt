project(SparkDemo)

set(GAME_NAME ${PROJECT_NAME})
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

set(GAME_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

add_executable(${GAME_NAME} ${GAME_SRC})
target_link_libraries(${GAME_NAME} ${URHOX_LIBRARIES})
add_dependencies(${GAME_NAME} URHOX_LIB)

if(EMSCRIPTEN)
    SET_TARGET_PROPERTIES(${GAME_NAME} PROPERTIES LINK_FLAGS "--preload-file ${OUT_DIR_BIN}/emdata@/ --bind")
endif()

if(MSVC)
    set_target_properties(${GAME_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUT_DIR_BIN}")
endif()

