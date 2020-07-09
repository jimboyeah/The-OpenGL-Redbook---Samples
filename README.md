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





## Example 02 Double Buffer 双缓冲绘图
- [OpenGL Programming Guide](http://www.glprogramming.com/red/index.html)

OpenGL Programming Guide 第一章有一个 double.c 示例，展示了 GLUT 的使用，画了一个旋转的方块，这里对其进行了一些修改，程序要点如下：

- 使用 `glutIdleFunc` 和 `glutTimerFunc` 两种方式来更新绘图，使用定时器事件能控制稳定的帧率，空闲回调却不容易。
- 使用了 `glutKeyboardFunc` 和 `glutMouseFunc` 两个基本的键盘、鼠标交互事件。
- 在 `glutInitDisplayMode` 绘图模式初始时，启用了`双缓冲` GLUT_DOUBLE 模式。
- 使用 `glutPostRedisplay` 激发重绘图形。
- 使用 `glOrtho` 和 `glViewport` 两个 API 实现正交投影。

双缓冲技术 `Double Buffer` 的使用能加速绘图效率，两个缓冲区分别用于显示和绘图，计算机动画是画一张显示一张，在复杂的组图过程中，显示设备可以需要等待程序画好下一张图，在这个等待过程中就会出现画面不流畅，画面残缺不完整的问题。双缓冲解决了这个问题，在对一个缓冲区绘图时，显示的画面在另一个缓冲区，当绘图完成后，再通过 `glutSwapBuffers()` 函数将画面映射到最新的绘图区上。

在一个复杂的绘图程序上，通常是以固定的帧率显示画面，而后台程序以最快速度完成下一帧画面的绘制。当出现速度有余量，那么程序可以选择更高的帧率运行，或者以更悠闲的状态运行，这时 `glutIdleFunc` 空闲回调就会得到更高频率的执行，相反它会断断续续地执行。

双缓冲技术使用的存储器有可能是显存，也有可能是主机内存中开辟两块区域，一块作为发送到显示器的数据，一块作为绘画的区域，在适当的时候交换它们。交换两块内存区域实际上只需要交换两个指针，这一方法效率非常高，所以，这是早期图形技术应用最广泛一种。



以下是修改的示例：

    // Example 1-3 : Double-Buffered Program: double.c
    // http://www.glprogramming.com/red/chapter01.html

    // #include <GL/gl.h>
    // #include <GL/glu.h>
    #include <GL/glut.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>

    static GLfloat spin = 0.0;

    #define KEY_0               0x30  // [0,48]
    #define KEY_9               0x39  // [9,57]
    #define KEY_backtick        0x60  // [`,96]
    #define KEY_minu            0x2d  // [-,45]
    #define KEY_equal           0x3d  // [=,61]
    #define KEY_squareR         0x5b  // [[,91]
    #define KEY_squareL         0x5d  // [],93]
    #define KEY_comma           0x2c  // [,,44]
    #define KEY_period          0x2e  // [.,46]
    #define KEY_div             0x2f  // [/,47]
    #define KEY_slash           0x5c  // [\,92]
    #define KEY_ESC             0x1b  // [ESC,27]
    #define KEY_A               0x41  // [A,65]
    #define KEY_B               0x42  // [B,66]
    #define KEY_a               0x61  // [a,97]
    #define KEY_b               0x62  // [b,98]
    #define KEY_x               0x78  // [x,120]

    #define FrameTimer          1
    #define AnimationFrame      50

    bool requestAnimationFrame = false;

    void init(void) 
    {
       glClearColor (0.0, 0.0, 0.0, 0.0);
       glShadeModel (GL_FLAT);
    }

    void display(void)
    {
       glClear(GL_COLOR_BUFFER_BIT);
       glPushMatrix();
       glRotatef(spin, 0.0, 0.0, 1.0);
       glColor3f(1.0, 1.0, 1.0);
       glRectf(-25.0, -25.0, 25.0, 25.0);
       glPopMatrix();
       glutSwapBuffers();
    }

    void spinDisplay(void)
    {
       spin = spin + 2.0;
       if (spin > 360.0)
          spin = spin - 360.0;
       glutPostRedisplay();
    }

    void reshape(int w, int h)
    {
       glViewport (0, 0, (GLsizei) w, (GLsizei) h);
       glMatrixMode(GL_PROJECTION);
       glLoadIdentity();
       glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();
    }

    void onFrame(int timerValue)
    {
       spinDisplay();
       if(requestAnimationFrame)
       {
          glutTimerFunc(AnimationFrame, onFrame, FrameTimer);
       }
    }

    void mouse(int button, int state, int x, int y) 
    {
       if (state != GLUT_DOWN) return;
       switch (button) {
          case GLUT_LEFT_BUTTON:
             glutIdleFunc(spinDisplay);
             requestAnimationFrame = false;
             break;
          case GLUT_RIGHT_BUTTON:
             glutIdleFunc(NULL);
             requestAnimationFrame = true;
             glutTimerFunc(AnimationFrame, onFrame, FrameTimer);
             break;
          default:
             break;
       }
    }

    void keyboard(unsigned char key, int x, int y) { 
        printf("#define KEY_%-16c0x%x  // [%c,%d]\n", key, key, key, key);
        switch (key) { 
          case KEY_x:
          case KEY_ESC:
             exit(EXIT_SUCCESS); break;  
        }
    }

    /* i
     *  Request double buffer display mode.
     *  Register keyboard, mouse input callback functions
     */
    int main(int argc, char** argv)
    {
       glutInit(&argc, argv);
       glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
       glutInitWindowSize (250, 250); 
       glutInitWindowPosition (100, 100);
       glutCreateWindow (argv[0]);
       init ();
       glutDisplayFunc(&display); 
       glutReshapeFunc(&reshape); 
       glutKeyboardFunc(&keyboard);
       glutMouseFunc(&mouse);
       glutMainLoop();
       return EXIT_SUCCESS;
    }

## Example 02 State Management
- http://www.glprogramming.com/red/chapter02.html

第二章讲的是 OpenGL 整个程序运行就是状态机的管理，初始化就是对状态机的初始化，执行绘图也就是设置状态机的绘图状态，还有清理画板，这是三个基本的状态：

Table 2-1 : Clearing Buffers

|        Buffer       |          Name         | Reference  |
|---------------------|-----------------------|------------|
| Color buffer        | GL_COLOR_BUFFER_BIT   | Chapter 4  |
| Depth buffer        | GL_DEPTH_BUFFER_BIT   | Chapter 10 |
| Accumulation buffer | GL_ACCUM_BUFFER_BIT   | Chapter 10 |
| Stencil buffer      | GL_STENCIL_BUFFER_BIT | Chapter 10 |

其中，`glBegin()` 和 `glEnd()` 之间是进行图元绘图，图元是最基本的 OpenGL 图形，`glVertef()` 定义构成图元的顶点坐标，在传统的 OpenGL 2.1 中可以绘制的图元有以下几种，但是在新式的 OpenGL 中这种绘图方法应用比较少，更多是使用着色器。

Table 2-2 : Geometric Primitive Names and Meanings

| 图元类型常量    | 说明    |
| :----------   | :----------   |
| GL_POINTS     | 点 |
| GL_LINES      | 线段    |
| GL_LINE_STRIP | 多段线   |
| GL_LINE_LOOP  | 线圈    |
| GL_TRIANGLES  | 三角形   |
| GL_TRIANGLE_STRIP | 三角形条带 |
| GL_TRIANGLE_FAN   | 三角形扇  |
| GL_QUADS      | 四边形   |
| GL_QUAD_STRIP | 四边形条带 |
| GL_POLYGON    | 多边形(凸)    |

Table 2-3 : Valid Commands between glBegin() and glEnd()

|              Command              |       命令功能       |    参考    |
|-----------------------------------|----------------------|------------|
| `glVertex*()`                     | 设置顶点坐标         | Chapter 2  |
| `glColor*()`                      | 设置当前颜色         | Chapter 4  |
| `glIndex*()`                      | 设置当前颜色索引号   | Chapter 4  |
| `glNormal*()`                     | 设置当前法线向量坐标 | Chapter 2  |
| `glTexCoord*()`                   | 设置纹理坐标         | Chapter 9  |
| `glEdgeFlag*()`                   | 控制连线的绘制       | Chapter 2  |
| `glMaterial*()`                   | 设备模型材质         | Chapter 5  |
| `glArrayElement()`                | 提取顶点数组数据     | Chapter 2  |
| `glEvalCoord*()` `glEvalPoint*()` | 生成坐标             | Chapter 12 |
| `glCallList()` `glCallLists()`    | 执行显示列表         | Chapter 7  |

在绘图过程中激活某些特性，例如 GL_BLEND、GL_FOG 是很常用到的：

    void glEnable(GLenum cap);
    void glDisable(GLenum cap);

常用特性：

|       特性      |                   功能                  |
|-----------------|-----------------------------------------|
| GL_BLEND        | RGBA 混合模式                           |
| GL_DEPTH_TEST   | 深度测试，根据尝试比较更新 depth buffer |
| GL_FOG          | 雾化模糊                                |
| GL_LINE_STIPPLE | 模板线条                                |
| GL_LIGHTING     | 光线                                    |

通过以下 API 获取状态机的信息：

    void glGetBooleanv(GLenum pname, GLboolean *params);
    void glGetIntegerv(GLenum pname, GLint *params);
    void glGetFloatv(GLenum pname, GLfloat *params);
    void glGetDoublev(GLenum pname, GLdouble *params);
    void glGetPointerv(GLenum pname, GLvoid **params);

Figure 2-7 : Geometric Primitive Types

| Primitive type              | GL_FIRST_VERTEX_CONVENTION | GL_LAST_VERTEX_CONVENTION           |
| :--------------             | :--------------            | :--------------                     |
| GL_POINTS                   | i​                         | i​                                  |
| GL_LINES                    | 2i​ - 1                    | 2i​                                 |
| GL_LINE_LOOP                | i​                         | i​ + 1 如果 i​ 小于顶点数，否则为 1 |
| GL_LINE_STRIP               | i​                         | i​ + 1                              |
| GL_TRIANGLES                | 3i​ - 2                    | 3i​                                 |
| GL_TRIANGLE_STRIP           | i​                         | i​ + 2                              |
| GL_TRIANGLE_FAN             | i​ + 1                     | i​ + 2                              |
| GL_LINES_ADJACENCY          | 4i​ - 2                    | 4i​ - 1                             |
| GL_LINE_STRIP_ADJACENCY     | i​ + 1                     | i​ + 2                              |
| GL_TRIANGLES_ADJACENCY      | 6i​ - 5                    | 6i​ - 1                             |
| GL_TRIANGLE_STRIP_ADJACENCY | 2i​ - 1                    | 2i​ + 3                             |

不同的图元，按不同的方法使用顶点来构造图元。

`三角形条带` GL_TRIANGLE_STRIP 的构成到最少 3 个点，正好 3 个点就是一个 GL_TRIANGLE，每增加 1 个点会和之前已有的两个点构成新的三角形，依次类推。也就是说当三角形条带 GL_TRIANGLE_STRIP 由 n 个点组成时，构成的三角形个数是 n -2 个，相同情况下 TRIANGLES 的三角形个数是 n / 3，可以看出当有大量点共用时，三角形条带相比三角形可以节省许多存储空间。

假设起始点的坐标序列号是 0，新增的点依次往后增加，那么`三角形条带`转换的算法如下：

- 当所有点数量小于或者等于 2 时，无法构成三角条带；
- 点号从 0 开始，点号n)是偶数时，构成的三角形是 [ n, n+1, n+2]
- 点号从 0 开始，点号n)是奇数时，构成的三角形是 [n, n+2, n+1]

`四边形` GL_QUADS 转换为三角形，每一个四边形转换为两个三角形：

    [n, n+1, n+2, n+3] = [n, n+1, n+2] + [n, n+2, n+3]


例如，可以通过设置状态机中的顶点来绘制不同的形状，也可以直接使用 glRect 设置一个矩形绘图状态：

    void drawPoints(int type)
    {
        glBegin(type);
        glVertex2d(-0.8, -0.8);
        glVertex2d(0.8, -0.8);
        glVertex2d(-0.8, 0.8);
        glVertex2d(0.8, 0.8);
        glVertex2d(0.2, 0.0);
        glVertex2d(0.8, -0.8);
        glEnd();
    }

    void drawQuads()
    {
        float r = colorQuads[0], g = colorQuads[1], b = colorQuads[2];
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2d(-0.9, -0.4);
        glVertex2d(0.4, -0.4);
        glVertex2d(0.9, 0.4);
        glVertex2d(-0.4, 0.4);
        glEnd();
    }

    void drawRectangle()
    {
        int wi = glutGet(GLUT_WINDOW_WIDTH);
        int he = glutGet(GLUT_WINDOW_HEIGHT);
        float h = 0.4, w = 0.4;
        float cx = 2*(float)_x/wi, cy = 2*(float)_y/he;

        // OpenGL 使用右手系，默认窗体中心为原点，左下角为负，右上角为正
        // 屏幕鼠标的 2D 坐标左上角为原点，右正角为正

        glPushMatrix();
        glScalef(1, -1, 1);
        glTranslatef(-1, -1, 0);
        glRectf(cx - w/2, cy - h/2, cx + w/2, cy + h/2);
        glPopMatrix();
    }

图形绘制前，可以设置颜色、几何变换等等状态，包含顶点也是可以设置状态的。在设置顶点前，也可以给顶点设置一个关联的颜色，在 Example 04 可以看到 GL_SMOOTH 和 GL_FLAT 显示模式是如何应用顶点关联的颜色的。

当图元顶点数量多的时候，可以直接使用顶点数组，首先是激活 GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_INDEX_ARRAY, GL_NORMAL_ARRAY, GL_TEXTURE_COORD_ARRAY, GL_EDGE_FLAG_ARRAY 等：

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

然后使用 API 指定顶点数据：

    void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
    void glIndexPointer(GLenum type, GLsizei stride, const GLvoid *pointer);
    void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *pointer);
    void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
    void glEdgeFlagPointer(GLsizei stride, const GLvoid *pointer);

- size 指定顶点坐标的维度，2、3、4 等。
- type 参数指定数据类型 GL_SHORT, GL_INT, GL_FLOAT, or GL_DOUBLE。
- pointer 是指向顶点数组的指针。
- stride 跨度指定连续顶点的数据偏移字节数，一般用 0 表示连续的数据，除非数组包含其它数据。

Table 2-4 : Vertex Array Sizes (Values per Vertex) and Data Types(continued)

|      Command      |   Sizes    |                 Values for type Argument                 |
|-------------------|------------|----------------------------------------------------------|
| glVertexPointer   | 2, 3, 4    | GL_SHORT, GL_INT, GL_FLOAT, GL_DOUBLE                    |
| glNormalPointer   | 3          | GL_BYTE, GL_SHORT, GL_INT, GL_FLOAT, GL_DOUBLE           |
| glColorPointer    | 3, 4       | 所有基本类型，额外支持 GL_UNSIGNED_SHORT GL_UNSIGNED_INT |
| glIndexPointer    | 1          | GL_UNSIGNED_BYTE, GL_SHORT, GL_INT, GL_FLOAT, GL_DOUBLE  |
| glTexCoordPointer | 1, 2, 3, 4 | GL_SHORT, GL_INT, GL_FLOAT, GL_DOUBLE                    |
| glEdgeFlagPointer | 1          | 无类型指定，只能使用 GLboolean                           |

OpenGL 9 种自带几何体也是这样由顶点定义的，至于像圆形、曲线这些则可以通过多条短线段连接起来模拟，当线段足够细，就能够模拟真实的曲线。

将属性压栈到属性堆栈保护起来：

    void glPushAttrib(GLbitfield mask);
    void glPopAttrib(void);

在 mask 属性指定 GL_ALL_ATTRIB_BITS 可以将所有属性数据压栈。

Table 2-6 : (continued) Attribute Groups

|        Mask Bit        | Attribute Group |
|------------------------|-----------------|
| GL_ACCUM_BUFFER_BIT    | accum-buffer    |
| GL_ALL_ATTRIB_BITS     | --              |
| GL_COLOR_BUFFER_BIT    | color-buffer    |
| GL_CURRENT_BIT         | current         |
| GL_DEPTH_BUFFER_BIT    | depth-buffer    |
| GL_ENABLE_BIT          | enable          |
| GL_EVAL_BIT            | eval            |
| GL_FOG_BIT             | fog             |
| GL_HINT_BIT            | hint            |
| GL_LIGHTING_BIT        | lighting        |
| GL_LINE_BIT            | line            |
| GL_LIST_BIT            | list            |
| GL_PIXEL_MODE_BIT      | pixel           |
| GL_POINT_BIT           | point           |
| GL_POLYGON_BIT         | polygon         |
| GL_POLYGON_STIPPLE_BIT | polygon-stipple |
| GL_SCISSOR_BIT         | scissor         |
| GL_STENCIL_BUFFER_BIT  | stencil-buffer  |
| GL_TEXTURE_BIT         | texture         |
| GL_TRANSFORM_BIT       | transform       |
| GL_VIEWPORT_BIT        | viewport        |


完整代码参考：

- Example 2-5 : Line Stipple Patterns: lines.c
- Example 2-6 : Polygon Stipple Patterns: polys.c
- Example 2-9 : Enabling and Loading Vertex Arrays: varray.c


## Example 03 Viewport & Primitive

要点：

- 鼠标事件处理，滚轮、移动、浮动、点击等。
- 键盘事件处理，一般按键、特殊按键处理。
- 各种图元绘制。
- 通过鼠标滚轮事件进行视图缩放。
- 使用方向按键移到视口中心坐标。
- 使用 gltBitmapString 绘制字符串。
- drawRectangle 中对接收到的光标对应的屏幕坐标变换到 OpenGL 空间坐标。

总体来说，OpenGL 编程中，涉及三大部分的坐标关系处理，参考红宝书 OpenGL Programming Guide - 03 Viewing：

- 模型坐标 Model Coordinates 涉及 Model Matrix 变换；
- 世界坐标 World Coordinates 涉及 View Matrix 变换；
- 相机坐标 Camera Coordinates 涉及 Projection Matrix 变换；

在 OpenGL 中使用 `gluLookAt` 改变的是相机坐标，而设置投影方式的 API 改变的是世界坐标系，即视口的变换，至于模型的坐标，顶点通过变换矩阵的转换后就改变了。

在 OpenGL 对视图变换并不会改变模型坐标，而是移动或缩放摄像机的镜头，从不同的方位观察模型，常用`glLookAt` 函数设置观察者视角的变换矩阵。

    void gluLookAt( GLdouble eyeX,
                    GLdouble eyeY,
                    GLdouble eyeZ,
                    GLdouble centerX,
                    GLdouble centerY,
                    GLdouble centerZ,
                    GLdouble upX,
                    GLdouble upY,
                    GLdouble upZ);

glLookAt 会定义一个视图矩阵，成像点为 (eyeX, eyeY, eyeZ)，目标物体对应 center 坐标方向，镜像上位方向对应 up 坐标，这三点的坐标确定了的的摄像机的姿态。视图变换矩阵与当前矩阵相乘，获得视图矩阵，再该模型视图矩阵左乘模型，就会获得在观察位置上模型的状态，就是我们在屏幕上最终看到的模型的状态。

gluLookAt 用来定义观察者(相机)的状态，包括观察者在世界坐标系中所处的位置、看向世界坐标系中的方向，可以理解为眼睛所看向的方向、观察者头部的朝向（可以在一个平面上360°旋转）。

如果没有调用 glLookAt 设置视图矩阵，默认情况下，相机会被设置为位置在世界坐标系原点，指向 Z 轴负方向，朝上向量为（0，1，0）。

正交投影在 OpenGL 中是比较基础的投影变换，软件在算法上使用的 3D 世界，在成像的过程中，需要将立体的世界投射为设备显示的 2D 画面，这就需要一个投射方法。

常见且较容易实现的有`透视法`和`正交投影` Perspective & Orthographic Projection。透视法是西方艺术中基本的构图法，有固定的`消逝点` Vanishing point or line，是一种模拟人类真视野中的观测物体，总结起来就是近大远小。明显的例子，就是平行铁轨，远处两条铁轨会相交于一点。而中国的山水法则不然，没有规则的消逝点或线，也不像正交投影，是一种在计算机上难以实现的构图方法。

    void glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

    void glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

    void gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear, GLdouble zFar);

`正交投影`通常用在工程制图中，需要比较精确的显示，而不是追求视觉的真实感。形象点说，一个物体特有点向投影平面作垂线，垂线与平面的交点的集合就是需要的投影。与透视法相比，正交投影法一般用于物体不会因为离屏幕的远近而产生大小的变换的情况。

这里的投影是向量的投影，几何的投影，算法实现上需要有一定的线性代数基础、几何学等。

OpenGL API 中的 `glOrtho()` 就是用来创建一个正交平行的视景体，它代表了一个变换矩阵，与 OpenGL 程序的当前矩阵相乘。而 `glFrustum()` 则对应一个透视投影的变换矩阵，`gluPerspective()` 函数封装了 `glFrustum()`。

透视法中，FOV - Field of View 视场角度，[0.0, 180.0]，是一个特别重要的参数，镜头到物体保持同样距离的条件下，FOV 越大看到的东西越多，从另一面讲，看到的东西细节越少，相当于放大镜的倍数低了。截断面的宽高比 aspect 会影响输出到窗体的图形比例。near 和 far 是景深的近端和远端，都应该为正数，沿 X 轴负向为远端。

glOrtho 函数参数表示视景体六面的坐标约束，依次是 left、right、bottom、top，zNear 和
zFar 分别代表 Z 轴上的前后两面约束位置。这个函数简单理解起来，就是创建一个盒子摆在那里。

其中近裁剪平面是一个矩形，靠前方的近端面，矩形左下角三维空间坐标点是 (left，bottom，-near)，
右上角坐标点是 (right，top，-near)；远裁剪平面也是一个矩形，左下角点空间坐标是 (left，bottom，-far)，右上角点是（right，top，-far)。

假设有一个半径为 1 的球体，圆心坐标在 (0, 0, 0)，那么，以下两个宽高都是 3 的正交投影盒子，分别会将球体：

    glOrtho(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5);
    glOrtho( 0.0, 1.5, -1.5, 1.5, -1.5, 1.5);

当 left = right，或者 top = bottom，又或者 near = far，那么这个这个视景体至少有一个维度压缩为 0，这样无法显示任何图形 参考 clip.c 示例。

针对不同的变换，OpenGL 系统中有多个变换矩阵对应，即`矩阵堆栈` Matrix Statck 保存的矩阵数据。所以，在执行矩阵变换前，需要通过 `glMatrixMode` 指定对什么矩阵进行操作：

- `GL_MODELVIEW` 开始对模型视图矩阵堆栈操作，进入此模式后可以输出自己的物体模型。
- `GL_PROJECTION` 开始对投影矩阵堆栈操作，进入此模式后可以为场景增加透视矩阵变换。
- `GL_TEXTURE` 开始对纹理矩阵堆栈操作，进入此模式后可以为模型增加纹理贴图。
- `GL_COLOR` 开始对色彩矩阵堆栈操作，可以变换色彩。

每个矩阵模式下都有一个矩阵堆栈，在 `GL_MODELVIEW` 模式中，堆栈深度至少为 32，在 `GL_PROJECTION` 和 `GL_TEXTURE` 模式中，堆栈深度至少为 2，无论在任何模式下，当前矩阵 Current Matrix 总是该模式下矩阵堆栈中的最顶层矩阵。


无论模型如果变换，最终还是要在设备上显示，`glViewport()` 设置视口，中心点坐标 (x, y)，宽度和高度 width、height，深度另外 API 设置。视口设置，即指定 OpenGL 3D 空间中，哪一部分输出到设备上显示出来。`默认视口` 在 Z 轴向其负轴方向看。对于`显示器坐标系统` Screen Coordinate 来说，左上角为原点，而且 Y 轴上下颠倒。那么，对于 OpenGL 空间位置坐标为 (-1, 1) 的一个物体，假定视口长宽为 2，它会显示在屏幕的左上角，也就显示器的 (0,0) 的坐标，但是在软件中编码，不去考虑屏幕的坐标，因为视口背后的逻辑已经进行了一个矩阵变换操作，将屏幕的坐标系统映射到了视口中。

    void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
    
而通过 `glViewport` 指定的视口区域代表的是一个仿射变换，affine transformation，只是将设备的坐标映射到了软件中 3D 世界的一个剖面上，函数参数 (x, y) 就在指定平移变换，width、height 就是指定大小缩放变换。

OpenGL 中存在多个视口，保存在 Viewport 数组中，视口数量为 [0, GL_MAX_VIEWPORTS)，并且给特定图元使用的视口还可以通过 GS - Geometry Shader 指定。如果 GS 没有指定视口，那么数组中开头那个就是默认的。

其它和视口相关的 API 如下:

    void glViewportIndexedf(GLuint index​, GLfloat x​, GLfloat y​, GLfloat w​, GLfloat h​)
    void glViewportIndexedfv(GLuint index​, const GLfloat *v​)
    void glDepthRangeIndexed(GLuint index​, GLdouble nearVal​, GLdouble farVal​)

    void glViewportArrayv(GLuint first​, GLsizei count​, const GLfloat *v​)
    void glDepthRangeArrayv(GLuint first​, GLsizei count​, const GLdouble *v​)


以下 API 会改变模型的变换矩阵：

    void glTranslate{fd}(TYPEx, TYPE y, TYPEz);
    void glRotate{fd}(TYPE angle, TYPE x, TYPE y, TYPE z);
    void glScale{fd}(TYPEx, TYPE y, TYPEz);

在以下方法可以将当前操作的矩阵堆栈保护起来、或恢复，更直接的办法是使用加载单位矩阵，它会还原之前所有的变换操作：

    glPushMatrix ();
    glPopMatrix ();
    glLoadIdentity ();

在变换过程中，会编码深度信息并保存在 depth buffer，通常，深度值范围是 [0.0, 1.0]，但可以通过以下 API 修改：

    void glDepthRange(GLdouble nearVal​, GLdouble farVal​);
    void glDepthRangef(GLfloat nearVal​, GLfloat farVal​);


一些函数功能说明：

`glEnable()`、`glDisable()` 配套使用，分别用于激活和关闭功能：

|        功能       |      说明      |
|-------------------|----------------|
| GL_CULL_FACE      | 背面剔除       |
| GL_BLEND          | 颜色混合       |
| GL_FOG            | 雾化效果       |
| GL_POLYGON_SMOOTH | 顶点图元的平滑 |
| GL_LINE_SMOOTH    | 顶点图元的平滑 |
| GL_POINT_SMOOTH   | 顶点图元的平滑 |


三维空间来看，一个多边形具有两个面。通过 API 设置不同的绘制方式：填充、绘制轮廓线、绘制顶点，其中填充是默认的方式：

    void glPolygonMode(GLenum face,GLenum mode);

face 参数控制多边形的正面和背面的绘图模式：

- `GL_FRONT`：表示显示模式将适用于物体的前向面（也就是物体能看到的面）
- `GL_BACK`：表示显示模式将适用于物体的后向面（也就是物体上不能看到的面）
- `GL_FRONT_AND_BACK`：表示显示模式将适用于物体的所有面

mode 参数指定图形显示模式：

- GL_POINT：表示只显示顶点，多边形用顶点显示
- GL_LINE：表示显示线段，多边形用轮廓显示
- GL_FILL：表示显示面，多边形采用填充形式

指定顶点或线条的大小：

    glPointSize(8);
    glLineWidth(5);

在 OpenGL 中图形锯齿现象的消除技术称为反走样，也叫做抗锯齿 Antialias，可通过函数 `glHint()`对图像质量和绘制速度之间的权衡作一些控制，其函数形式为：

    void glHint(GLenum target,GLenum hint);

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points  
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines  

target 参数指定需要抗锯齿的目标，参考以 `_HINT` 后缀的常量：

    GL_FOG_HINT
    GL_LINE_SMOOTH_HINT
    GL_POINT_SMOOTH_HINT
    GL_POLYGON_SMOOTH_HINT
    GL_FRAGMENT_SHADER_DERIVATIVE_HINT
    GL_GENERATE_MIPMAP_HINT
    GL_PERSPECTIVE_CORRECTION_HINT
    GL_TEXTURE_COMPRESSION_HINT

GL_FOG_HINT 指出雾是按像素进行（GL_NICEST）还是按顶点进行（GL_FASTEST）；

SMOOTH_HINT 后后缀常量分别指定点、线和多边形的采样质量；

hint 参数可以指定模式为速度优先 GL_FASTEST、质量优先 GL_NICEST、不关注 GL_DONT_CARE。

对图元进行反走样要先调用 `glEnable()` 激活图元平滑模式，如 GL_POLYGON_SMOOTH。如果是在 RGBA 模式下进行反走样，还必须与融合模式配合使用，通常分别使用 GL_SRC_ALPHA 和 GL_ONE_MINUS_SRC_ALPHA 作为源和目的因子。

    glEnable(GL_BLEND);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

对于空间上的物体，按其到镜头的距离，即通过深度 depth  可以很容易检测到有没有被遮挡，对于遮挡住的部分就不应该渲染，否则就破坏了图形的逻辑正确。

深度测试开启、深度图形清除：

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glEnable(GL_DEPTH_TEST);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


代码参考：

- Example 3-1 : Transformed Cube: cube.c
- Example 3-2 : Using Modeling Transformations: model.c
- Example 3-5 : Wireframe Sphere with Two Clipping Planes: clip.c
- Example 3-6 : Planetary System: planet.c
- Example 3-7 : Robot Arm: robot.c






## Example 04 Color
- http://www.glprogramming.com/red/chapter04.html
- https://github.com/markkilgard/glut
- https://www.opengl.org/resources/libraries/glut/spec3/node65.html

Example 4-1 : Drawing a Smooth-Shaded Triangle: smooth.c

OpenGL 程序初始化时设置一个颜色模式和着色模式：

    glutInitDisplayMode (GLUT_SINGLE | GLUT_INDEX);

    glShadeModel (mode? GL_FLAT:GL_SMOOTH);

    #define  GLUT_RGB                           0x0000
    #define  GLUT_RGBA                          0x0000
    #define  GLUT_INDEX                         0x0001

彩色可以是 RGB 或 RGBA 模式，也可以是索引颜色。当图形中使用的颜色是有限的数量时，索引颜色能节省内存，原先需要 32bit 的 RBGA 可以用一个索引号码和一个色盘定义就可以替代。

在索引色模式下，OpenGL 使用 `glIndexi()` 这类方法进行色彩映射或称为 lookup table 来确定颜色，这非常像使用色盘。索引色的使用可以参考 GLUT 中提供的示例 olympic.c 奥林匹克五环，scube.c 旋转盒子。

根据不同数据类型，色彩指定 API 使用相应后缀：

    void glColor3{b s i f d ub us ui} (TYPEr, TYPEg, TYPEb);
    void glColor4{b s i f d ub us ui} (TYPEr, TYPEg, TYPEb, TYPEa);
    void glColor3{b s i f d ub us ui}v (const TYPE*v);
    void glColor4{b s i f d ub us ui}v (const TYPE*v);

    void glIndex{sifd ub}(TYPE c);
    void glIndex{sifd ub}v(const TYPE *c);

索引色管理 API，如下，获取索引色分量或设置索引色，以及获取当前系统索引色数量：

    GLfloat glutGetColor(int cell, int component);
    void glutSetColor(int cell, GLfloat red, GLfloat green, GLfloat blue);
    glutGet(GLUT_WINDOW_COLORMAP_SIZE);
    void glutCopyColormap(int win);

参数:

- `cell` 表示索引色单元位置，[0 - GLUT_WINDOW_COLORMAP_SIZE)。
- `component` 指定分量 GLUT_RED, GLUT_GREEN, or GLUT_BLUE。
- `win` 指定要从哪个窗体拷贝色彩映射 colormap。

如果获取索引色大小返回以下信息，表明 GLUT 的实现没有包含此功能，测试当前 Windows 10 运行的程序有 19 种索引色：

    glutGet(): missing enum handle 119

设置颜色后，可以通过 glGetIntegerv 获取和分量值 GL_RED_BITS, GL_GREEN_BITS, GL_BLUE_BITS, GL_ALPHA_BITS, GL_INDEX_BITS：

    glColor3f (1.0, 0.0, 0.0);  /* the current RGB color is red: */
                                /* full red, no green, no blue. */
    glBegin (GL_POINTS);
        glVertex3fv (point_array);
    glEnd ();

    glGetIntegerv(GL_INDEX_BITS);

通过激活抖动，可以用黑色散点模拟灰度，黑点越密集，抖动形成的灰度色越深：

    glEnable(GL_DITHER);
    ....
    glDisable(GL_DITHER);

Table 4.1 颜色值转换浮点表示的对应关系：

| Suffix |        Data Type        | Minimum Value  | Min Value Maps to | Maximum Value | Max Value Maps to |
|--------|-------------------------|----------------|-------------------|---------------|-------------------|
| b      | 1-byte integer          | -128           |              -1.0 | 127           |               1.0 |
| s      | 2-byte integer          | -32,768        |              -1.0 | 32,767        |               1.0 |
| i      | 4-byte integer          | -2,147,483,648 |              -1.0 | 2,147,483,647 |               1.0 |
| ub     | unsigned 1-byte integer | 0              |               0.0 | 255           |               1.0 |
| us     | unsigned 2-byte integer | 0              |               0.0 | 65,535        |               1.0 |
| ui     | unsigned 4-byte integer | 0              |               0.0 | 4,294,967,295 |               1.0 |

在 GL_SMOOTH 着色模式，每个顶点对应颜色会平滑地应用到片元上，在顶点构成的片元空间以渐变色填充；而使用 GL_FLAT 平铺模式，假设几何图形由 n 个三角形构成，则片元空间只会使用顶点颜色数组中最后一个颜色进行着色。

Table 4-2 在各种 Flat-Shaded 多边形中 OpenGL 选择颜色的方式：

|      多边形类型      | 用于选择颜色的顶点 |
|----------------------|--------------------|
| single polygon       | 1                  |
| triangle strip       | i+2                |
| triangle fan         | i+2                |
| independent triangle | 3i                 |
| quad strip           | 2i+2               |
| independent quad     | 4i                 |


## Example 05 Lighting
- http://www.glprogramming.com/red/chapter05.html
- https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glDepthFunc.xml
- https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glCullFace.xml
- https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMaterial.xml
- https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLightModel.xml
- https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml


本示例展示：

- OpenGL 如果模拟真实世界的光照。
- 定义光源，照亮模型。
- 定义材质属性以反映光照。
- 操纵矩阵堆栈控制光源。

前面讲解模型的彩色使用，但是眼睛看到的彩色并非在完全来自模型，而是光线与模型的材质相互作用下产生的视觉感官效应。光线在物理上可以看作电磁波，不同的波长具有不同的颜色效应，面物体的材质会影响电磁波的形成最终的视觉效果。比如，同样的绿色植物，在白天看起来是绿色的，晚上用其它彩色灯光照射，却会显示不同的颜色。而同样的绿色玻璃，在同样的光线条件下，也与绿色植物看起来不一样，这是材质决定的。

材质影响视觉效果的属性有很多，其中材质颜色属性 Material Colors 是基本的一个，材质的透射率、折射率都会影响视觉效果。透射 Transmission 相关属性对金属无效，对于不透明的材料 Transmission = 0，那么折射率 IOR - Index of Refraction 就是无效属性。物体的粗糙度对光反射有很大的影响，但是 Fresnel 效应一直存在，它会在即使是没有镜面反射的木球也会表现出周边更亮。

而光线与模型接触点是很关键的，按照光的直线传播原理，接触面的法线方向 Normal Vectors 决定的光的反射方向，折射率和入射角共同决定折射方向。

真实世界中，不存在唯一光源，白天除了太阳光，还大量的散射光，透射光。环境光 Ambient, 散射光 Diffuse, 反射光 Specular Light 是 OpenGL 三类主要的模拟光源属性，这决定了模型的材质是否产生效果。

可以参考 PBR - Physically Based Rendering 中对物理学上的光线处理模型。

在 OpenGL 中，基本每次重绘都需要清理画板中旧的图形，不可能看到无限远的物体，只能通过清除方法将这些物休的颜色信息清除：

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

OpenGL 抽象的相机在成像过程中，使用到 3D 空间模型的深度信息 Z，它是模型到镜头的距离，如果要理解为镜头坐标的 Z 轴也可以。在决定是否绘制一个物体的表面时，首先将表面对应像素的深度值与当前深度缓冲区中的值进行比较，如果大于等于深度缓冲区中值，则丢弃这部分。否则利用这个像素对应的深度值和颜色值，分别更新深度缓冲区和颜色缓冲区。这一过程称之为深度测试 Depth Testing。

尝试测试的方式可以通过以下 API 设置：

    void glDepthFunc(GLenum func);

    glEnable (GL_DEPTH_TEST);
    glDisable (GL_DEPTH_TEST);

指定 func 为以下测试比较方式：

| 测试比较方式 |                  说明                  |
|--------------|----------------------------------------|
| GL_NEVER     | 总是不通过，总是不绘制。               |
| GL_LESS      | 测试深度小于保存的深度值时通过。       |
| GL_EQUAL     | 测试深度等于保存的深度值时通过。       |
| GL_LEQUAL    | 测试深度小于或等于保存的深度值时通过。 |
| GL_GREATER   | 测试深度大于保存的深度值时通过。       |
| GL_NOTEQUAL  | 测试深度不等于保存的深度值时通过。     |
| GL_GEQUAL    | 测试深度大于或等于保存的深度值时通过。 |
| GL_ALWAYS    | 总是通过测试，问题绘制。               |

在绘制 3D 场景的模型还需要决定哪些部分是对观察者可见的，或者哪些部分是对观察者不可见的。对于不可见的部分，对于一个不透明的盒子，同时最多只能看到三个面，其它面被遮挡就不应该渲染，这种情况叫做隐藏面消除 Hidden surface elimination。

    glEnalbe(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    void glCullFace(GLenum mode);
    void glFrontFace(int mode );

剔除模式 mode：

|      剔除方式     |        说明        |
|-------------------|--------------------|
| GL_BACK           | 只剔除背向面，默认 |
| GL_FRONT          | 只剔除正向面       |
| GL_FRONT_AND_BACK | 剔除正向面和背向面 |


除了需要剔除的面之外，glFrontFace 方法告诉 OpenGL 如何判断正向面：

| 判断模式 |             说明             |
|----------|------------------------------|
| GL_CCW   | 代表逆时针方向为正向面，默认 |
| GL_CW    | 代表顺时针方向为正向面       |

默认设置，假定一个三角形，握住右手四指逆时针方向放在三个顶点对应的绘画顺序上，那么拇指指向的面为正向面。


在 OpenGL 内部定义了 8 个光源属性组 `GL_LIGHT0`, `GL_LIGHT1` ... `GL_LIGHT7`：

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

创建光源时要在 `light` 参数指定光源属于那一组，还有灯光其它属性：

    void glLight{if}(GLenum light, GLenum pname, TYPEparam);
    void glLight{if}v(GLenum light, GLenum pname, TYPE *param);

Table 5-1 pname 参数指定的光源默认值

|      Parameter Name      |    Default Value     |             Meaning              |
|--------------------------|----------------------|----------------------------------|
| GL_AMBIENT               | (0.0, 0.0, 0.0, 1.0) | ambient RGBA intensity of light  |
| GL_DIFFUSE               | (1.0, 1.0, 1.0, 1.0) | diffuse RGBA intensity of light  |
| GL_SPECULAR              | (1.0, 1.0, 1.0, 1.0) | specular RGBA intensity of light |
| GL_POSITION              | (0.0, 0.0, 1.0, 0.0) | (x, y, z, w) position of light   |
| GL_SPOT_DIRECTION        | (0.0, 0.0, -1.0)     | (x, y, z) direction of spotlight |
| GL_SPOT_EXPONENT         | 0.0                  | spotlight exponent               |
| GL_SPOT_CUTOFF           | 180.0                | spotlight cutoff angle           |
| GL_CONSTANT_ATTENUATION  | 1.0                  | constant attenuation factor      |
| GL_LINEAR_ATTENUATION    | 0.0                  | linear attenuation factor        |
| GL_QUADRATIC_ATTENUATION | 0.0                  | quadratic attenuation factor     |

其中 GL_DIFFUSE、GL_SPECULAR 指定的默认值只应用于 GL_LIGHT0，其它光源属性 GL_DIFFUSE、GL_SPECULAR 默认值是 (0.0, 0.0, 0.0, 1.0)。

可以在初始化阶段指定光源，也可以在重绘事件中指定：

    static void initLights()
    {
        glClearColor(0.0f, 0.0f, 0.7f, 1.0f);

        GLfloat ambientLight[]  = {0.2f,  0.2f,  0.2f,  1.0f};//环境光源
        GLfloat diffuseLight[]  = {0.9f,  0.9f,  0.9f,  1.0f};//漫反射光源
        GLfloat specularLight[] = {1.0f,  1.0f,  1.0f,  1.0f};//镜面光源
        GLfloat lightPos[]      = {50.0f, 80.0f, 60.0f, 1.0f};//光源位置
     
        glEnable(GL_LIGHTING);                                //启用光照
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);       //设置环境光源
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);       //设置漫反射光源
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);     //设置镜面光源
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);          //设置灯光位置
        glEnable(GL_LIGHT0);                                  //打开第一个灯光
     
        glEnable(GL_COLOR_MATERIAL);                          //启用材质的颜色跟踪
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);    //指定材料着色的面
        glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);   //指定材料对镜面光的反应
        glMateriali(GL_FRONT, GL_SHININESS, 100);             //指定反射系数
    }

有了光源后，就可以为模型指定材质属性：

    void glMaterialf(   GLenum face, GLenum pname, GLfloat param);
    void glMateriali(   GLenum face, GLenum pname, GLint param);
    void glMaterialfv(  GLenum face, GLenum pname, const GLfloat * params);
    void glMaterialiv(  GLenum face, GLenum pname, const GLint * params);

face 可以设置 GL_FRONT, GL_BACK, GL_FRONT_AND_BACK。

pname 参数设置参考，整形或者浮点值都可以，整形会被映射到 [-1, 1]：

|        pname 值        |                        对应 params 值                        |
|------------------------|--------------------------------------------------------------|
| GL_AMBIENT             | 设置材质对环境光的作用，默认值 (0.2, 0.2, 0.2, 1.0)          |
| GL_DIFFUSE             | 材质对散射光的作用，默认值 (0.8, 0.8, 0.8, 1.0)              |
| GL_SPECULAR            | 设置一个高光值，默认值为 (0, 0, 0, 1)                        |
| GL_EMISSION            | 设置一个自发光值，默认初始值为 (0, 0, 0, 1)                  |
| GL_SHININESS           | 设置一个亮度，默认前后面的值为 0。                           |
| GL_AMBIENT_AND_DIFFUSE | 相当调用两次 API                                             |
| GL_COLOR_INDEXES       | 索引色默认值 (0,1,1)，对应 ambient, diffuse, specular 索引值 |

材质索引色示例：

    GLfloat mat_colormap[] = { 16.0, 47.0, 79.0 };
    glMaterialfv(GL_FRONT, GL_COLOR_INDEXES, mat_colormap);

还可以设置灯光模型：

    void glLightModelf( GLenum pname, GLfloat param);
    void glLightModeli( GLenum pname, GLint param);

    void glLightModelfv( GLenum pname, const GLfloat * params);
    void glLightModeliv( GLenum pname, const GLint * params);

pname 和 params 参数参考：

|            pname             |                  对应 params                  |
|------------------------------|-----------------------------------------------|
| GL_LIGHT_MODEL_AMBIENT       | 默认值 (0.2, 0.2, 0.2, 1.0)                   |
| GL_LIGHT_MODEL_COLOR_CONTROL | GL_SEPARATE_SPECULAR_COLOR 或 GL_SINGLE_COLOR |
| GL_LIGHT_MODEL_LOCAL_VIEWER  | 默认值 0 表示平行 X 轴的光                   |
| GL_LIGHT_MODEL_TWO_SIDE      | 默认值 0 表示前单独光照，否则双面光照         |

<script id="MathJax-script" async
      src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js">
</script>

索引色光照的数学 Color-Index Mode Lighting

As you might expect, since the allowable parameters are different for color-index mode than for RGBA mode, the calculations are different as well. Since there's no material emission and no ambient light, the only terms of interest from the RGBA equations are the diffuse and specular contributions from the light sources and the shininess. Even these need to be modified, however, as explained next.

Begin with the diffuse and specular terms from the RGBA equations. In the diffuse term, instead of diffuselight * diffusematerial, substitute dci as defined in the previous section for color-index mode. Similarly, in the specular term, instead of specularlight * specularmaterial, use sci as defined in the previous section. (Calculate the attenuation, spotlight effect, and all other components of these terms as before.) Call these modified diffuse and specular terms d and s, respectively. Now let \\(s' = min(s,1)\\), and then compute

$$c = am + d(1-s')(dm-am) + s'(sm-am)$$

where am, dm, and sm are the ambient, diffuse, and specular material indexes specified using GL_COLOR_INDEXES. The final color index is

$$c' = min { c, sm }$$

After lighting calculations are performed, the color-index values are converted to fixed-point (with an unspecified number of bits to the right of the binary point). Then the integer portion is masked (bitwise ANDed) with 2n-1, where n is the number of bits in a color in the color-index buffer.



## Chapter 6 Blending, Antialiasing, Fog, Polygon Offset

章节学习目标：

- 使用色彩混合 Blend 实现半透明效果；
- 使用抗锯齿平滑边线和多边形；
- 使用雾化实现大气模糊效果；
- 在指定深度绘制避免几何体交叠产生不真实感，unaesthetic artifacts；

混合涉及来源和目标两个数据，还有对应的系数，\\((S_r, S_g, S_b, S_a)\\) 和 \\((D_r, D_g, D_b, D_a)\\)，每个分量值范围 [0,1]，混合结果可以这样表示：

$$(R_s S_r+R_d D_r, G_s S_g+G_d D_g, B_s S_b+B_d D_b, A_s S_a+A_d D_a) $$

激活混合模式以及设置混合系数：

    glEnable(GL_BLEND); 
    glDisable(GL_BLEND);

    void glBlendFunc(GLenum sfactor, GLenum dfactor);

禁止混合和设置 `glBlendFunc(GL_ONE, GL_ZERO);` 是等效的，这也是默认的设置。

Table 6-1 Source & Destination 混合因数的计算，加减号表示对应分量相加减：

|        Constant        |    Relevant Factor    |     Computed Blend Factor     |
|------------------------|-----------------------|-------------------------------|
| GL_ZERO                | source or destination | (0, 0, 0, 0)                  |
| GL_ONE                 | source or destination | (1, 1, 1, 1)                  |
| GL_DST_COLOR           | source                | (Rd, Gd, Bd, Ad)              |
| GL_SRC_COLOR           | destination           | (Rs, Gs, Bs, As)              |
| GL_ONE_MINUS_DST_COLOR | source                | (1, 1, 1, 1)-(Rd, Gd, Bd, Ad) |
| GL_ONE_MINUS_SRC_COLOR | destination           | (1, 1, 1, 1)-(Rs, Gs, Bs, As) |
| GL_SRC_ALPHA           | source or destination | (As, As, As, As)              |
| GL_ONE_MINUS_SRC_ALPHA | source or destination | (1, 1, 1, 1)-(As, As, As, As) |
| GL_DST_ALPHA           | source or destination | (Ad, Ad, Ad, Ad)              |
| GL_ONE_MINUS_DST_ALPHA | source or destination | (1, 1, 1, 1)-(Ad, Ad, Ad, Ad) |
| GL_SRC_ALPHA_SATURATE  | source                | (f, f, f, 1); f=min(As, 1-Ad) |

举例说明：

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

源因子 GL_SRC_ALPHA 表示用来源数据的 Alpha 与来源像素相乘，目标因子 GL_ONE_MINUS_SRC_ALPHA 表示对来源 Alpha 取反再和目标像素相乘。结果就是，来源相素 Alpha 越高即透明度越低，混合后的两个图案中，来源图案更清晰。

另一个例子：

    glBlendFunc(GL_ONE, GL_ZERO);

这就使用使用来源的像素覆盖目标像素，因为来源因子全是 1 表示完全保留，目标因子全是 0 表示被覆盖。

Example 6-1 : Blending Example: alpha.c 绘制两个三角形演示了混合因子对混合结果的影响。

Example 6-2 : Three-Dimensional Blending: alpha3D.c 展示 3D 混合，其中使用了显示列表 Display List。

Example 6-3 : Antialiased lines: aargb.c 展示了抗锯齿效果。


Table 6-2 在 glHint(target, hint) 使用的常量

|           Parameter            |                       Meaning                       |
|--------------------------------|-----------------------------------------------------|
| GL_FOG_HINT                    | 指定雾体是按像素 GL_NICEST 还按顶点 GL_FASTEST 处理, |
| GL_LINE_SMOOTH_HINT            | 指定线条采样质量                                    |
| GL_PERSPECTIVE_CORRECTION_HINT | 指定颜色或纹理插值的质量                            |
| GL_POINT_SMOOTH_HINT           | 指定顶点采样质量                                    |
| GL_POLYGON_SMOOTH_HINT         | 指定多边形采样质量                                  |

