cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
# find_package( OpenCV REQUIRED )

set(CMAKE_C_FLAGS "-std=c99")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
# set(CMAKE_CXX_FLAGS "-std=c++11" )
# set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# set(CMAKE_PREFIX_PATH "C:/download/OpenCV/freeglut-3.2.1/build/FreeGLUT")

# find_library(libs FreeGLUT)
# find_package(FreeGLUT REQUIRED)

# if (NOT FreeGLUT_FOUND)
# message(FATAL_ERROR "FreeGLUT Not Found!")
# else(ADD_FOUND)
# message(STATUS "FreeGLUT Found!")
# message(----> ${FreeGLUT_LIBRARIES})
# message(----> ${FreeGLUT_LIBS})
# endif (NOT FreeGLUT_FOUND)

# include_directories(${FreeGLUT_INCLUDE_DIRS})
# add_executable(demo demo.cpp)
# target_link_libraries(demo ${FreeGLUT_LIBRARIES})

include_directories(
    "C:/download/OpenCV/redbook/src"
    "C:/download/OpenCV/freeglut-3.2.1/include/"
    "C:/download/OpenCV/freeglut-3.2.1/include/GL/"

    "C:/download/OpenCV/glfw-3.3.2/deps/"
    "C:/download/OpenCV/glfw-3.3.2/include/"
    "C:/download/OpenCV/glfw-3.3.2.bin.WIN64/include/"
    
    "C:/download/OpenCV/glew-2.1.0/include/"
    "C:/download/OpenCV/glew-2.1.0/include/GL/"
)
link_directories(
    "C:/download/OpenCV/freeglut-3.2.1/build/lib/"

    "C:/download/OpenCV/glew-2.1.0/lib/Release/x64/"

    "C:/download/OpenCV/glfw-3.3.2.bin.WIN64/lib-mingw-w64/"
)

# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# Static Libs
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
# set(BUILD_SHARED_LIBS OFF)

message(STATUS)
message("COMPILER LIST:")
message(STATUS)
# set(ENV{PATH} C:/MinGW/bin)
# message($ENV{PATH})
execute_process(COMMAND where g++ )
execute_process(COMMAND where make )

set(OpenGL_LIBS 
    # glut
    # glut32
    freeglut
    # freeglut_static
    glu32
    opengl32
    # winmm
    glew32
    glfw3dll
    glfw3
)

macro(add_demo name)
    add_executable( ${name} ${ARGN} )
    target_link_libraries( ${name} ${OpenGL_LIBS} )
endmacro(add_demo)

message(STATUS)
message("DEMOS LIST:")
message(STATUS)
# aux_source_directory("src/" src)
# foreach(item ${src})
#     string(REGEX REPLACE ".*/\(.*\)" "\\1" name ${item})
#     message(STATUS ${name})
#     add_demo(${name} ${item})
# endforeach(item)

aux_source_directory("demos/" demos)
foreach(item ${demos})
    string(REGEX REPLACE ".*/\(.*\)" "\\1" name ${item})
    message(STATUS ${name})
    add_demo(${name} ${item})
endforeach(item)

