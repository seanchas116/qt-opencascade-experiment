in vec3 inPosition;
in vec2 inTexCoords;
in float inNormal;
uniform mat4 mvpMatrix;
out vec4 texCoords;

void main(void) {
    gl_Position = mvpMatrix * inPosition;
    texCoords = inTexCoords;
}
