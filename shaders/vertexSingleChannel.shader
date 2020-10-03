layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;

out vec2 v_TexCoord;
out vec4 v_Color;

uniform mat4 u_Projection;

void main() {
	v_TexCoord = texCoord;
	v_Color = color;
	gl_Position = u_Projection * vec4(position, 0.0, 1.0);
}