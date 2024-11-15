workspace "Tungsten"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	filter { "action:gmake2" }
		buildoptions
		{
			"-Wall",
			"-Wextra",
			"-pedantic",
			"-Wconversion",
			"-Wshadow",
			"-Wcast-align",
			"-Wformat=2",
			"-Wnon-virtual-dtor",
			"-Wnull-dereference",
			"-Wdeprecated-declarations",
			"-Wstrict-aliasing=2"
		}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "TungstenCore/vendor/GLFW"
	include "TungstenCore/vendor/Glad"
	include "libTungstenForge/vendor/yaml-cpp"
group ""

project "TungstenCore"
	location "TungstenCore"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "%{prj.name}/src/TungstenCore/wCorePCH.hpp"
	pchsource "%{prj.name}/src/TungstenCore/wCorePCH.cpp"

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/**.hpp",
		"%{prj.name}/vendor/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/mono/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/GLAD/include"
	}
	
	libdirs
	{
		"%{prj.name}/vendor/mono/lib/%{cfg.buildcfg}"
	}

	links
	{
		"libmono-static-sgen.lib",
		"GLFW",
		"Glad"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"W_PLATFORM_WINDOWS"
		}
		
		links
		{
			"Ws2_32.lib",
			"Winmm.lib",
			"Version.lib",
			"Bcrypt.lib"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines
		{
			"W_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines
		{
			"W_RELEASE"
		}

project "libTungstenForge"
	location "libTungstenForge"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "%{prj.name}/src/wForgePCH.hpp"
	pchsource "%{prj.name}/src/wForgePCH.cpp"

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor",
		"%{prj.name}/vendor/yaml-cpp/include",
		"TungstenCore/src",
		"TungstenCore/vendor",
		"TungstenCore/vendor/spdlog/include"
	}

	links
	{
		"TungstenCore",
		"yaml-cpp"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"W_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines
		{
			"W_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines
		{
			"W_RELEASE"
		}

project "TungstenForge"
	location "TungstenForge"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"libTungstenForge/src",
		"libTungstenForge/vendor",
		"libTungstenForge/vendor/yaml-cpp/include",
		"TungstenCore/src",
		"TungstenCore/vendor",
		"TungstenCore/vendor/spdlog/include"
	}
	
	links
	{
		"libTungstenForge",
		"TungstenCore",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"W_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines
		{
			"W_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines
		{
			"W_RELEASE"
		}

project "TungstenEditor"
	location "TungstenEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"TungstenCore/src",
		"TungstenCore/vendor",
		"TungstenCore/vendor/spdlog/include"
	}
	
	links
	{
		"TungstenCore",
		"libTungstenForge"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"W_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines
		{
			"W_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines
		{
			"W_RELEASE"
		}

project "TungstenRuntime"
	location "TungstenRuntime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"TungstenCore/src",
		"TungstenCore/vendor",
		"TungstenCore/vendor/spdlog/include"
	}
	
	links
	{
		"TungstenCore"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"W_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		defines
		{
			"W_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

		defines
		{
			"W_RELEASE"
		}
