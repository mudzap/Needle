#version 330 core

layout(location = 0) out vec4 gl_Color;
//layout(location = 1) out vec4 BrightColor;

in VertexData{
	vec3 v_FragPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
	vec2 v_TexCoord;
	mat3 v_TBN;
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
uniform samplerCube skybox;

void main() {

	//AMBIENT
	vec3 lightColor = vec3(0.8, 0.8, 1.0);
	vec3 objectColor = texture(u_Texture, v_In.v_TexCoord).rgb;
	//vec3 objectColor = texture(u_Normal, v_In.v_TexCoord).rgb;
	vec3 ambient = m_In.v_Ka * lightColor;	

	//NORMALS
	vec4 mapping = texture(u_Normal, v_In.v_TexCoord);
	mapping = mapping * 2.0 - 1.0;
	mapping.y = -mapping.y;
	vec3 normal = normalize(mapping.rgb);
	vec3 lightDir = normalize(v_In.TangentLightPos - v_In.TangentFragPos);
	vec3 viewDir = normalize(v_In.TangentViewPos - v_In.TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//DIFFUSE
	float diff = max(dot(lightDir, normal), 0.0); //normal diffuse
	//float diff = max( sign( max(dot(lightDir, normal), 0.0) ), 0.0 ) + 0.2; //cartoon
	vec3 diffuse = m_In.v_Kd * diff * lightColor;

	
	//SPECULAR
	float spec = pow(max(dot(lightDir, halfwayDir), 0.0), m_In.v_Ns) * mapping.a;
	vec3 specular = m_In.v_Ks * spec * lightColor; 

	//REFLECTION
	const float reflectionStrength = 0.3;
	vec3 refl = reflect(viewDir, normal);

	//vec3 result = (ambient + diffuse) * objectColor + texture(skybox, refl).rgb * reflectionStrength * (specular + 1);
	vec3 result = (ambient + diffuse) * objectColor + specular;
	//vec3 result = (ambient + diffuse) * objectColor; //cartoon
	gl_Color = vec4(result, 1.0);

}