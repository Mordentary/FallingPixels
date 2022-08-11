workspace "PixelSimulation"
architecture "x86_64"
startproject "PixelSimulation"

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
	IncludeDir = {}
	IncludeDir["Orion"] = "vendor/OrionEngine/Orion"

	group "Dependencies"
	externalproject "Orion"
		location "PixelSimulation/vendor/OrionEngine/Orion"
		uuid "57943020-8A99-EAB6-271F-61E0F7F6B73B"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
	
	group ""
		group "Dependencies/OrionsDependencies"
		include "PixelSimulation/vendor/OrionEngine/Orion/vendor/GLFW"
		include "PixelSimulation/vendor/OrionEngine/Orion/vendor/Glad"
		include "PixelSimulation/vendor/OrionEngine/Orion/vendor/imgui"
		group ""


project "PixelSimulation"

	location"PixelSimulation"
	kind"ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir ..  "/%{prj.name}")
	objdir("bin-int/" .. outputdir ..  "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/**.glsl"
	}

	includedirs
	{
		"PixelSimulation/vendor/OrionEngine/Orion/src",
		"PixelSimulation/vendor/OrionEngine/Orion/vendor/ImGui",
		"PixelSimulation/vendor/OrionEngine/Orion/vendor/glm",
		"PixelSimulation/vendor/OrionEngine/Orion/vendor/spdlog/include"
	}

	links
	{
		"Orion"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "ORI_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ORI_REALESE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "ORI_DIST"
		runtime "Release"
		optimize "on"
