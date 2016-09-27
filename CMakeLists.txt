cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("cs148-opengl4")

# Check system
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE(STATUS "64 bits compiler detected" )
    SET( EX_PLATFORM 64 )
    SET( EX_PLATFORM_NAME "x64" )
    SET( EX_PLATFORM_STR "64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    MESSAGE(STATUS "32 bits compiler detected" )
    SET( EX_PLATFORM 32 )
    SET( EX_PLATFORM_NAME "x86" )
    SET( EX_PLATFORM_STR "32" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

# Import utility cmake scripts...mainly useful for Linux systems since we provide
# the external libraries elsewhere.
if (NOT WIN32 AND NOT APPLE)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
endif()

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

# CPP Flags
if (NOT WIN32)
	set(CXX_FLAGS "-Wall -std=c++11 -Wno-missing-braces")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Common Files necessary for all assignments
include_directories("./")
file(GLOB_RECURSE COMMON_SOURCES "common/*.cpp")
file(GLOB_RECURSE COMMON_HEADERS "common/*.h")

# GLM Headers
include_directories("./external/glm")

# GLEW Headers
if (WIN32 OR APPLE)
    include_directories("./external/glew/include")
else()
    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})
endif()

# Open Asset Import Headers
include_directories("./external/assimp/include")

# FreeImage Header
if (WIN32)
    include_directories("./external/freeimage/distrib/windows/${EX_PLATFORM_NAME}")
elseif (APPLE)
    include_directories("./external/freeimage/distrib/osx")
else()
    find_package(FreeImage REQUIRED)
    include_directories(${FREEIMAGE_INCLUDE_DIRS})
endif()

# SDL Shenanigans
if (WIN32)
	include_directories("./external/SDL2/windows/include")
elseif (NOT APPLE)
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
endif()

# OpenGL Shenanigans
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE ASSIGNMENT_SOURCES "./assignment*/*.cpp")
file(GLOB_RECURSE ASSIGNMENT_HEADERS "./assignment*/*.h")

file(GLOB_RECURSE INSTRUCTOR_SOURCES "./instructor/*.cpp")
file(GLOB_RECURSE INSTRUCTOR_HEADERS "./instructor/*.h")

# Path to Shaders
add_definitions("-DSHADER_PATH=${CMAKE_CURRENT_SOURCE_DIR}/shaders")

# Path to Assets
add_definitions("-DASSET_PATH=${CMAKE_CURRENT_SOURCE_DIR}/assets")

# Note that file globbing is generally not recommended. It is usually better to explicitly list files; however,
# I am assuming that most students are unfamiliar with CMake and thus will not be able to update the CMakeLists.txt
# file appropriately. Hence, globbing is necessary for me to generate the Makefiles/whatever again should I ever
# want to run their code.
add_definitions(${CXX_FLAGS})
add_executable(cs148opengl4 main.cpp ${COMMON_SOURCES} ${COMMON_HEADERS}
    ${ASSIGNMENT_SOURCES} ${ASSIGNMENT_HEADERS} ${INSTRUCTOR_SOURCES} ${INSTRUCTOR_HEADERS})

# OpenGL
target_link_libraries(cs148opengl4 ${OPENGL_LIBRARIES})

# SDL2 Library
if (WIN32)
	target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2/windows/lib/${EX_PLATFORM_NAME}/SDL2.lib")
elseif (APPLE)
    target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2/osx/SDL2.framework")

    # Need to code sign for use with XCode!
    add_custom_command(TARGET cs148opengl4 POST_BUILD
        COMMAND codesign -f -s - "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2/osx/SDL2.framework")
else()
    target_link_libraries(cs148opengl4 ${SDL2_LIBRARY})
endif()

# GLEW Library
if (WIN32)
	target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/distrib/windows/lib/${EX_PLATFORM_NAME}/glew32.lib")
elseif (APPLE)
    target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/distrib/osx/lib/libGLEW.a")
else()
    target_link_libraries(cs148opengl4 ${GLEW_LIBRARIES})
endif()

# Open Asset Import Library
if (WIN32)
    target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/distrib/windows/lib${EX_PLATFORM_STR}/assimp.lib")
elseif (APPLE)
    target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/distrib/osx/libassimp.dylib")
else()
    target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/distrib/unix/libassimp.so")
endif()

# FreeImage Library
if (WIN32)
	target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/freeimage/distrib/windows/${EX_PLATFORM_NAME}/FreeImage.lib")
elseif (APPLE)
    target_link_libraries(cs148opengl4 "${CMAKE_CURRENT_SOURCE_DIR}/external/freeimage/distrib/osx/libfreeimage.a")
else()
    target_link_libraries(cs148opengl4 ${FREEIMAGE_LIBRARY})
endif()

# Source Group Folders for IDE Usage
# Shaders
source_group(basicColor REGULAR_EXPRESSION basicColor/.*)

source_group(brdf REGULAR_EXPRESSION brdf/.*)
source_group(brdf\\blinnphong REGULAR_EXPRESSION brdf/blinnphong/.*)
source_group(brdf\\blinnphong\\frag REGULAR_EXPRESSION brdf/blinnphong/frag/.*)
source_group(brdf\\blinnphong\\fragTexture REGULAR_EXPRESSION brdf/blinnphong/fragTexture/.*)
source_group(brdf\\blinnphong\\vert REGULAR_EXPRESSION brdf/blinnphong/vert/.*)
source_group(brdf\\blinnphong\\frag\\noSubroutine REGULAR_EXPRESSION brdf/blinnphong/frag/noSubroutine/.*)
source_group(brdf\\blinnphong\\fragTexture\\noSubroutine REGULAR_EXPRESSION brdf/blinnphong/fragTexture/noSubroutine/.*)
source_group(brdf\\blinnphong\\vert\\noSubroutine REGULAR_EXPRESSION brdf/blinnphong/vert/noSubroutine/.*)

source_group(required REGULAR_EXPRESSION required/.*)
source_group(required\\ REGULAR_EXPRESSION required/pass/.*)

# Source Files
source_group(common REGULAR_EXPRESSION common/.*)
source_group(common\\Rendering REGULAR_EXPRESSION common/Rendering/.*)
source_group(common\\Rendering\\Shaders REGULAR_EXPRESSION common/Rendering/Shaders/.*)
source_group(common\\Rendering\\Textures REGULAR_EXPRESSION common/Rendering/Textures/.*)
source_group(common\\Scene REGULAR_EXPRESSION common/Scene/.*)
source_group(common\\Scene\\Camera REGULAR_EXPRESSION common/Scene/Camera/.*)
source_group(common\\Scene\\Light REGULAR_EXPRESSION common/Scene/Light/.*)
source_group(common\\Utility REGULAR_EXPRESSION common/Utility/.*)
source_group(common\\Utility\\Mesh REGULAR_EXPRESSION common/Utility/Mesh/.*)
source_group(common\\Utility\\Mesh\\Loading REGULAR_EXPRESSION common/Utility/Mesh/Loading/.*)
source_group(common\\Utility\\Mesh\\Simple REGULAR_EXPRESSION common/Utility/Mesh/Simple/.*)
source_group(common\\Utility\\Texture REGULAR_EXPRESSION common/Utility/Texture/.*)

# Copy dlls
if (WIN32)
	add_custom_command(TARGET cs148opengl4 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/distrib/windows/bin${EX_PLATFORM_STR}/assimp.dll" "$<TARGET_FILE_DIR:cs148opengl4>")
	add_custom_command(TARGET cs148opengl4 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/external/freeimage/distrib/windows/${EX_PLATFORM_NAME}/FreeImage.dll" "$<TARGET_FILE_DIR:cs148opengl4>")
	add_custom_command(TARGET cs148opengl4 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/distrib/windows/bin/${EX_PLATFORM_NAME}/glew32.dll" "$<TARGET_FILE_DIR:cs148opengl4>")
	add_custom_command(TARGET cs148opengl4 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2/windows/lib/${EX_PLATFORM_NAME}/SDL2.dll" "$<TARGET_FILE_DIR:cs148opengl4>")
endif()
