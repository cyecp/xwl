newoption {
	trigger = "raspberrypi",
	value = nil,
	description = "Enables RaspberryPi support, adds -DRASPBERRYPI=1, plus platform files."
}

function setup_raspberry_pi()
			print( "Compiling for raspberry pi" )
			defines
			{
				"RASPBERRYPI=1"
			}

			files
			{
				"src/platforms/rpi/**.c",
				"include/platforms/rpi/**.h"
			}

			libdirs
			{
				"/opt/vc/lib"
			}

			includedirs
			{
				"/opt/vc/include",
				"includes/platforms/rpi/"
			}

			links
			{
				"EGL",
				"GLESv2"
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
	kind "StaticLib"
	language ("C")
	
	files
	{
		"src/*.c",	
		"src/**.h",
		"include/**.h"
	}

	excludes { "samples/**.c" }
	
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

		if _OPTIONS["raspberrypi"] ~= nil then
			setup_raspberry_pi()
		else
			
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
		defines { "WIN32", "UNICODE", baseDefines }
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

		if _OPTIONS["raspberrypi"] ~= nil then
			setup_raspberry_pi()
		else
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
