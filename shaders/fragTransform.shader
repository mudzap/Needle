#version 330 core

layout(location = 0) out vec4 gl_Color;

in vec2 v_TexCoord;
//in int v_TexIndex;

uniform sampler2D u_Texture;

void main() {
    gl_Color = texture(u_Texture, v_TexCoord);
}