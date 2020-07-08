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

Begin with the diffuse and specular terms from the RGBA equations. In the diffuse term, instead of diffuselight * diffusematerial, substitute dci as defined in the previous section for color-index mode. Similarly, in the specular term, instead of specularlight * specularmaterial, use sci as defined in the previous section. (Calculate the attenuation, spotlight effect, and all other components of these terms as before.) Call these modified diffuse and specular terms d and s, respectively. Now let \[[s' = min{ s, 1 }\\], and then compute

$$c = am + d(1-s')(dm-am) + s'(sm-am)$$

where am, dm, and sm are the ambient, diffuse, and specular material indexes specified using GL_COLOR_INDEXES. The final color index is

\\[c' = min { c, sm }\\]

After lighting calculations are performed, the color-index values are converted to fixed-point (with an unspecified number of bits to the right of the binary point). Then the integer portion is masked (bitwise ANDed) with 2n-1, where n is the number of bits in a color in the color-index buffer.



## Chapter 6 Blending, Antialiasing, Fog, Polygon Offset

章节学习目标：

- 使用色彩混合 Blend 实现半透明效果；
- 使用抗锯齿平滑边线和多边形；
- 使用雾化实现大气模糊效果；
- 在指定深度绘制避免几何体交叠产生不真实感，unaesthetic artifacts；

混合涉及来源和目标两个数据，还有对应的系数，\\((S_r, S_g, S_b, S_a)\\) 和 \\((D_r, D_g, D_b, D_a)\\)，每个分量值范围 [0,1]，混合结果可以这样表示：

\\[(R_s S_r+R_d D_r, G_s S_g+G_d D_g, B_s S_b+B_d D_b, A_s S_a+A_d D_a) \\]

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

示例 Example 6-1 : Blending Example: alpha.c 中，通过绘制两个三角形演示了混合因子对混合结果的影响。

