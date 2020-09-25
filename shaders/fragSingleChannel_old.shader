#version 330 core

layout(location = 0) out vec4 gl_Color;

in vec2 v_TexCoord;
//in int v_TexIndex;

uniform vec4 v_Color;
uniform sampler2D u_Texture;

void main() {
	gl_Color = vec4(v_Color.r, v_Color.g, v_Color.b, texture(u_Texture, v_TexCoord).r * v_Color.a);
}