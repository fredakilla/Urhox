set ROOTDIR=%CD%
set LIBDIR=%1
set OUTDIR=%2
set LIBNAME=%3

echo "Amalgamating target dir: %LIBDIR%"

md "%LIBDIR%"

cd %LIBDIR%
del %LIBNAME%
md tmp
xcopy *.lib tmp

LIB.EXE /OUT:%LIBNAME% tmp\*

:: move %LIBNAME% %OUTDIR%

rmdir /s /q "tmp\"
cd %ROOTDIR%
