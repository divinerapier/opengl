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

## GLEW

### 从 0 开始

#### Include

包含 `<GL/glew.h>` 文件时，`#include <GL/glew.h>` 必须写在其他有关 `OpenGL` 的头文件语句之前，否则会出现错误:

> 1>C:\Users\divinerapier\Documents\code\opengl\opengl\dependencies\glew-2.1.0\include\GL\glew.h(85,1): fatal error C1189: #error:  gl.h included before glew.h

因在，在 `<GL/glew.h>` 文件中会进行判断:

``` c
#if defined(__gl_h_) || defined(__GL_H__) || defined(_GL_H) || defined(__X_GL_H)
#error gl.h included before glew.h
#endif
```

#### 初始化

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

    // TODO: 正式工作
}
```

#### 使用更多功能

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

### GLEW 绘制三角形

使用 `GLEW` 绘制图形需要自定义 `Shader` 渲染图形。

#### 数据

在 `GLEW` 正确初始化后，可以开始准备数据工作。

描述 `2D` 三角形一共需要 **3 个点**，每个点的坐标由 **2 个 float** 组成:

``` c
float positions[6] = {
    -0.5f, -0.5f,
     0.0f,  0.5f,
     0.5f, -0.5f
};
```

然后，将点的坐标数据并定到 `OpenGL` 的缓冲区中。

`glGenBuffers` 函数可以创建若干的**命名缓冲区 `(generate buffer object names)`**，由于这里只需要显示一个三角形，所以第一个参数 `n` 传入 **1**，表示生成一个缓冲区，如果需要生成多个，则需要传入一个已分配好内存的数组。

一个缓冲区:

``` c
unsigned int buffer = 0;
// https://docs.gl/gl4/glGenBuffers
glGenBuffers(1, &buffer);
```

多个缓冲区:

``` c
unsigned int buffers[4] = { 0 };
glGenBuffers(sizeof(buffers) / sizeof(buffers[0]), buffers);
```

缓冲区的本质只是一段内存，而在 `OpenGL` 中更是仅仅使用一个 `unsigned int` 类型作为描述符(OpenGL 称之为**名称**)，因此，需要将缓冲区与某种特定的类型绑定，`GPU` 才能由办法解释缓冲区数据(同一段内存，使用 `int` 与 `float` 解释的结果是不同的)。

`glBindBuffer` 函数接收一种缓冲区目标与一个缓冲区名称。当缓冲区名称不存在时，会自动创建一个同名缓冲区。并且，同一时间最多只能有一个缓冲区被绑定到目标上 **(When a buffer object is bound to a target, the previous binding for that target is automatically broken)**。

当成功绑定缓冲区的指定目标上之后，使用 `glBufferData` 函数用来创建并初始化缓冲区数据。

``` c
// https://docs.gl/gl4/glBindBuffer
glBindBuffer(GL_ARRAY_BUFFER, buffer);
// https://docs.gl/gl4/glBufferData
glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), (const void*)(&positions), GL_STATIC_DRAW);
```

最后，启用顶点:

* `index`: 这个目前还不明白，只知道要与 `glEnableVertexAttribArray` 参数同时为 `0`
* `size`: 每个顶点包含的元素数量，包括 **position(位置)**, **normal(法线)**, **color(颜色)**, 和 **texture coordinates(纹理坐标)**，可取值 **(1,2,3,4)**。
* `type`: 每个元素的数据类型，`GL_FLOAT` 表示以 `float` 为单位_
* `normalized`: 是否为向量(只有方向无大小)
* `stride`: 连续的顶点属性之间的字节偏移间隔
* `pointer`: 数据偏移量

``` c
glEnableVertexAttribArray(0);

// https://docs.gl/gl4/glVertexAttribPointer
// 
// size: 每个顶点元素个数 (此处每个顶点包含 2 个 float)
// type: 每个元素的数据类型 (此处为 float)
// stride: 顶点位置之间的距离(字节数)
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
```

最后，完整代码如下:

``` c
    if (glewInit() != GLEW_OK) {
        std::cout << "ohhhhhhhhh" << std::endl;
        return 1;
    }

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };


    unsigned int buffer = 0;
    // https://docs.gl/gl4/glGenBuffers
    glGenBuffers(1, &buffer);
    // https://docs.gl/gl4/glBindBuffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // https://docs.gl/gl4/glBufferData
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), (const void*)(&positions), GL_STATIC_DRAW);
    // https://docs.gl/gl4/glEnableVertexAttribArray
    glEnableVertexAttribArray(0);


    // https://docs.gl/gl4/glVertexAttribPointer
    // 
    // size: 每个顶点元素个数 (此处每个顶点包含 2 个 float)
    // type: 每个元素的数据类型 (此处为 float)
    // stride: 顶点位置之间的距离(字节数)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

```

#### Shader

图形渲染由 `GPU` 负责，因此，需要对 `GPU` 编程，指示 `GPU` 在哪里画点，如何显示颜色等。这就是 `Shader` 的作用。

`Shader` 的本质是一段运行在 `GPU` 上的**程序**，正如一般的 `c/c++` 编写的程序告诉 `CPU` 如何工作一样，`OpenGL` 提供的 `GLSL` 语言编写的 `Shader` 程序告诉 `GPU` 如何工作(渲染)。

本次将使用两种着色器 `VertexShader (顶点着色器)`，`FragmentShader (片段着色器)`。

* `VertexShader` 作用于每个顶点，通常是处理从世界空间到裁剪空间(屏幕坐标)的坐标转换，后接`光栅化`。
* `FragmentShader` 作用于每个屏幕上的片元(可近似理解为像素)，通常是计算颜色。

##### 创建 `Shader`

`Shader` 的本质是一个**程序**，也需要类似于源码，编译等流程。

声明两个字符串变量，分别代表 `VertexShader` 与 `FragmentShader` 的源代码:

``` c
std::string vertexShader =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec4 position;\n"
    "\n"
    "void main() {\n"
    "    gl_Position = position;\n"
    "}\n"
    "";
```

1. `layout(location = 0)` 表示使用 `glVertexAttribPointer` 第一个参数 `index=0` 对应的数据。
2. `in vec4 position;` 中使用 `vec4` 是因为后续的 `gl_Position` 是 `vec4` 类型，虽然实际每个顶点是 `vec2`(`glVertexAttribPointer` 的第二个参数 `size=2`)类型
3. `in vec4 position;` 中的 `position` 表示每一个顶点的 `position` 属性部分

``` c
std::string fragmentShader =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) out vec4 color;\n" // location = 0 对应 glVertexAttribPointer 的第一个参数 index 传 0, vec4 是因为 gl_Position 是一个 vec4，虽然 glVertexAttribPointer 的第二个参数传 2 (即 vec2)，但即使使用了 vec2，在 gl_Position 处还是要转换为 vec4
    "\n"
    "void main() {\n"
    "    color = vec4(0.0, 1.0, 0.0, 1.0);\n" // 0: 黑色 1: 白色 范围: 0-1 (类比 0-255) 顺序: rgba
    "}\n"
    "";
```

编译 `Shader` 源码:

``` c
static unsigned int ComplieShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // https://docs.gl/gl4/glShaderSource
    // void glShaderSource(GLuint shader, GLsizei count, const GLchar * *string, const GLint * length);
    // 
    // sets the source code in shader to the source code in the array of strings specified by string. 
    // 替换 Shader 中的代码
    // 
    // id: Shader 对象的描述符。
    // 
    // count: string 数组的长度
    // 
    // string: 代码数组，类型为 const char**，因此，可以传入多个 Shader 代码，数量为 count
    // 
    // length: 长度数组，类型为 const int*，数组中的每个元素表示对应 string 参数元素的字符串长度，NULL 表示字符串以 NULL 结束
    // If length is NULL, each string is assumed to be null terminated.
    // 
    // The source code strings are not scanned or parsed at this time; they are simply copied into the specified shader object.
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    // 错误处理
    if (GL_FALSE == result) {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout <<
            "Failed compile " <<
            ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") <<
            " shader: " << source <<
            " error: " << message <<
            std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

// CreateProgram 输入 Shader 源码，返回相应的 Shader 程序
static unsigned int CreateProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = ComplieShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = ComplieShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
```

最后，在 `c/c++` 的源码中调用:

``` c
unsigned int program = CreateProgram(vertexShader, fragmentShader);
glUseProgram(program);

// ...

glDeleteProgram(program);
return 0;
```

#### 暂不清楚

1. `Vertex` 有哪些属性(Attributes)
2. 函数 `glVertexAttribPointer` 的第一个参数需要开发者自行维护从 `0` 开始自增吗?
3. `glDeleteShader` 与 `glDetachShader` 的区别，如何管理 `Shader`

## 相关链接

* [Windows API](https://docs.microsoft.com/en-us/windows/win32/)
* [GLFW](https://www.glfw.org/)
* https://www.cnblogs.com/tjulym/p/5037124.html
* [GLEW](http://glew.sourceforge.net/)
* [OpenGL API](https://docs.gl/)
