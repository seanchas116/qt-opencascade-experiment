in vec3 normal_cameraSpace;

out vec4 fragColor;

const vec3 lightDirection = vec3(0, 0, 1);
const vec3 diffuse = vec3(1, 0, 0);

void main(void) {
    vec3 normalDirection = normalize(normal_cameraSpace);

    float diffuseStrength = clamp(dot(normalDirection, lightDirection), 0.0, 1.0);

    fragColor = vec4(diffuse * diffuseStrength, 1);
}
