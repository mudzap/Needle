#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec2 translation;
layout(location = 3) in vec2 rotComplex;


out vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform float scale;

void main() {
	v_TexCoord = texCoord;
	mat2 transform;

	transform[0].xy = vec2(rotComplex.x, rotComplex.y); //TRANSPOSED BECAUSE GLSL
	transform[1].xy = vec2(-rotComplex.y, rotComplex.x);

	gl_Position = u_Projection * vec4(transform * position + translation, 0.0, 1.0/scale);
}