#define _USE_SPECULAR_
#define _USE_RIM_LIGHTING_
#define _USE_REFLECTIONS_

#define _GAMMA_ 2.0

layout(location = 0) out vec4 fragColor;
//layout(location = 1) out vec4 BrightColor;

in VertexData{
	vec3 v_FragPos;
	vec3 TangentLightPos;
	vec3 TangentLightDir;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
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

#ifdef _USE_REFLECTIONS_
uniform samplerCube skybox;
#endif

void main() {

	//AMBIENT
	vec3 lightColor = vec3(0.8, 0.8, 1.0);
	vec3 objectColor = texture(u_Texture, v_In.v_TexCoord).rgb;
	vec3 ambient = m_In.v_Ka * lightColor;	

	//NORMALS
	//vec4 mapping = texture(u_Normal, v_In.v_TexCoord);
	vec3 mapping = texture(u_Normal, v_In.v_TexCoord).rgb * 2.0 - 1.0;
	mapping.y = -mapping.y;
	vec3 normal = normalize(mapping);

	vec3 lightDist = v_In.TangentLightPos - v_In.TangentFragPos;
	vec3 playerLightDir = normalize(lightDist);
	vec3 lightDir = v_In.TangentLightDir;

	float distance2 = lightDist.x * lightDist.x + lightDist.y * lightDist.y + lightDist.z * lightDist.z;;
	float distanceFade = 1 / (1.0 + (0.001 * distance2));

	vec3 viewDir = normalize(v_In.TangentViewPos - v_In.TangentFragPos);

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 playerHalfwayDir = normalize(playerLightDir + viewDir);

	//DIFFUSE
	//float diff = max(dot(lightDir, normal), 0.0);
	//float playerDiff = max(dot(playerLightDir, normal), 0.0);
	float diff = pow(0.5*dot(lightDir, normal) + 0.5, _GAMMA_);
	float playerDiff = pow(0.5*dot(playerLightDir, normal) + 0.5, _GAMMA_) * 0.5;
	vec3 diffuse = m_In.v_Kd * lightColor * (diff + playerDiff * distanceFade);
	
#ifdef _USE_SPECULAR_
	//SPECULAR
	//float spec = pow(max(dot(halfwayDir, normal), 0.0), m_In.v_Ns) * mapping.a;
	//float spec = pow(max(dot(halfwayDir, normal), 0.0), m_In.v_Ns);
	float spec = pow(0.5 * dot(halfwayDir, normal) + 0.5, m_In.v_Ns);
	//float playerSpec = pow(max(dot(playerHalfwayDir, normal), 0.0), m_In.v_Ns);
	float playerSpec = pow(0.5 * dot(playerHalfwayDir, normal) + 0.5, m_In.v_Ns);
	vec3 specular = m_In.v_Ks * lightColor * (spec + playerSpec);
#endif

#ifdef _USE_RIM_LIGHTING_
	//RIM LIGHTING
	float rim = 1.0 - max(dot(viewDir, normal), 0.0);
	float upperRim = 1.0 - max(dot(vec3(0.0, 1.0, 0.0), normal), 0.0);
	//vec3 finalRim = pow(max(rim, upperRim * 0.85), m_In.v_Ns) * lightColor * m_In.v_Ks;
	vec3 finalRim = pow(rim, m_In.v_Ns) * lightColor * m_In.v_Ks;
#endif

#if defined _USE_REFLECTIONS_ && defined _USE_SPECULAR_
	//REFLECTIONS
	const float reflectionStrength = 1.0;
	vec3 refl = reflect(viewDir, normal);
#endif


	//FUCK BRANCHING
#if defined _USE_SPECULAR_ && defined _USE_RIM_LIGHTING_ && defined _USE_REFLECTIONS_ 
	vec3 result = (finalRim + ambient + diffuse) * objectColor + specular * (texture(skybox, refl).rgb * reflectionStrength + 1);
	//vec3 result = (ambient + diffuse + finalRim) * objectColor + (texture(skybox, refl).rgb * reflectionStrength);

#elif defined _USE_SPECULAR_ && defined _USE_RIM_LIGHTING_ && !defined _USE_REFLECTIONS_
	vec3 result = (ambient + diffuse + finalRim) * objectColor + specular;

#elif defined _USE_SPECULAR_ && !defined _USE_RIM_LIGHTING_ && defined _USE_REFLECTIONS_
	vec3 result = (ambient + diffuse) * objectColor + specular * (texture(skybox, refl).rgb * reflectionStrength + 1);

#elif !defined _USE_SPECULAR_ && defined _USE_RIM_LIGHTING_ && !defined _USE_REFLECTIONS_
	vec3 result = (ambient + diffuse + finalRim) * objectColor;

#elif !defined _USE_SPECULAR_ && !defined _USE_RIM_LIGHTING_ && !defined _USE_REFLECTIONS
	vec3 result = (ambient + diffuse) * objectColor;

#endif

	fragColor = vec4(result, 1.0);

}