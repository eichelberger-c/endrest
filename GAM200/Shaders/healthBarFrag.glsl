#version 400 core

in vec4 fragClr;
in vec2 TexCoord;
in float vs_Health;

out vec4 fsColor;

uniform sampler2D textId;
uniform sampler2D maskId;
void main()
{
    vec4 image = texture(textId, TexCoord);
    vec4 mask = texture(maskId, TexCoord);
    
    if(mask.r > vs_Health)
      discard;

    fsColor = vec4(image.rgb, (image.a * mask.a)) * fragClr;
}
