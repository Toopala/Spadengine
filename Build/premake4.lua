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
		language "C++"
		files {"../ThirdParty/glad/Source/**.c"}
		includedirs {"../ThirdParty/glad/Include/"}
	
	project "Renderer"
		kind "StaticLib"
		language "C++"
		defines {"OPENGL4"}
		files { "../Renderer/**.cpp" }
		includedirs { "../Renderer/Include/",	
				"../Core/Include",
				"../ThirdParty/glad/Include/" }
		links { "SDL2", "glad"}


	project "HID"
		kind "StaticLib"
		language "C++"
		files { "../HID/**.cpp" }
		includedirs {"../HID/Include/",
				"../ThirdParty/glm/include/" }
		links { "SDL2" }


	project "Game"
		kind "StaticLib"
		language "C++"
		files {"../Game/**.cpp"}	
		includedirs { "../Game/Include/",
				"../Core/Include/",
				"../HID/Include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/SDL/include/" }
		links { "SDL2", "Core", "HID" }

	project "Spade"
		kind "StaticLib"
		language "C++"
		files { "../Spade/**.cpp" }
		includedirs { "../Spade/Include",
				"../Renderer/Include",
				"../Game/Include",
				"../ThirdParty/glm/include/",
				"../HID/Include"}
		links { "Renderer" }

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
		links { "SDL2", "Core", "Renderer", "glad","dl","assimp" }
