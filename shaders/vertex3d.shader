#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec3 bitangent;
layout(location = 4) in vec2 texCoord;
layout(location = 5) in vec3 Ka;
layout(location = 6) in vec3 Kd;
layout(location = 7) in vec3 Ks;
layout(location = 8) in float Ns;
layout(location = 9) in float Tr;

out VertexData{
	vec3 v_FragPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	vec2 v_TexCoord;
	mat3 v_TBN;
} v_Out;

out MaterialData{
	vec3 v_Ka;
	vec3 v_Kd;
	vec3 v_Ks;
	float v_Ns;
	float v_Tr;
} m_Out;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat4 u_VP;

void main() {

	mat3 TBN = transpose(mat3(tangent, bitangent, normals));

	v_Out.v_FragPos = position;
	v_Out.v_TexCoord = texCoord;
	v_Out.TangentLightPos = TBN * lightPos;
	v_Out.TangentViewPos = TBN * viewPos;
	v_Out.TangentFragPos = TBN * v_Out.v_FragPos;
	v_Out.v_TBN = TBN;

	m_Out.v_Ka = Ka;
	m_Out.v_Kd = Kd;
	m_Out.v_Ks = Ks;
	m_Out.v_Ns = Ns;
	m_Out.v_Tr = Tr;

	gl_Position = u_VP * vec4(position, 1.0);
}