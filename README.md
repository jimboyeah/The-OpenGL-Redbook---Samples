# Original README

These are the example programs which are featured in the OpenGL
Programming Guide, Version 1.1.  To compile these programs, you
need OpenGL development libraries for your machine and Mark Kilgard's
GLUT (Graphics Library Utility Toolkit). 

There is a simple Imakefile in this directory, which can be used to 
create a Makefile to compile the example programs.  There is also 
a simple Makefile, which can be used to compile the programs
(in case you don't support imake, or you just don't want to be
bothered).  There is also a Makefile.win, which has been tested
on Windows-based PCs.

When using either the Imakefile or the Makefile,
you will probably need to modify some of the variables inside the
files to make sure that GLUT headers and libraries are correctly
included and linked.

The Imakefile should generate a good Makefile with a simple
command such as:
% imake -DUseInstalled -I/usr/lib/X11/config

There are a handful of modifications from the code which is
printed in the OpenGL Programming Guide.  Most noticeably, every
program (except for hello.c and double.c, which are featured in
the first chapter) has a keyboard callback function to exit the
program when the ESCape key is pressed.

* Changes between this directory and the examples printed in the book

Also, after the book went to press, a couple of discrepancies
between the UNIX and MS Windows compilers became noticeable, requiring
changes to the source code.  Most obviously, all PC programs had to
include <windows.h> before <gl.h> or <glu.h>.  GLUT already does
this in the correct order, so the simplest solution was to only
include <GL/glut.h> and to let it include (if needed) the windows.h,
gl.h, and glu.h files, in the correct order.

The torus.c program refers to a symbolic constant M_PI, which may
not be found on MS Windows based systems.  A new constant PI_ 
has replaced it and is defined within the program.

Several programs use callback functions.  The method of casting
those callback functions worked fine on UNIX based systems, but
not on PCs.  The programs quadric.c, surface.c, trim.c, tess.c,
and tesswind.c have been modified.  References to (GLvoid (*))
cast have been removed, and a reference to the CALLBACK type 
has been added to the declaration of the callback functions.
Where CALLBACK is undefined (for instance, in UNIX systems),
it is stubbed out by use of #ifndef.

The programs accpersp.c and dof.c used the variable names "near"
and "far" which are reserved words for PC compilers.  If needed,
these variables names are now redefined during pre-processing 
(by using #ifdef).

* OpenGL 1.0 to 1.1 compatibility issues

Most of these programs also run well on OpenGL 1.0.  There are
nine programs which use features not found in OpenGL 1.0.  The 
four programs checker.c, mipmap.c, texgen.c, and wrap.c use
texture objects, and have been modified so that they will avoid
the use of texture objects on OpenGL 1.1 machines.  So these
programs will still run well on OpenGL 1.0.

The five programs polyoff.c, texbind.c, texprox.c, texsub.c,
and varray.c demonstrate features which are new in OpenGL 1.1.
On OpenGL 1.0, these five programs will not run; instead an error
message will be printed out.  If your implementation of OpenGL 1.0
supports polygon offset, vertex array, and/or texture extensions,
you may be able to modify the code to run on your implementation.

Thank you.

Mason Woo, co-author OpenGL Programming Guide, Version 1.1
mason@woo.com


# OpenGL Programming Guide 红宝书教程讲解
- [The OpenGL Programming Guide](http://www.opengl-redbook.com/)
- [The OpenGL Programming Guide 9th Edition](https://pan.baidu.com/s/1pNgJbOf#gy2l)
- [The OpenGL Programming Guide (Red Book), 9th Edition](https://github.com/openglredbook/examples)
- [The Official Guide to Learning OpenGL, Version 1.1](http://www.glprogramming.com/red/index.html)
- [OpenGL v1.1 Programming Guide (Redbook samples)](https://www.opengl.org/archives/resources/code/samples/redbook/)
- [深入游戏变速底层原理以及内核变速的实现](https://www.52pojie.cn/thread-951616-1-1.html)
- [oglpg-8th-edition.7z](https://pan.baidu.com/s/1kVpv1MR)


红宝书 8、9 版本代码心 MSVC 编译，如果使用 CMake + MinGW-x64 GCC 8.1.0 编译会有兼容性问题。


- ✗ 问题一

代码中使用了 `GetTickCount64`：

    #if (_WIN32_WINNT >= 0x0600) 
    //...
    WINBASEAPI ULONGLONG WINAPI GetTickCount64(void);

    #endif 

所以，遇到符号未定义请设置 CMake 编译器参数，`::GetTickCount64' has not been declared`。


- ✗ 问题二

代码 vdds.cpp 使用了 goto 导致 GCC 兼容性错误，`error: jump to label 'xxx' [-fpermissive]`：

    void vglLoadDDS(const char* filename, vglImageData* image)
    {
        ...
        if (file_header.magic != DDS_MAGIC)
        {
            goto done_close_file;
        }
        size_t current_pos = ftell(f);
        ...

    done_close_file:
        fclose(f);
    }

可以直接使用宽容模式 `-fpermissive` 编译选项忽略这些问题，又或者将 goto 后面的变量初始化移到 goto 前面。


- ✗ 问题三

在 12-particlesimulator.cpp 中使用了一个 `STRINGIZE` 宏来定义着色器程序代码，导致错误的语法 `#version 430`，。

    #define STRINGIZE(a) #a

此宏定义 `#a` 原意是给 a 加双引号变成字符串，解决办法是给着色器程序部分加双引号，并将换行符号转义：

    static const char compute_shader_source[] = STRINGIZE("\
        #version 430 core\n\
        \n\
        layout (std140, binding = 0) uniform attractor_block\n\
        ..."
        );

- ✓ 解决办法

给 CMakeLists.txt 脚本设置编译器条件，同时示例依赖的 GLFW 也需要编译，生成的 `libglfw3.a`、`libvermilion.a` 根据 CMake 编译目录而定，一并设置到链接目录中：

    LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/build/lib )

    message(STATUS "Platform is ----------------> ${CMAKE_SYSTEM_NAME}")
    IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x -D_WIN32_WINNT=0x0600 -fpermissive")
    ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")


