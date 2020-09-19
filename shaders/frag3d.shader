#version 330 core

layout(location = 0) out vec4 gl_Color;

in vec2 v_TexCoord;
in vec3 v_normal;
in vec3 v_fragPos;

uniform sampler2D u_Texture;
uniform vec3 lightPos;
uniform samplerCube skybox;

void main() {

	//float gamma = 2.2;

	float ambientStrength = 0.2;
	vec3 objectColor = vec3(texture(u_Texture, v_TexCoord));
	vec3 lightColor = vec3(1.0, 0.5, 0.5);
	

	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(lightPos - v_fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 ambient = ambientStrength * lightColor;

	//vec3 I = normalize(v_fragPos - lightPos);
	vec3 R = reflect(lightDir, norm);

	vec3 result = (ambient + diffuse) * objectColor + texture(skybox, R).rgb * 0.3;

	//GAMMA CORRECTION
	//vec3 Result = pow(result, vec3(1.0 / gamma));
	gl_Color = vec4(result, 1.0);

}