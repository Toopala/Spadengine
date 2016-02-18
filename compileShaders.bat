::@ECHO OFF

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

rmdir /Q /S "./Shaders/Compiled/"

:: Build release shaders
cd Shaders
msbuild Shaders.vcxproj /target:Rebuild /property:Configuration=Release;Platform=Win32
cd ..

:: Lets create the glsl shaders for the

for %%f in ("./Shaders/Compiled/*") do (
	.\HLSLcc.exe -lang=es100 -in="./Shaders/Compiled/%%~f" -out="./Shaders/Compiled/%%~nf.glsl" -reflect="./Shaders/Compiled/%%~nf.reflect"
)
