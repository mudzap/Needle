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
	//vec4 mapping = texture(u_Normal, v_In.v_TexCoord);
	vec3 mapping = texture(u_Normal, v_In.v_TexCoord).rgb * 2.0 - 1.0;
	mapping.y = -mapping.y;
	vec3 normal = normalize(mapping);
	vec3 lightDist = v_In.TangentLightPos - v_In.TangentFragPos;
	vec3 lightDir = normalize(lightDist);
	float distance2 = lightDist.x * lightDist.x + lightDist.y * lightDist.y + lightDist.z * lightDist.z;;
	float distanceFade = 1 / (1.0 + (0.0001 * distance2));
	vec3 viewDir = normalize(v_In.TangentViewPos - v_In.TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	//DIFFUSE
	float diff = max(dot(lightDir, normal), 0.0);
	float distdiff = diff * distanceFade;
	vec3 diffuse = m_In.v_Kd * distdiff * lightColor;
	
	//SPECULAR
	//float spec = pow(max(dot(halfwayDir, normal), 0.0), m_In.v_Ns) * mapping.a;
	float spec = pow(max(dot(halfwayDir, normal), 0.0), m_In.v_Ns);
	vec3 specular = m_In.v_Ks * spec * lightColor;

	//RIM
	float rim = 1 - max(dot(viewDir, normal), 0.0);
	rim = smoothstep(0.9, 1.0, rim);
	vec3 RimColor = vec3(0.2) * lightColor;
	vec3 finalRim = RimColor * vec3(rim);

	//REFLECTION
	const float reflectionStrength = 0.5;
	vec3 refl = reflect(viewDir, normal);

	vec3 result = (ambient + diffuse + finalRim) * objectColor + specular * (texture(skybox, refl).rgb * reflectionStrength + 1);
	//vec3 result = (ambient + diffuse) * objectColor; //cartoon
	gl_Color = vec4(result, 1.0);

}