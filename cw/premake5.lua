projectName = "CW1_Starter"

workspace "CW1_Starter"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

project "CW1_Starter"
	location "%{prj.name}"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/")
	objdir ("build/")
	debugdir "./%{prj.name}"
	
	files
	{
		"./%{prj.name}/include/**.h",
		"./%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"./%{prj.name}/include/",
		"../../vendor/Box2D/",
		"../../vendor/SFML-2.5.1/include"
	}

	includedirs
	{
		"include/",
		"../../vendor/Box2D/",
		"../../vendor/SFML-2.5.1/include"
	}

	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		defines "SFML_STATIC"
		
		libdirs 
		{
			"../../vendor/Box2D/x64/Debug",
			"../../vendor/SFML-2.5.1/lib"
		}
		
		links
		{
			"Box2D",
			"sfml-main",
			"sfml-graphics-s-d",
			"sfml-system-s-d",
			"sfml-window-s-d",
			"sfml-audio-s-d",
			"opengl32.lib",
			"winmm.lib",
			"gdi32.lib",
			"openal32.lib",
			"flac.lib",
			"vorbisenc.lib",
			"vorbisfile.lib",
			"vorbis.lib",
			"ogg.lib",
			"freetype.lib"
		}
		
		debugenvs {"PATH=$PATH;../../../vendor/SFML-2.5.1/bin;"}

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		defines "SFML_STATIC"
		
		libdirs 
		{
			"../../vendor/Box2D/x64/Release",
			"../../vendor/SFML-2.5.1/lib"
		}
		
		links
		{
			"Box2D",
			"sfml-main",
			"sfml-graphics-s",
			"sfml-system-s",
			"sfml-window-s",
			"sfml-audio-s",
			"opengl32.lib",
			"winmm.lib",
			"gdi32.lib",
			"openal32.lib",
			"flac.lib",
			"vorbisenc.lib",
			"vorbisfile.lib",
			"vorbis.lib",
			"ogg.lib",
			"freetype.lib"
		}
		
		debugenvs {"PATH=$PATH;../../../vendor/SFML-2.5.1/bin;"}
		
			-- Clean Function --
newaction {
   trigger     = "clean",
   description = "clean the build",
   execute     = function ()
      os.rmdir("./build")
	  os.rmdir("./bin")
	  os.remove(projectName .. "/" .. projectName .. ".vcxproj")
	  os.remove(projectName .. "/" .. projectName .. ".vcxproj.user")
	  os.remove(projectName .. "/" .. projectName .. ".vcxproj.filters")
	  os.remove("*.sln")
   end
}