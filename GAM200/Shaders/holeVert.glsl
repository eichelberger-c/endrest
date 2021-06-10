#version 400 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertClr;
layout (location = 2) in vec2 vertTex;

out vec4 fragClr;
out vec2 TexCoord;
out vec2 scale;
//out vec2 scale2;

uniform mat3 transform;
uniform mat4 OrthoView;
uniform vec4 rgba;
uniform vec2 texCoord;
uniform float size;
void main()
{
   fragClr = rgba;
   TexCoord = vec2(vertTex.x, vertTex.y * -1.0f);

   // * inverse of scale
    scale = vec2(vertTex.x, vertTex.y * -1.0f) * size + texCoord;
    //scale2 = vec2(vertTex.x, vertTex.y * -1.0f) * size + texCoord + vec2(.1f,0.f);
   gl_Position = OrthoView * vec4((transform * vec3(vertPos.x, vertPos.y, 1.0f)).xy, 0, 1);
}
