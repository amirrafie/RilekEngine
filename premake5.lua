-- https://premake.github.io/docs/

workspace "RilekEngine"
	architecture "x64"

	configurations
	{
	"Debug", 
	"Release"
	}
	
	startproject "RilekEngine"

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- includes directories relative to solution directory
IncludeDir = {}
IncludeDir["spdlog"] = "RilekEngine/Deps/spdlog/include"
IncludeDir["glfw"] = "RilekEngine/Deps/glfw/include"
IncludeDir["glad"] = "RilekEngine/Deps/glad/include"

include "RilekEngine/Deps/glfw"
include "RilekEngine/Deps/glad"

project "RilekEngine"
	location "RilekEngine"
	kind "WindowedApp"
	language "C++"

    cppdialect "C++17"
    systemversion "latest"
	flags
    {
        "MultiProcessorCompile"
    }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "stdafx.h"
	pchsource("%{prj.name}/Source/stdafx.cpp")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs 
	{
		"%{prj.name}/Source",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}"
	}
	
	links
	{
		"glfw",
		"opengl32.lib",
		"glad"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_WINDOWS"
	}

	filter "configurations:Debug"
	
		runtime "Debug"
		defines 
		{
			"_DEBUG"
		}

	filter "configurations:Release"

		
		defines 
		{
			"NDEBUG"
		}