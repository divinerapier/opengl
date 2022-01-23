# OpenGL

`OpenGL` 学习项目。

## 开发平台

* windows11
* vs2022
* GLFW 3.3.6
* GLEW

## 环境配置

右键点击项目，进入 `properties`。

* 将 `Configuration` 设置为 `All Configurations`

### GLFW

* 在 `Configuration Properties` > `C/C++` > `General` > `Additional Include Directories` 添加 `$(SolutionDir)dependencies\glfw\include`
* 在 `Configuration Properties` > `Linker` > `General` > `Additional Library Directories` 添加 `$(SolutionDir)dependencies\glfw\lib-vc2022`
* 在 `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies` 添加 `glfw3.lib`

### GLEW

* 在 `Configuration Properties` > `C/C++` > `General` > `Additional Include Directories` 添加 `$(SolutionDir)dependencies\glew-2.1.0\include`
* 在 `Configuration Properties` > `Linker` > `General` > `Additional Library Directories` 添加 `$(SolutionDir)dependencies\glew-2.1.0\lib\Release\Win32`
* 在 `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies` 添加 `glew32s.lib`

#### 静态链接库

在 `lib` 文件夹下存在两个文件

``` text
glew32.lib
glew32s.lib
```

其中，`glew32.lib` 需要配合 `dll` 一起使用，`glew32s.lib` 用于静态链接。

## 编译

选中 `Application.cpp` 按 `ctrl + f7` 可以编译文件，但并不会链接。

## 构建解决方案

按 `f7` 会构建当前解决方案。

## 常见错误

* `1>Application.obj : error LNK2019: unresolved external symbol __imp__glClear@4 referenced in function _main`
  * `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies` 添加 `opengl32.lib`
* `1>glfw3.lib(win32_init.obj) : error LNK2019: unresolved external symbol __imp__TranslateMessage@4 referenced in function __glfwPlatformInit`
  * 在 `google.com` 中搜索 `TranslateMessage` 进入 `https://docs.microsoft.com/` 的链接，在最下方可以找到 `Library User32.lib`，将 `User32.lib` 添加到 `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies`。

## 使用 GLFW 构建三角形

### GlClear

[glClear](https://docs.microsoft.com/zh-cn/windows/win32/opengl/glclear) 函数将缓冲区清除为预设值，即重置缓冲区颜色。目标颜色由 [glClearColor](https://docs.microsoft.com/en-us/windows/win32/opengl/glclearcolor) 确定。例如，在调用 `glClear` 之前调用了 `glClearColor(1, 0, 0, 1)`，则缓冲区会被重置为红色。

### GlBegin

[glBegin & glEnd](https://docs.microsoft.com/en-us/windows/win32/opengl/glbegin) 函数之间包含若干基本的点。

`glBegin` 的函数参数表示如何呈现这些点。

``` c++
while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f( 0.0f,  0.5f);
    glVertex2f( 0.5f, -0.5f);
    glEnd();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}
```

## GLEW 从 0 开始

### Include

包含 `<GL/glew.h>` 文件时，`#include <GL/glew.h>` 必须写在其他有关 `OpenGL` 的头文件语句之前，否则会出现错误:

> 1>C:\Users\divinerapier\Documents\code\opengl\opengl\dependencies\glew-2.1.0\include\GL\glew.h(85,1): fatal error C1189: #error:  gl.h included before glew.h

因在，在 `<GL/glew.h>` 文件中会进行判断:

``` c
#if defined(__gl_h_) || defined(__GL_H__) || defined(_GL_H) || defined(__X_GL_H)
#error gl.h included before glew.h
#endif
```

### 初始化

根据 [文档](http://glew.sourceforge.net/basic.html) 可知，使用函数 `glewInit()` 函数进行初始化。但如果直接使用，会有链接错误:

``` c
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(void) {
    glewInit();
}
```

`1>Application.obj : error LNK2019: unresolved external symbol __imp__glewInit@0 referenced in function _main`。

跳转到 `glewInit()` 函数的源代码:

``` c
GLEWAPI GLenum GLEWAPIENTRY glewInit (void);
```

再跳转到 `GLEWAPI` 的宏定义:

``` c
#ifdef GLEW_STATIC
#  define GLEWAPI extern
#else
#  ifdef GLEW_BUILD
#    define GLEWAPI extern __declspec(dllexport)
#  else
#    define GLEWAPI extern __declspec(dllimport)
#  endif
#endif
```

可以发现，由于 `GLEW_STATIC` 与 `GLEW_BUILD` 均未定义，因此，会使用

``` c
define GLEWAPI extern __declspec(dllimport)
```

解决办法就是定义 `GLEW_STATIC`。

在 `Configuration Properties` > `C/C++` > `Preprocessor` > `Preprocessor Definitions` 中添加 `GLEW_STATIC`。

现在，就可以正常的编译过了。

接下来，还是根据 [文档](http://glew.sourceforge.net/basic.html) 可知，`glewInit()` 函数执行正确后会返回 `GLEW_OK`。所以，在函数入口出，初始化 `glfw` 之前初始化 `glew`:

``` c
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ohhhhhhhhh" << std::endl;
        return 1;
    }

    /* Initialize the library */
    if (!glfwInit())
        return -1;
}
```

但是很遗憾，初始化 `glew` 失败。

再回到文档，文档中说:

> First you need to create a valid OpenGL rendering context and call glewInit() to initialize the extension entry points.

要求先要初始化一个有效的 `OpenGL` 上下文，然后再初始化 `glew`。所以，应该将代码移动到 `glfwMakeContextCurrent(window);` 函数调用之后:

``` c
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ohhhhhhhhh" << std::endl;
        return 1;
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1, 0, 0, 1);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
```

### 如何使用更多功能

`GLEW` 提供强大的运行时机制，根据平台判断哪些 `OpenGL` 扩展功能可以使用。其头文件中包含了大量的函数指针，以函数 `glAttachShader` 为例，使用 `IDE` 层层跳转:

``` c
#define glAttachShader GLEW_GET_FUN(__glewAttachShader)

#ifndef GLEW_GET_FUN
#define GLEW_GET_FUN(x) x
#endif

GLEW_FUN_EXPORT PFNGLATTACHSHADERPROC __glewAttachShader;

typedef void (GLAPIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
```

最终，`glAttachShader` 其实是一个函数指针 `typedef void (GLAPIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);` 最终的功能由显示设备提供支持。

## 相关链接

* https://docs.microsoft.com/en-us/windows/win32/
* https://www.glfw.org/
* https://www.cnblogs.com/tjulym/p/5037124.html
* http://glew.sourceforge.net/