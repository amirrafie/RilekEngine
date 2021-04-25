workspace "RilekEngine"
	architecture "x64"
	configuration 
	{
	"Debug", 
	"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "RilekEngine"
	location "RilekEngine"
	kind "WindowedApp"
	language "C++"

	cppdialect "C++17"
    systemversion "latest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "stdafx.h"
	pchsource("%{prj.name}/Source/stdafx.cpp")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	include
	{
		"%{prj.name}/Source/",
		"%{prj.name}/Deps/spdlog/include"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_WINDOWS"
	}

	filter "configuration:Debug"
		defines 
		{
			"_DEBUG"
		}

	filter "configuration:Release"
		defines 
		{
			"NDEBUG"
		}