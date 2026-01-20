# WORK IN PROGRESS GAME FROM SCRATCH

Mastering C by making a fully shippable computer game from scratch!
Will make my own assets and art


The code is compiled using a batch file `build.bat` that runs the `cl` command from Microsoft Visual Studios that is set up using `vcvarsall.bat`.

The batch file `vcvarsall.bat` is obtained from installing the community version. Choose the module "Desktop Development with C++."
Every time you want to build the code using `build.bat` on the command prompt, you must run `vcvarsall.bat` to enable the `cl` command. 
The location of `vcvarsall.bat` may be different if you installed Microsoft Visual Studios differently, but generally the following prompt should work off the bat.

```
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```
Unlike mainstream software development, we are compiling everything from scratch every time. No build tools like CMake or Ninja. 