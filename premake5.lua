include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Crystal"
	architecture "x86_64"
	startproject "Crystal-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
Includedir = {}
Includedir["GLFW"] = "%{wks.location}/Crystal/vendor/GLFW/include"
Includedir["Glad"] = "%{wks.location}/Crystal/vendor/Glad/include"
Includedir["ImGui"] = "%{wks.location}/Crystal/vendor/imgui"
Includedir["glm"] = "%{wks.location}/Crystal/vendor/glm"
Includedir["stb_image"] = "%{wks.location}/Crystal/vendor/stb_image"
Includedir["entt"] = "%{wks.location}/Crystal/vendor/entt/include"

group "Dependencies"
	include "vendor/premake"
	include "Crystal/vendor/GLFW"
	include "Crystal/vendor/Glad"
	include "Crystal/vendor/imgui"
group ""

include "Crystal"
include "Sandbox"
include "Crystal-Editor"
