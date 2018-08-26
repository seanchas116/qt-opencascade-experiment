layout(location = 0) in vec3 position_modelSpace;

uniform mat4 MVP;

void main(void) {
    gl_Position = MVP * vec4(position_modelSpace, 1) + vec4(0, 0, -0.001, 0);
}
