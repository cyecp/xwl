newoption {
	trigger = "rpi",
	value = nil,
	description = "Enables RaspberryPi support, adds -DRASPBERRYPI=1,-DEGL=1, plus platform files."
}

newoption {
	trigger = "with_egl",
	value = nil,
	description = "Enable EGL support, adds -DEGL=1, plus platform files."
}

if _OPTIONS["rpi"] then
	print( "Compiling for the RaspberryPi..." )
end

if _OPTIONS["with_egl"] then
	print( "Compiling with EGL..." )
end

function setup_raspberry_pi()
	defines
	{
		"RASPBERRYPI=1",
		"EGL=1",
	}

	files
	{
		"src/platforms/egl/**.c",
		"include/platforms/egl/**.h",
		"src/platforms/rpi/**.c",
		"include/platforms/rpi/**.h",
	}

	libdirs
	{
		"/opt/vc/lib"
	}

	includedirs
	{
		"/opt/vc/include",
		"includes/platforms/rpi/",
		"includes/platforms/egl/"
	}

	buildoptions
	{
		"-Wall"
	}

	flags
	{
		"ExtraWarnings",
	}

	links
	{
		"EGL",
		"GLESv2"
	}
end

function setup_egl()
	defines
	{
		"EGL=1"
	}

	files
	{
		"src/platforms/egl/**.c",
		"include/platforms/egl/**.h"
	}

	links
	{
		"EGL"
	}
end


solution "xwl"
configurations { "debug", "release" }

local target_folder = "lib"
function translate_platform( platform )
	if platform == "x32" then
		return "x86"
	elseif platform == "Native" then
		return "native"	
	end
	
	return platform
end

project "xwl"
	objdir "obj"
	uuid( "71CAA3FB-9077-7F4F-A0F5-54FD79A6A0F6" )
	platforms { "x32", "x64", "native" }
	kind "SharedLib"
	language ("C")
	
	files
	{
		"src/*.c",	
		"src/**.h",
		"include/**.h"
	}

	excludes { "samples/**.c", "src/ogl.c" }
	
	includedirs 
	{ 
		"include"
	}	
	
	configuration{ "linux" }
		defines { "LINUX=1", baseDefines }
		links
		{
			"Xinerama",
			"X11",
			"GL"
		}
		files
		{
			"src/platforms/x11/**.c",
			"include/platforms/x11/**.h"
		}

		if _OPTIONS["rpi"] ~= nil then
			setup_raspberry_pi()
		elseif _OPTIONS["with_egl"] ~= nil then
			setup_egl()
		end

	configuration{ "macosx" }
		files
		{
			"src/**.m"
		}
		defines { "__MACH__", baseDefines }
		linkoptions
		{
			"-framework Cocoa",
			"-framework OpenGL"
		}

	configuration{ "windows" }
		defines { "XWL_DLL=1", "UNICODE", baseDefines }
		links
		{
			"opengl32"
		}

	for _,platform in ipairs(platforms()) do

		configuration { "debug", platform }
			targetdir (target_folder .. "/" .. translate_platform( platform ) .. "/debug" )
			flags { "Symbols" }
			defines { "DEBUG" }
			
		configuration { "release", platform }
			targetdir (target_folder .. "/" .. translate_platform( platform ) .. "/release" )
			flags { "Optimize", "Symbols" }
	end

target_folder = "bin"
project "sample"
	objdir "obj"
	targetdir "build/bin"
	uuid( "D2302F80-6997-9346-8033-1FA0E461A6CE" )
	platforms{ "x32", "x64", "native" }
	-- ConsoleApp, WindowedApp, SharedLib, or StaticLib
	kind "ConsoleApp"
	language ("C")
	baseDefines = {}
		
	files
	{
		"samples/**.c"
	}

	includedirs 
	{ 
		"include"
	}	

	links
	{
		"xwl"
	}

	configuration{ "windows" }
		defines { "WIN32", "UNICODE", baseDefines }
		links
		{
			"opengl32"
		}

	configuration{ "linux" }
		defines { "LINUX=1", baseDefines }
		links
		{
			"Xinerama",
			"X11",
			"GL"
		}
		linkoptions
		{
			"-Wl,-rpath,."
		}
		
		if _OPTIONS["rpi"] ~= nil then
			setup_raspberry_pi()
		elseif _OPTIONS["with_egl"] ~= nil then
			setup_egl()
		end

	configuration{ "macosx" }
		defines { "__MACH__", baseDefines }
		linkoptions
		{
			"-framework Cocoa",
			"-framework OpenGL"
		}

	for _,platform in ipairs(platforms()) do
		
		configuration { "debug", platform }
			targetdir (target_folder .. "/" .. translate_platform( platform ) .. "/debug" )
			flags { "Symbols" }
			defines { "DEBUG" }
			libdirs { "lib/" .. translate_platform( platform ) .. "/debug" }
			
		configuration { "release", platform }
			targetdir (target_folder .. "/" .. translate_platform( platform ) .. "/release" )
			flags { "Optimize", "Symbols" }
			libdirs { "lib/" .. translate_platform( platform ) .. "/release" }
	end
