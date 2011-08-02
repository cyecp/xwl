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

project "xwl"
	objdir "obj"
	targetdir "build/bin"
	uuid( "CCD84535-0BFB-DC4A-AF85-4AADA85D998E" )
	platforms{ "x32", "x64" }
	-- ConsoleApp, WindowedApp, SharedLib, or StaticLib
	kind "ConsoleApp"
	language ("C")
	baseDefines = {}
	
	files
	{
		"src/**.c",	
		"src/**.h",	
		"samples/**.c"
	}

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

	configuration { "debug" }
		targetsuffix "d"		
		defines { "DEBUG" }
		flags { "Symbols" }
		
	configuration { "release" }
