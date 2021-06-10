#version 400 core

in vec4 fragClr;
in vec2 TexCoord;

out vec4 fsColor;

uniform sampler2D textId;

void main()
{
   fsColor = texture(textId, TexCoord) * fragClr;
}