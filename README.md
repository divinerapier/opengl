# OpenGL

`OpenGL` ѧϰ��Ŀ��

## ����ƽ̨

* windows11
* vs2022
* GLFW 3.3.6

## ��������

�Ҽ������Ŀ������ `properties`��

* �� `Configuration` ����Ϊ `All Configurations`
* �� `Configuration Properties` > `C/C++` > `General` > `Additional Include Directories` ��� `$(SolutionDir)dependencies\glfw\include`
* �� `Configuration Properties` > `Linker` > `General` > `Additional Library Directories` ��� `$(SolutionDir)dependencies\glfw\lib-vc2022`
* �� `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies` ��� `glfw3.lib`

## ����

ѡ�� `Application.cpp` �� `ctrl + f7` ���Ա����ļ��������������ӡ�

## �����������

�� `f7` �ṹ����ǰ���������

## ��������

* `1>Application.obj : error LNK2019: unresolved external symbol __imp__glClear@4 referenced in function _main`
  * `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies` ��� `opengl32.lib`
* `1>glfw3.lib(win32_init.obj) : error LNK2019: unresolved external symbol __imp__TranslateMessage@4 referenced in function __glfwPlatformInit`
    * �� `google.com` ������ `TranslateMessage` ���� `https://docs.microsoft.com/` �����ӣ������·������ҵ� `Library User32.lib`���� `User32.lib` ��ӵ� `Configuration Properties` > `Linker` > `Input` > `Additional Dependencies`��

## �������

* https://docs.microsoft.com/en-us/windows/win32/
* https://www.glfw.org/
