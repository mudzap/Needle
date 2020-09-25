#version 330 core

layout(location = 0) out vec4 gl_Color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture;

void main() {
	float a = texture(u_Texture, v_TexCoord).r;
	gl_Color = vec4(v_Color.rgb, a * v_Color.a);
}