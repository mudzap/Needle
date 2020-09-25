#version 330 core

layout(location = 0) out vec4 gl_Color;
//layout(location = 1) out vec4 BrightColor;

in VertexData{
	vec3 v_FragPos;
	vec3 v_Normal;
	vec3 v_Tangent;
	vec3 v_Bitangent;
	vec2 v_TexCoord;
} v_In;

in MaterialData{
	vec3 v_Ka;
	vec3 v_Kd;
	vec3 v_Ks;
	float v_Ns;
	float v_Tr;
} m_In;

uniform sampler2D u_Texture;
uniform vec3 lightPos;
//uniform vec3 viewPos;
uniform samplerCube skybox;

void main() {


	vec3 viewPos = lightPos;
	//vec3 LightPos = lightPos + vec3(0.0, 0.0, 0.0);
	vec3 LightPos = vec3(0.0, 10000.0, 0.0);

	vec3 lightColor = vec3(0.8, 0.8, 1.0);

	//AMBIENT
	vec3 objectColor = vec3(texture(u_Texture, v_In.v_TexCoord));
	vec3 ambient = m_In.v_Ka * lightColor;

	
	//NORMALS
	vec3 norm = normalize(v_In.v_Normal);
	vec3 lightDir = normalize(LightPos - v_In.v_FragPos); // = viewDir, for now
	vec3 viewDir = normalize(viewPos - v_In.v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	//DIFFUSE
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = m_In.v_Kd * diff * lightColor;

	//SPECULAR
	float spec = pow(max(dot(lightDir, reflectDir), 0.0), m_In.v_Ns);
	vec3 specular = m_In.v_Ks * spec * lightColor;

	//REFLECTION
	float reflectionStrength = 0.2;
	vec3 refl = reflect(viewDir, norm);

	//REFRACTION
	float ratio = 1.00 / 1.1;
	vec3 refr = refract(-viewDir, norm, ratio);

	vec3 result = (ambient + diffuse + specular) * objectColor + mix(reflectionStrength * texture(skybox, refl).rgb, m_In.v_Tr * texture(skybox, refr).rgb, 0.5);

	//GAMMA CORRECTION
	//float gamma = 2.0;
	//vec3 Result = pow(result, vec3(1.0 / gamma));
	gl_Color = vec4(result, 1.0);

	/*
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	float shouldBloom = max(0.0, sign(brightness - 0.9));
	BrightColor += vec4(shouldBloom);
	*/

	/*
	if (brightness > 0.9)
		BrightColor = vec4(result, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
	*/

}