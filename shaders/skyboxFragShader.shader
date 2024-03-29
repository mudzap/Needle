layout(location = 0) out vec4 fragColor;
//layout(location = 1) out vec4 BrightColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    fragColor = texture(skybox, TexCoords);
}