workspace "Crystal"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
Includedir = {}
Includedir["GLFW"] = "Crystal/vendor/GLFW/include"
Includedir["Glad"] = "Crystal/vendor/Glad/include"
Includedir["ImGui"] = "Crystal/vendor/imgui"
Includedir["glm"] = "Crystal/vendor/glm"

include "Crystal/vendor/GLFW"
include "Crystal/vendor/Glad"
include "Crystal/vendor/Imgui"

project "Crystal"
	location "Crystal"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "crystalpch.h"
	pchsource "Crystal/src/crystalpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{Includedir.GLFW}",
		"%{Includedir.Glad}",
		"%{Includedir.ImGui}",
		"%{Includedir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CRYSTAL_PLATFORM_WINDOWS",
			"CRYSTAL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CRYSTAL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Crystal/vendor/spdlog/include",
		"Crystal/src",
		"Crystal/vendor",
		"Crystal/vendor/glm"
	}

	links
	{
		"Crystal"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CRYSTAL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CYSTAL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CYSTAL_DIST"
		runtime "Release"
		optimize "On"
