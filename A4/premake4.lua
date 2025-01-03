#!lua

includeDirList = {
    "../shared",
    "../shared/include",
    "../shared/gl3w",
    "../shared/imgui",
    "../shared/glfw-3.3.8/include",
}

libDirectories = { 
    "../lib",
    "../shared/glfw-3.3.8/build/src",
}


if os.get() == "macosx" then
    linkLibs = {
        "cs488-framework",
        "imgui",
        "glfw3",
        "lua",
		"lodepng"
    }
end

if os.get() == "linux" then
    linkLibs = {
        "cs488-framework",
        "lua",
        "lodepng",
        "stdc++",
        "dl",
        "pthread"
    }
end

if os.get() == "windows" then
    linkLibs = {
        "cs488-framework",
        "imgui",
        "glfw3",
        "opengl32",
        "gdi32",
        "user32",
        "kernel32",
        "shell32",
        "Imm32",
        "lua",
        "lodepng"
    }
    table.insert(includeDirList, "C:/projects/vcpkg/installed/x64-windows/include")
    table.insert(libDirectories, "C:/projects/vcpkg/installed/x64-windows/lib")
end

if os.get() == "macosx" then
    linkOptionList = { "-framework IOKit", "-framework Cocoa", "-framework CoreVideo", "-framework OpenGL" }
end

buildOptions = {"-std=c++14"}

solution "CS488-Projects"
    configurations { "Debug", "Release" }

    project "A4"
        kind "ConsoleApp"
        language "C++"
        location "build"
        objdir "build"
        targetdir "."
        buildoptions (buildOptions)
        libdirs (libDirectories)
        links (linkLibs)
        linkoptions (linkOptionList)
        includedirs (includeDirList)
        files { "*.cpp" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }
