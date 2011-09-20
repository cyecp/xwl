solution "xwl"
configurations { "debug", "release" }
	
targetOS = "unknown";

if _ACTION == "vs2005" or _ACTION == "vs2008" or _ACTION == "vs2010" then
	targetOS = "windows"
elseif _ACTION == "codeblocks" or _ACTION == "gmake" then
	targetOS = "linux"
elseif _ACTION == "xcode3" then
	targetOS = "macosx"
end

local target_folder = "lib"

function translate_platform( platform )
	if platform == "x32" then
		return "x86"
	end
	
	return platform
end

project "xwl"
	objdir "obj"
	uuid( "71CAA3FB-9077-7F4F-A0F5-54FD79A6A0F6" )
	platforms { "x32", "x64" }
	kind "StaticLib"
	language ("C")
	
	files
	{
		"src/**.c",	
		"src/**.h",	
		
	}

	excludes { "samples/**.c" }
	
	includedirs 
	{ 
		"include"
	}	
	
	if targetOS == "macosx" then
		files
		{
			"src/**.m"
		}
	end

	if targetOS == "windows" then
		defines { "WIN32", "UNICODE", baseDefines }
		links
		{
			"opengl32"
		}
	elseif targetOS == "linux" then
		defines { "LINUX=1", baseDefines }
		links
		{
			"Xinerama",
			"X11",
			"GL"
		}
	elseif targetOS == "macosx" then
		defines { "__MACH__", baseDefines }
		linkoptions
		{
			"-framework Cocoa",
			"-framework OpenGL"
		}
	end

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
	platforms{ "x32", "x64" }
	-- ConsoleApp, WindowedApp, SharedLib, or StaticLib
	kind "ConsoleApp"
	language ("C")
	baseDefines = {}
		
	files
	{
--[[
		"src/**.c",	
		"src/**.h",	
--]]
		"samples/**.c"
	}

	includedirs 
	{ 
		"include"
	}	
	
--[[
	if targetOS == "macosx" then
		files
		{
			"src/**.m"
		}
	end
--]]
	links
	{
		"xwl"
	}

	if targetOS == "windows" then
		defines { "WIN32", "UNICODE", baseDefines }
		links
		{
			"opengl32"
		}
	elseif targetOS == "linux" then
		defines { "LINUX=1", baseDefines }
		links
		{
			"Xinerama",
			"X11",
			"GL"
		}
	elseif targetOS == "macosx" then
		defines { "__MACH__", baseDefines }
		linkoptions
		{
			"-framework Cocoa",
			"-framework OpenGL"
		}
	end

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