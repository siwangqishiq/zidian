layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aColor;

out vec4 vColor;

void main() {
    gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    gl_PointSize = 10.0f; // 点大小
    vColor = aColor;
}
