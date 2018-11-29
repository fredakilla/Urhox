URHO3D_HOME = /home/fred/Dev/Urho3D/BUILD

QT -= core gui
TARGET = Urhox
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++14
CONFIG -= qt
CONFIG(debug, debug|release): DEFINES += _DEBUG

CONFIG(debug, debug|release): TARGET = Urhox
DESTDIR = $$PWD/../build

INCLUDEPATH += $${URHO3D_HOME}/include
INCLUDEPATH += $${URHO3D_HOME}/include/Urho3D/ThirdParty
INCLUDEPATH += ThirdParty

HEADERS += \
    ThirdParty/ImGui/imconfig.h \
    ThirdParty/ImGui/imgui.h \
    ThirdParty/ImGui/imgui_internal.h \
    Urhox/SystemUI/Console.h \
    Urhox/SystemUI/DebugHud.h \
    Urhox/SystemUI/SystemMessageBox.h \
    Urhox/SystemUI/SystemUI.h \
    Urhox/SystemUI/SystemUIEvents.h \
    Urhox/UrhoxEvents.h \
    Urhox/Urhox.h \
    ThirdParty/ImGuizmo/ImGuizmo.h \
    ThirdParty/ImGuizmo/ImSequencer.h

SOURCES += \
    ThirdParty/ImGui/imgui.cpp \
    ThirdParty/ImGui/imgui_demo.cpp \
    ThirdParty/ImGui/imgui_draw.cpp \
    ThirdParty/ImGui/imgui_widgets.cpp \
    Urhox/SystemUI/Console.cpp \
    Urhox/SystemUI/DebugHud.cpp \
    Urhox/SystemUI/SystemMessageBox.cpp \
    Urhox/SystemUI/SystemUI.cpp \
    ThirdParty/ImGuizmo/ImGuizmo.cpp \
    ThirdParty/ImGuizmo/ImSequencer.cpp


