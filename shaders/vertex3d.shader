#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoord;


out vec2 v_TexCoord;
out vec3 v_normal;
out vec3 v_fragPos;

uniform mat4 u_VP;
uniform mat4 u_Model;


void main() {

	v_normal = vec3(u_Model * vec4(normals, 0.0));
	v_TexCoord = texCoord;

	vec4 temp = u_Model * vec4(position.x, position.y, position.z, 1.0);

	v_fragPos = vec3(temp);
	gl_Position = u_VP * temp;
}