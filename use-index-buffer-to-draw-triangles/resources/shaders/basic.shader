#shader vertex
#version 330 core

// location = 0 对应 glVertexAttribPointer 的第一个参数 index 传 0, vec4 是因为 gl_Position 是一个 vec4，
// 虽然 glVertexAttribPointer 的第二个参数传 2 (即 vec2)，但即使使用了 vec2，在 gl_Position 处还是要转换为 vec4
layout(location = 0) in vec4 position;

void main() {
    gl_Position = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
    // 0: 黑色 1: 白色 范围: 0-1 (类比 0-255) 顺序: rgba
    color = vec4(0.0, 1.0, 0.0, 1.0);
}
