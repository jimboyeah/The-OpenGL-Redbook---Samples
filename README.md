OpenGL Red Book Example Code
============================

This is the example code for the OpenGL Programming Guide (Red Book), 9th Edition.
It is mostly an import of code from previous editions with minor updates
to match some of the descriptions in the latest edition of the book.
We will be adding more samples and updating existing ones over time.
Treat this as a live repository. Pull requests and issues are welcome.
Go forth and fork!

Building
--------

We are using CMake as our build system. To build the samples, enter
the "build" subdirectory and enter "cmake -G "{your generator here}" .."
Project files or makefiles will be produced in the build subdirectory.
We have tested Unix makefiles ("Unix Makefiles") and Visual Studio project files.
Visual Studio 2013 ("Visual Studio 12") was used to develop most of the samples.
An update to add support for 2015 will land shortly. Code should compile, but the
templates for the project files are not in the repository at this time.
Other compilers and IDEs should work, but we haven't tested them.

Running
-------

After building, there will be a large collection of executables in the bin directory.
These are the example applications, which you can directly execute.

The media is now included with the git repository, and
the samples should compile and run out-of-the box. Of course, make sure your
OpenGL drivers are up to date and that your hardware is capable of running OpenGL 4.5.
That's what this book is about. Some systems don't support OpenGL 4.5 and will therefore
not run some of the samples. Trying to run such samples on a machine that doesn't have
OpenGL 4.5 support will fail. For example, if a platform were limited to, say OpenGL 4.1,
then the samples wouldn't work on that platform. Please don't file bugs about that either.
Error checking in these applications is minimal. If you don't have media files or if
your OpenGL drivers are out of date, they'll probably fail spectacularly.


# OpenGL Programming Guide 红宝书教程讲解
- [The OpenGL Programming Guide](http://www.opengl-redbook.com/)
- [The OpenGL Programming Guide 9th Edition](https://pan.baidu.com/s/1pNgJbOf#gy2l)
- [The OpenGL Programming Guide (Red Book), 9th Edition](https://github.com/openglredbook/examples)
- [The Official Guide to Learning OpenGL, Version 1.1](http://www.glprogramming.com/red/index.html)
- [OpenGL v1.1 Programming Guide (Redbook samples)](https://www.opengl.org/archives/resources/code/samples/redbook/)
- [深入游戏变速底层原理以及内核变速的实现](https://www.52pojie.cn/thread-951616-1-1.html)
- [oglpg-8th-edition.7z](https://pan.baidu.com/s/1kVpv1MR)

主分支为出版社原书 9th 配套代码，支持为红宝书 1st、8th、9th 配置 Sublime + MinGW + CMake 编译环境，请进入各分支下查看：

- https://github.com/jimboyeah/The-OpenGL-Redbook---Samples/tree/redbook-samples-1.1
- https://github.com/jimboyeah/The-OpenGL-Redbook---Samples/tree/redbook-samples-8th
- https://github.com/jimboyeah/The-OpenGL-Redbook---Samples/tree/redbook-samples-9th

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


