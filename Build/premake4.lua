solution "Spadengine"
	configurations { "DEBUG", "RELEASE" }

	configuration "Debug"
		defines { "DEBUG" }
		flags   { "Symbols" }
		buildoptions { "-std=c++11" }

	configuration "Release"
		defines { "NDEBUG","RELEASE_BUILD" }
		flags   { "Optimize" }
		buildoptions { "-std=c++11" }

	project "Core"
		kind "StaticLib"
		language "C++"
		files { "../Core/**.cpp" }
		includedirs { "../Core/Include/",
			      "../ThirdParty/glm/include/" }

	project "glad"
		kind "StaticLib"
		language "C"
		files {"../ThirdParty/glad/**.c"}
		includedirs {"../ThirdParty/glad/Include/"}
	
	project "Renderer"
		kind "StaticLib"
		language "C++"
		defines {"OPENGL4"}
		files { "../Renderer/**.cpp" }
		linkoptions {"../Build/libglad.a"}
		links { "SDL2"}
		includedirs { "../Renderer/Include/",	
				"../Core/Include",
				"../ThirdParty/glad/Include/" }

	project "Game"
		kind "StaticLib"
		language "C++"
		files {"../Game/**.cpp"}	
		includedirs { "../Game/Include/"}

	project "Sample"
		kind "ConsoleApp"
		language "C++"
		files {"../Samples/Sample/**.cpp"}
		includedirs {"../Core/Include/",
				"../Renderer/Include/",
				"../Game/Include/",
				"../ThirdParty/SDL/include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/stb_image/Include/",
				"../ThirdParty/assimp/include/",
				"../Samples/Sample/Include/"}
		links { "SDL2", "Core", "Renderer", "glad", "dl", "assimp" }
