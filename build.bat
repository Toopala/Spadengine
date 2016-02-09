call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
call "C:\Program Files\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

msbuild /t:Rebuild Build/Spadengine.sln /property:Platform=Win32

if %ERRORLEVEL% NEQ 0 (
	EXIT 1
)
