#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderSources {
    std::string vertex_shader;
    std::string fragment_shader;
};

static ShaderSources LoadShader(std::string filename) {
    std::fstream input(filename);
    std::string line;
    std::stringstream ss[2];

    enum ShaderType {
        None = -1,
        Vertex = 0,
        Fragment = 1,
    };
    ShaderType type = ShaderType::None;

    while (getline(input, line)) {
        if (line.starts_with("#shader")) {
            if (line.ends_with("vertex")) {
                type = ShaderType::Vertex;
            } else if (line.ends_with("fragment")) {
                type = ShaderType::Fragment;
            } else {
                std::cout << "Unknown Shader: " << line << std::endl;
            }
            continue;
        }
        ss[(int)type] << line << std::endl;
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int ComplieShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // https://docs.gl/gl4/glShaderSource
    // void glShaderSource(GLuint shader, GLsizei count, const GLchar * *string, const GLint * length);
    // 
    // sets the source code in shader to the source code in the array of strings specified by string. 
    // �滻 Shader �еĴ���
    // 
    // id: Shader �������������
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

// CreateProgram ���� Shader Դ�룬������Ӧ�� Shader ����
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

    float positions[] = {
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };

    unsigned int indexes[] = {
        0, 1, 2,
        2, 3, 0
    };

    // ���ݲ��� 
    unsigned int buffer = 0;
    // https://docs.gl/gl4/glGenBuffers
    glGenBuffers(1, &buffer);
    // https://docs.gl/gl4/glBindBuffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // https://docs.gl/gl4/glBufferData
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), (const void*)(&positions), GL_STATIC_DRAW);
    // https://docs.gl/gl4/glEnableVertexAttribArray
    glEnableVertexAttribArray(0);


    // https://docs.gl/gl4/glVertexAttribPointer
    // 
    // size: ÿ������Ԫ�ظ��� (�˴�ÿ��������� 2 �� float)
    // type: ÿ��Ԫ�ص��������� (�˴�Ϊ float)
    // stride: ����λ��֮��ľ���(�ֽ���)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);


    // index buffer
    unsigned int ibo; // index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indexes, GL_STATIC_DRAW);

    ShaderSources sources = LoadShader("./resources/shaders/basic.shader");
    unsigned int program = CreateProgram(sources.vertex_shader, sources.fragment_shader);
    glUseProgram(program);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glAttachShader(0, 0);
        glClearColor(1, 0, 0, 1);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6/*indexes ������*/, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    glDeleteProgram(program);

    return 0;
}
