cmake_minimum_required(VERSION 3.1)

include_directories (../../ThirdParty)

set(IMGUIZMO_HEADERS
	ImGuizmo.h
	ImSequencer.h
)

set(IMGUIZMO_SRC
	ImGuizmo.cpp
	ImSequencer.cpp
)

add_library(imguizmo STATIC ${IMGUIZMO_SRC})



