#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 Ka;
layout(location = 4) in vec3 Kd;
layout(location = 5) in vec3 Ks;
layout(location = 6) in float Ns;
layout(location = 7) in float Tr;

out vec2 v_TexCoord;
out vec3 v_normal;
out vec3 v_fragPos;
out vec3 v_Ka;
out vec3 v_Kd;
out vec3 v_Ks;
out float v_Ns;
out float v_Tr;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main() {

	v_normal = vec3(u_Model * vec4(normals, 0.0));
	v_TexCoord = texCoord;

	vec4 temp = u_Model * vec4(position, 1.0);

	v_fragPos = temp.xyz;

	v_Ka = Ka;
	v_Kd = Kd;
	v_Ks = Ks;
	v_Ns = Ns;
	v_Tr = Tr;

	gl_Position = u_VP * temp;
}