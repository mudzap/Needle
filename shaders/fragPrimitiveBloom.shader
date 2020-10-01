#version 330 core

#define _USE_BLOOM_

layout(location = 0) out vec4 gl_Color;
#ifdef _USE_BLOOM_
layout(location = 1) out vec4 BrightColor;
#endif

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
    
void main() {

    gl_Color = texture(u_Texture, v_TexCoord * 3.0);

#ifdef _USE_BLOOM_
    float brightness = dot(gl_Color.rgb, vec3(0.2126, 0.7152, 0.0722));
    float shouldBloom = max(0.0, sign(brightness - 0.95));
    BrightColor = vec4(shouldBloom);
#endif

}