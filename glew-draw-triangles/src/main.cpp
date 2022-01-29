#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int ComplieShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // https://docs.gl/gl4/glShaderSource
    // void glShaderSource(GLuint shader, GLsizei count, const GLchar * *string, const GLint * length);
    // 
    // sets the source code in shader to the source code in the array of strings specified by string. 
    // �滻 Shader �еĴ���
    // 
    // id: Shader ����ľ����
    // 
    // count: string ����ĳ���
    // 
    // string: �������飬����Ϊ const char**����ˣ����Դ����� Shader ���룬����Ϊ count
    // 
    // length: �������飬����Ϊ const int*�������е�ÿ��Ԫ�ر�ʾ��Ӧ string ����Ԫ�ص��ַ������ȣ�NULL ��ʾ�ַ����� NULL ����
    // If length is NULL, each string is assumed to be null terminated.
    // 
    // The source code strings are not scanned or parsed at this time; they are simply copied into the specified shader object.
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    // ������
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



// CreateShader ������ɫ��������������ɫ��Դ����
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "ohhhhhhhhh" << std::endl;
        return 1;
    }

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // ���ݲ��� 
    unsigned int buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // https://docs.gl/gl4/glBufferData
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), (const void*)(&positions), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);


    // https://docs.gl/gl4/glVertexAttribPointer
    // 
    // size: ÿ������Ԫ�ظ��� (�˴�ÿ��������� 2 �� float)
    // type: ÿ��Ԫ�ص��������� (�˴�Ϊ float)
    // stride: ����λ��֮��ľ���(�ֽ���)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n" // location = 0 ��Ӧ glVertexAttribPointer �ĵ�һ������ index �� 0, vec4 ����Ϊ gl_Position ��һ�� vec4����Ȼ glVertexAttribPointer �ĵڶ��������� 2 (�� vec2)������ʹʹ���� vec2���� gl_Position ������Ҫת��Ϊ vec4
        "\n"
        "void main() {\n"
        "    gl_Position = position;\n"
        "}\n"
        "";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n" // location = 0 ��Ӧ glVertexAttribPointer �ĵ�һ������ index �� 0, vec4 ����Ϊ gl_Position ��һ�� vec4����Ȼ glVertexAttribPointer �ĵڶ��������� 2 (�� vec2)������ʹʹ���� vec2���� gl_Position ������Ҫת��Ϊ vec4
        "\n"
        "void main() {\n"
        "    color = vec4(0.0, 1.0, 0.0, 1.0);\n" // 0: ��ɫ 1: ��ɫ ��Χ: 0-1 (��� 0-255) ˳��: rgba
        "}\n"
        "";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glAttachShader(0, 0);
        glClearColor(1, 0, 0, 1);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    glDeleteProgram(shader);

    return 0;
}
