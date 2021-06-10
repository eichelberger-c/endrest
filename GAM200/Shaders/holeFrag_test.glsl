#version 400 core
in vec4 fragClr;
in vec2 TexCoord;
in int numMasks_Frag;
in vec2 scales[numMasks_Frag];
out vec4 fsColor;

uniform sampler2D textId;
uniform sampler2D maskId;

void main()
{
    vec4 image = texture(textId, TexCoord);

    vec4 masks[numMasks_Frag];

    float alpha = 1.0f;
    for(int i = 0; i < numMasks_Frag; ++i)
    {
      masks[i] = texture(maskId, scales[i]);
      alpha *= (1 - mask[i].a);
    }

    fsColor = vec4(image.rgb * fragClr.rgb, clamp(fragClr.a * alpha,0,1));
}
