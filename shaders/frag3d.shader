#version 330 core

layout(location = 0) out vec4 gl_Color;
//layout(location = 1) out vec4 BrightColor;

in VertexData{
	vec3 v_FragPos;
	mat3 v_TBN;
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
uniform sampler2D u_Normal;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform samplerCube skybox;

void main() {

	//AMBIENT
	vec3 lightColor = vec3(0.8, 0.8, 1.0);
	vec3 objectColor = texture(u_Texture, v_In.v_TexCoord).rgb;
	vec3 ambient = m_In.v_Ka * lightColor;

	//NORMALS
	vec3 normal = normalize( v_In.v_TBN * (texture(u_Normal, v_In.v_TexCoord) * 2.0 - 1).rgb );
	vec3 lightDir = normalize(lightPos - v_In.v_FragPos);
	vec3 viewDir = normalize(viewPos - v_In.v_FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	//DIFFUSE
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = m_In.v_Kd * diff * lightColor;
	
	//SPECULAR
	float spec = pow(max(dot(lightDir, reflectDir), 0.0), m_In.v_Ns);
	vec3 specular = m_In.v_Ks * spec * lightColor;

	//REFLECTION
	const float reflectionStrength = 0.5;
	vec3 refl = reflect(viewDir, normal);


	vec3 result = (ambient + diffuse + specular) * objectColor + texture(skybox, refl).rgb * reflectionStrength;
	gl_Color = vec4(result, 1.0);

}