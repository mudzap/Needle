#version 330 core

layout(location = 0) out vec4 gl_Color;
layout(location = 1) out vec4 BrightColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
    
void main() {

    //vec2 f_TexCoord = v_TexCoord * 3.0;
    //gl_Color = texture(u_Texture, f_TexCoord);
    gl_Color = texture(u_Texture, v_TexCoord * 3.0);


    float brightness = dot(gl_Color.rgb, vec3(0.2126, 0.7152, 0.0722));
    float shouldBloom = max(0.0, sign(brightness - 0.95));
    BrightColor = vec4(shouldBloom);

}   