#version 400 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertClr;
layout (location = 2) in vec2 vertTex;

out vec4 fragClr;
out vec2 TexCoord;

uniform mat3 transform;
uniform mat4 OrthoView;
uniform vec4 rgba;
uniform vec2 uv;

void main()
{
   fragClr = rgba;
   TexCoord = vec2(vertTex.x, vertTex.y * -1.f) + uv;
   gl_Position = OrthoView * vec4((transform * vec3(vertPos.x, vertPos.y, 1.0)).xy, 0, 1);
}