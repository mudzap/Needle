layout(location = 0) out vec4 gl_Color;
//layout(location = 1) out vec4 BrightColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    gl_Color = texture(skybox, TexCoords);
}