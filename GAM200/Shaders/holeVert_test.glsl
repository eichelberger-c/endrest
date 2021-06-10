#version 400 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertClr;
layout (location = 2) in vec2 vertTex;

out vec4 fragClr;
out vec2 TexCoord;
out vec2 scales[numMasks];
out int numMasks_Frag;

uniform int numMasks;
uniform mat3 transform;
uniform mat4 OrthoView;
uniform vec4 rgba;
uniform vec2 uv;
uniform vec2 texCoords[numMasks];
uniform float sizes[numMasks];
void main()
{
   numMaks_Frag = numMasks;
   fragClr = rgba;
   TexCoord = vec2(vertTex.x, vertTex.y * -1.0f) + uv;

   // * inverse of scale
   for(int i = 0; i < numMasks; ++i)
   {
      scales[i] = vec2(vertTex.x * 1 / sizes[i], vertTex.y * 1 / sizes[i]* -1.0f) + texCoords[i];
   }
   gl_Position = OrthoView * vec4((transform * vec3(vertPos.x, vertPos.y, 1.0f)).xy, 0, 1);
}
