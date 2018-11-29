URHO3D_HOME = /home/fred/Dev/Urho3D/BUILD
URHOX_OUTPUT = $$PWD/../../output

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt c++14

QMAKE_CLEAN += $$DESTDIR/$$TARGET
DESTDIR = $$URHOX_OUTPUT/bin

DEFINES += URHO3D_64BIT URHO3D_THREADING

INCLUDEPATH += $${URHO3D_HOME}/include
INCLUDEPATH += $${URHO3D_HOME}/include/Urho3D/ThirdParty
INCLUDEPATH += $${URHOX_OUTPUT}/include/Urhox
INCLUDEPATH += $${URHOX_OUTPUT}/include/Urhox/ThirdParty

HEADERS += \

SOURCES += \
    src/main.cpp \

linux {

    PRE_TARGETDEPS += $$URHOX_OUTPUT/lib/liburhoxcore.a
    LIBS += -L$$URHOX_OUTPUT/lib/ -lurhoxcore -limgui -limguizmo -lspark
    LIBS += -L$${URHO3D_HOME}/lib -lUrho3D
    LIBS += -lXi -ldl  -lpthread -lGL -lGLU -lX11
}



