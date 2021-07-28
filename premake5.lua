workspace "Crystal"
	architecture "x86_64"
	startproject "Crystal-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
Includedir = {}
Includedir["GLFW"] = "Crystal/vendor/GLFW/include"
Includedir["Glad"] = "Crystal/vendor/Glad/include"
Includedir["ImGui"] = "Crystal/vendor/imgui"
Includedir["glm"] = "Crystal/vendor/glm"
Includedir["stb_image"] = "Crystal/vendor/stb_image"
Includedir["entt"] = "Crystal/vendor/entt/include"

group "Dependencies"
	include "Crystal/vendor/GLFW"
	include "Crystal/vendor/Glad"
	include "Crystal/vendor/Imgui"
group ""

project "Crystal"
	location "Crystal"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "crystalpch.h"
	pchsource "Crystal/src/crystalpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{Includedir.GLFW}",
		"%{Includedir.Glad}",
		"%{Includedir.ImGui}",
		"%{Includedir.stb_image}",
		"%{Includedir.glm}",
		"%{Includedir.entt}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CRYSTAL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		systemversion "latest"

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CYSTAL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CYSTAL_DIST"
		runtime "Release"
		optimize "on"

project "Crystal-Editor"
	location "Crystal-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		"%{Includedir.glm}",
		"%{Includedir.entt}"
	}

	links
	{
		"Crystal"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CYSTAL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CYSTAL_DIST"
		runtime "Release"
		optimize "on"
