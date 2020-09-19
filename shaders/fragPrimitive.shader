#version 330 core

layout(location = 0) out vec4 gl_Color;

in vec2 v_TexCoord;
//in int v_TexIndex;

uniform sampler2D u_Texture;
uniform int r_to_a;
    
void main() {
    gl_Color = mix(texture(u_Texture, v_TexCoord), vec4(1.0, 1.0, 1.0, texture(u_Texture, v_TexCoord).r), (r_to_a == 1));
}   