solution "Spadengine"
	configurations { "DEBUG", "RELEASE" }

	configuration "Debug"
		defines { "DEBUG" }
		flags   { "Symbols" }

	configuration "Release"
		defines { "NDEBUG","RELEASE_BUILD" }
		flags   { "Optimize" }

	project "Core"
		kind "StaticLib"
		language "C++"
		files { "../Core/**.cpp" }
		includedirs { "../Core/Include/",
			      "../ThirdParty/glm/include/" }

	project "Renderer"
		kind "StaticLib"
		language "C++"
		files { "../Renderer/**.ccp" }
		includedirs { "../Renderer/Include/" }

	project "Sample"
		kind "WindowedApp"
		language "C++"
		files {"../Sample/Source/**.ccp" }
		includedirs { "../Sample/Include/"} 
