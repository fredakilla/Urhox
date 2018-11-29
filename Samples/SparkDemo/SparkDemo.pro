URHO3D_HOME = /home/fred/Dev/Urho3D/BUILD

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt c++14

QMAKE_CLEAN += $$DESTDIR/$$TARGET
DESTDIR = $$PWD/../../build

DEFINES += URHO3D_64BIT URHO3D_THREADING

INCLUDEPATH += $${URHO3D_HOME}/include
INCLUDEPATH += $${URHO3D_HOME}/include/Urho3D/ThirdParty

INCLUDEPATH += ../../Sources/Urhox
INCLUDEPATH += ../../Sources/ThirdParty

HEADERS += \

SOURCES += \
    src/main.cpp \

linux {

    PRE_TARGETDEPS += $${DESTDIR}/libUrhox.a
    LIBS += -L$${DESTDIR} -lUrhox
    LIBS += -L$${URHO3D_HOME}/lib -lUrho3D
    LIBS += -lXi -ldl  -lpthread -lGL -lGLU -lX11
}



