cmake_minimum_required(VERSION 3.1)


set(IMGUI_HEADERS
	imconfig.h
	imgui.h
	imgui_internal.h
)

set(IMGUI_SRC
	imgui.cpp
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_widgets.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})



