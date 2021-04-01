layout(location = 0) out vec4 fragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
    
void main() {

    fragColor = texture(u_Texture, v_TexCoord);

}   