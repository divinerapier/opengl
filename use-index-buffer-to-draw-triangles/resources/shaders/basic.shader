#shader vertex
#version 330 core

// location = 0 ��Ӧ glVertexAttribPointer �ĵ�һ������ index �� 0, vec4 ����Ϊ gl_Position ��һ�� vec4��
// ��Ȼ glVertexAttribPointer �ĵڶ��������� 2 (�� vec2)������ʹʹ���� vec2���� gl_Position ������Ҫת��Ϊ vec4
layout(location = 0) in vec4 position;

void main() {
    gl_Position = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
    // 0: ��ɫ 1: ��ɫ ��Χ: 0-1 (��� 0-255) ˳��: rgba
    color = vec4(0.0, 1.0, 0.0, 1.0);
}
