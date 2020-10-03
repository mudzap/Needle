layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_VP;

void main()
{
    TexCoords = aPos;
    vec4 pos = u_VP * vec4(aPos, 0.0);
    gl_Position = pos.xyww;
}