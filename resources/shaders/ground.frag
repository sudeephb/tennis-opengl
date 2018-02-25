#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture samplers
uniform sampler2D texture1;

void main()
{
// Linearly interpolate between both textures (second texture is only slightly combined)
vec4 textureColor = texture(texture1, TexCoord);
if(textureColor.a < 0.5f)
    discard;
    
    color = textureColor;
    //color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
