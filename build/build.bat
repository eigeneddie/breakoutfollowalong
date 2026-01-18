@echo off

if not exist ..\build mkdir ..\build
pushd ..\build
del *.pbd > NUL 2> NUL
echo WAITING FOR PBD > lock.tmp

cl -nologo -Zi ..\code\win32_platform.c /link user32.lib gdi32.lib -incremental:no -opt:ref

del lock.tmp
del *.obj

popd