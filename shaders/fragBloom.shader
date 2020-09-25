#version 330 core

layout(location = 0) out vec4 gl_Color;


in vec2 v_TexCoord;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{
    //const float gamma = 2.2;

    vec3 hdrColor = texture(scene, v_TexCoord).rgb;
    vec3 bloomColor = texture(bloomBlur, v_TexCoord).rgb;

    hdrColor += bloomColor * exposure; // additive blending

    // tone mapping
    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / gamma));

    //gl_Color = vec4(result, 1.0);
    gl_Color = vec4(hdrColor, 1.0);
}