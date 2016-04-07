solution "Spadengine"
	
	defines {"OPENGL4"}
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
	
	project "freetype"
		kind "StaticLib"
		language "C++"
		files {"../ThirdParty/freetype/include/"}

--	project "bulletphysicssge"
--		kind "StaticLib"
--		language "C++"
--		files {"../ThirdParty/bulletphysics/include/",
--			"../ThirdParty/bulletphysics/include/Bullet"}

	project "Renderer"
		kind "StaticLib"
		language "C++"
		defines {"OPENGL4"}
		files { "../Renderer/**.cpp" }
		includedirs { "../Renderer/Include/",	
				"../Core/Include/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/glm/include/" }
		links { "SDL2", "glad"}


	project "HID"
		kind "StaticLib"
		language "C++"
		files { "../HID/**.cpp" }
		includedirs {"../HID/Include/",
				"../Core/Include/",
				"../ThirdParty/glm/include/" }
		links { "SDL2" }

	project "Spade"
		kind "StaticLib"
		language "C++"
		files { "../Spade/**.cpp" }
		includedirs { "../Spade/Include/",
				"../Renderer/Include/",
				"../Game/Include/",
				"../Core/Include/",
				"../Resources/Include/",
				"../ThirdParty/glm/include/",
				"../HID/Include/"}
		links { "Renderer" }

	project "Game"
		kind "StaticLib"
		language "C++"
		files {"../Game/**.cpp"}	
		includedirs { "../Game/Include/",
				"../Core/Include/",
				"../HID/Include/",
				"../Renderer/Include/",
				"../Resources/Include/",
				"../Spade/Include",
				"../ThirdParty/glm/include/",
				"../ThirdParty/SDL/include/" }
		links { "SDL2", "Core", "HID","Resources", "Spade" }


	project "Resources"
		kind "StaticLib"
		language "C++"
		files {"../Resources/**.cpp"}
		includedirs {"../Resources/Include/",
				"../Core/Include/",
				"../Renderer/Include/",
				"../Game/Include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/freetype/include/",
				"../ThirdParty/stb_image/Include/"}

	project "Audio"
		kind "StaticLib"
		language "C++"
		includedirs {"../Audio/Include/"}


	project "Sample"
		kind "ConsoleApp"
		language "C++"
		location "../Samples/Sample/"
		files {"../Samples/Sample/**.cpp"}
		includedirs {"../Core/Include/",
				"../Renderer/Include/",
				"../Game/Include/",
				"../Resources/Include/",
				"../ThirdParty/SDL/include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/stb_image/Include/",
				"../ThirdParty/assimp/include/",
				"../Samples/Sample/Include/"}
		links { "SDL2", "Core", "Renderer", "glad","dl","assimp", "Resources" }
	
	project "ECSample"
		kind "ConsoleApp"
		language "C++"
		location "../Samples/ECSample"
		files {"../Samples/RenderSample/**.cpp"}
		includedirs {"../Core/Include/",
				"../Renderer/Include/",
				"../Game/Include/",
				"../Resources/Include/",
				"../ThirdParty/SDL/include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/stb_image/Include/",
				"../ThirdParty/assimp/include/"}
		links {"Core", "Renderer", "Game", "Resources","SDL2","glad","dl","assimp"}

	project "RenderSample"
		kind "ConsoleApp"
		language "C++"
		location "../Samples/RenderSample/"
		files {"../Samples/RenderSample/**.cpp"}
		includedirs {"../Core/Include/",
				"../Renderer/Include/",
				"../Game/Include/",
				"../Resources/Include/",
				"../ThirdParty/SDL/include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/stb_image/Include/",
				"../ThirdParty/assimp/include/"}
		links {"Core", "Renderer", "Game", "Resources","SDL2","glad","dl","assimp"}

	project "SceneSample"
		kind "ConsoleApp"
		language "C++"
		location "../Samples/SceneSample/"
		files {"../Samples/SceneSample/**.cpp"}
		includedirs {"../Samples/SceneSample/Include/",
				"../Core/Include/",	
				"../Renderer/Include/",
				"../Game/Include/",
				"../Resources/Include/",
				"../Spade/Include/",
				"../HID/Include/",
				"../ThirdParty/SDL/include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/stb_image/Include/",
				"../ThirdParty/bulletphysics/include/",
				"../ThirdParty/bulletphysics/include/Bullet/LinearMath/",
				"../ThirdParty/assimp/include/"}
		links {"Core", "Renderer", "Game", "Resources","Spade","HID","SDL2","glad","dl","Bullet","assimp"}
	 
	
