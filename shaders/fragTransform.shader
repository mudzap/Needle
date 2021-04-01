layout(location = 0) out vec4 fragColor;
//layout(location = 1) out vec4 BrightColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {

    fragColor = texture(u_Texture, v_TexCoord);

	/*
	float brightness = dot(gl_Color.rgb, vec3(0.2126, 0.7152, 0.0722));
	float shouldBloom = max(0.0, sign(brightness - 0.9));
	BrightColor += vec4(shouldBloom);
	*/

}