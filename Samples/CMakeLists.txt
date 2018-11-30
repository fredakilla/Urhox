# shared configuration for examples

set(CMAKE_CXX_STANDARD 14)

include_directories(
    ${URHO3D_HOME}/include
    ${URHO3D_HOME}/include/Urho3D/ThirdParty
    ${OUT_DIR_INCLUDE}/Urhox
    ${OUT_DIR_INCLUDE}/Urhox/ThirdParty
)

link_directories(
    ${OUT_DIR_LIB}
    ${URHO3D_HOME}/lib
)

if(LINUX)

    add_definitions(-D__linux__)

    set(URHOX_LIBRARIES
        Urhox
        Urho3D
        Xi
        dl
        pthread
        GL
        GLU
        X11
        )

elseif(WINDOWS)

    set(URHOX_LIBRARIES
        Urhox
        Urho3D
        opengl32
        kernel32
        user32
        winmm
        gdi32
        winspool
        comdlg32
        advapi32
        shell32
        imm32
        ole32
        oleaut32
        uuid
        version
        odbc32
        odbccp32
        )

elseif(EMSCRIPTEN)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2  -s USE_FREETYPE=1 -s TOTAL_MEMORY=268435456")
        #-s DISABLE_EXCEPTION_CATCHING=0
        #-s DEMANGLE_SUPPORT=1
        #-s SAFE_HEAP=1 --bind
        #-s WASM=1
        #-s TOTAL_MEMORY=268435456
        #-s ALLOW_MEMORY_GROWTH=1
        #-s FULL_ES2=1

    set(URHOX_LIBRARIES
        Urhox
        Urho3D
    )

endif()

add_subdirectory(ImguiDemo)
add_subdirectory(SparkDemo)
