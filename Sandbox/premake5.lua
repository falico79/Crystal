project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Crystal/vendor/spdlog/include",
		"%{wks.location}/Crystal/src",
		"%{wks.location}/Crystal/vendor",
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
