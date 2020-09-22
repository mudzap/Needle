#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform float scale;

void main() {
	v_TexCoord = texCoord;
	gl_Position = u_Projection * vec4(position, 0.0, 1.0 / scale);
}