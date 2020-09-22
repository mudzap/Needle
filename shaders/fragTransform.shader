#version 330 core

layout(location = 0) out vec4 gl_Color;
layout(location = 1) out vec4 BrightColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {

    gl_Color = texture(u_Texture, v_TexCoord);

	float brightness = dot(gl_Color.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 0.9)
		BrightColor = gl_Color;
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 0.0);

}