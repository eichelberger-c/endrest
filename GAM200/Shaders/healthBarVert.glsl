#version 400 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertClr;
layout (location = 2) in vec2 vertTex;

out vec4 fragClr;
out vec2 TexCoord;
out float vs_Health;

uniform mat3 transform;
uniform mat4 OrthoView;
uniform vec4 rgba;
uniform vec2 uv;
uniform float hpPercent;

void main()
{
   fragClr = rgba;
   vs_Health = hpPercent;
   TexCoord = vec2(vertTex.x, vertTex.y * -1.0f) + uv;
   gl_Position = OrthoView * vec4((transform * vec3(vertPos.x, vertPos.y, 1.0f)).xy, 0, 1);
}
