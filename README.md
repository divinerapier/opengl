# OpenGL

`OpenGL` 学习项目。

## 开发平台

* windows11
* vs2022
* GLFW 3.3.6

## 环境配置

右键点击项目，进入 `properties`。

* 将 `Configuration` 设置为 `All Configurations`
* 在 `Configuration Properties` > `C/C++` > `General` > `Additional Include Directories` 添加 `$(SolutionDir)dependencies\glfw\include`
* 在 `Configuration Properties` > `Linker` > `General` > `Additional Library Directories` 添加 `$(SolutionDir)dependencies\glfw\lib-vc2022`
* 在 `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies` 添加 `glfw3.lib`

## 编译

选中 `Application.cpp` 按 `ctrl + f7` 可以编译文件，但并不会链接。

## 构建解决方案

按 `f7` 会构建当前解决方案。

## 常见错误

* `1>Application.obj : error LNK2019: unresolved external symbol __imp__glClear@4 referenced in function _main`
  * `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies` 添加 `opengl32.lib`
* `1>glfw3.lib(win32_init.obj) : error LNK2019: unresolved external symbol __imp__TranslateMessage@4 referenced in function __glfwPlatformInit`
  * 在 `google.com` 中搜索 `TranslateMessage` 进入 `https://docs.microsoft.com/` 的链接，在最下方可以找到 `Library User32.lib`，将 `User32.lib` 添加到 `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies`。

## OpenGL 应用

### 构建三角形

#### GlClear

[glClear](https://docs.microsoft.com/zh-cn/windows/win32/opengl/glclear) 函数将缓冲区清除为预设值，即重置缓冲区颜色。目标颜色由 [glClearColor](https://docs.microsoft.com/en-us/windows/win32/opengl/glclearcolor) 确定。例如，在调用 `glClear` 之前调用了 `glClearColor(1, 0, 0, 1)`，则缓冲区会被重置为红色。

#### GlBegin

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

## 相关链接

* https://docs.microsoft.com/en-us/windows/win32/
* https://www.glfw.org/
* https://www.cnblogs.com/tjulym/p/5037124.html
