#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec2 translation;
layout(location = 3) in mat4 transform;


out vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform float scale;

void main() {
	v_TexCoord = texCoord;
	//mat4 MVP = u_Projection * transform;
	//gl_Position = MVP * vec4(position + translation, 0.0, 1.0);
	vec4 temp = transform * vec4(position, 0.0, 0.0);
	gl_Position = u_Projection * (temp + vec4(translation, 0.0, 1.0/scale));
}