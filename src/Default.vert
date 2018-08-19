layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;
layout(location = 2) in float inNormal;
uniform mat4 MVP;
out vec2 texCoords;

void main(void) {
    gl_Position = MVP * vec4(inPosition, 1);
    texCoords = inTexCoords;
}
