layout(location = 0) in vec2 aPos;
layout(location = 1) in vec4 aColor;

uniform mat3 uScreenToNdcMat;

out vec4 vColor;

void main() {
    gl_Position = vec4(uScreenToNdcMat * vec3(aPos, 1.0f), 1.0f);
    gl_PointSize = 50.0f; // 点大小
    vColor = aColor;
}
