solution {"Spadengine"}
	premake.gcc.cc = 'clang'
	premake.gcc.cxx = 'clang++'
	
	
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

-- THIRD PARTY LIBRARIES
	project "glad"
		kind "StaticLib"
		language "C++"
		files {"../ThirdParty/glad/Source/**.c"}
		includedirs {"../ThirdParty/glad/Include/"}
	
	project "freetype"
		kind "StaticLib"
		language "C++"
		files {"../ThirdParty/freetype/include/"}

	project "Bullet"
		kind "StaticLib"
		defines { "B3_USE_CLEW" }
		language "C++"
		files {"../ThirdParty/bulletphysics/**.cpp"}
		includedirs {"../ThirdParty/bulletphysics/include/",
				"../ThirdParty/bulletphysics/include/Bullet/"}



-- SPADE LIBRARIES
	project "Core"
		kind "StaticLib"
		language "C++"
		files { "../Core/**.cpp" }
		includedirs { "../Core/Include/",
				"../Core/Include/Core/Memory/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/SDL/include/" }
		links {"SDL2"}

	project "Renderer"
		kind "StaticLib"
		language "C++"
		files { "../Renderer/**.cpp" }
		includedirs { "../Renderer/Include/",	
				"../Core/Include/",
				"../ThirdParty/glm/include/", 
				"../ThirdParty/glad/Include/",
				"../ThirdParty/SDL/include/"}
		links {"Core","glad", "SDL2"}


	project "HID"
		kind "StaticLib"
		language "C++"
		files { "../HID/**.cpp" }
		includedirs {"../HID/Include/",
				"../Core/Include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/SDL/include/" }
		links {"Core", "SDL2" }

	project "Game"
		kind "StaticLib"
		language "C++"
		files {"../Game/**.cpp"}	
		includedirs { "../Game/Include/",
				"../Core/Include/",
				"../Core/Include/Core/Memory/",
				"../HID/Include/",
				"../Renderer/Include/",
				"../Resources/Include/",
				"../Spade/Include/",
				"../ThirdParty/freetype/include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/stb_image/Include/",
				"../ThirdParty/assimp/include/",
				"../ThirdParty/SDL/include/" }
		links { "Core", "HID", "Renderer", "Resources", "Assimp", "SDL2"}


	project "Spade"
		kind "StaticLib"
		language "C++"
		files { "../Spade/**.cpp" }
		includedirs { "../Spade/Include/",
				"../Core/Include/",
				"../Game/Include/",
				"../HID/Include/",
				"../Renderer/Include/",
				"../Resources/Include/",
				"../ThirdParty/assimp/include/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/SDL/include/"}
		links { "Core", "Game", "Renderer", "Resources", "Assimp", "glad", "SDL2"}

	project "Resources"
		kind "StaticLib"
		language "C++"
		files {"../Resources/**.cpp"}
		includedirs {"../Resources/Include/",
				"../Core/Include/",
				"../Renderer/Include/",
				"../ThirdParty/assimp/include/",
				"../ThirdParty/freetype/include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/SDL/include/",
				"../ThirdParty/stb_image/Include/"}
		links { "Core", "Renderer", "Assimp", "glad", "SDL2"}

	project "Audio"
		kind "StaticLib"
		language "C++"
		includedirs {"../Audio/Include/"}

-- SPADE SAMPLES
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

	project "PowerSample"
		kind "ConsoleApp"
		language "C++"
		files {"../Samples/PowerSample/**.cpp"}
		includedirs {"../Samples/PowerSample/Include/",
				"../Core/Include/",
				"../Game/Include/",
				"../HID/Include",
				"../Resources/Include/",
				"../Renderer/Include/",
				"../Spade/Include/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/SDL/include/",
				"../ThirdParty/stb_image/Include/"}		
		links {"Spade","Renderer","HID","Game","Resources","Core","assimp","glad", "dl", "SDL2"}

	project "SceneSample"
		kind "ConsoleApp"
		language "C++"
		location "../Samples/SceneSample/"
		files {"../Samples/SceneSample/**.cpp"}
		includedirs {"../Samples/SceneSample/Include/",
				"../Core/Include/",	
				"../Game/Include/",
				"../HID/Include/",
				"../Renderer/Include/",
				"../Resources/Include/",
				"../Spade/Include/",
				"../ThirdParty/assimp/include/",
				"../ThirdParty/bulletphysics/include/",
				"../ThirdParty/bulletphysics/include/Bullet/",
				"../ThirdParty/glad/Include/",
				"../ThirdParty/glm/include/",
				"../ThirdParty/SDL/include/",
				"../ThirdParty/stb_image/Include/"}
		links {"Spade","Renderer","HID","Game","Resources","Core","assimp","glad", "dl", "SDL2","Bullet"}
	 
	
