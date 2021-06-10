#version 400 core
in vec4 fragClr;
in vec2 TexCoord;
in vec2 scale;
//in vec2 scale2;
out vec4 fsColor;

uniform sampler2D textId;
uniform sampler2D maskId;

void main()
{
    vec4 image = texture(textId, TexCoord);

    vec4 mask = texture(maskId, scale);
    //vec4 test = texture(maskId, scale2);

    fsColor = vec4(image.rgb * fragClr.rgb, clamp(fragClr.a * (1  - mask.a),0,1));
}
