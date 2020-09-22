#version 330 core

layout(location = 0) out vec4 gl_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
    
void main() {

    //vec3 sample = 1.0 - texture(u_Texture, v_TexCoord).rgb;

    //gl_Color = vec4(sample, 1.0);
    gl_Color = texture(u_Texture, v_TexCoord);
}   