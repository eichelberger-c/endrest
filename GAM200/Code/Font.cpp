///-----------------------------KLANG-FILE-HEADER-------------------------------
//  File: Font.cpp
//
//  Author: Megan Works
//
//  Copyright © 2019 DigiPen (USA) Corporation
//
//  Functions:
//      - Font
//      - 
//
///-----------------------------USE-OR-DIE-THANKS-------------------------------

///----------------------------------INCLUDES-----------------------------------
#include "stdafx.h"
#include "Font.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// forces include to generate implementation
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stb_image_write.h"

#define STBTT_STATIC

///----------------------------------GLOBALS-----------------------------------
// fonts contains ascii characters 32-127
#define ASCIIstart 32
#define ASCIIend 127

///---------------------------------FUNCTIONS-----------------------------------

void Font::Test()
{
  ///* load font file */
  //long size;
  //unsigned char* fontBuffer;

  //FILE* fontFile = fopen("Data/Fonts/GoudyBookletter1911.ttf", "rb");
  //fseek(fontFile, 0, SEEK_END);
  //size = ftell(fontFile); /* how long is the file ? */
  //fseek(fontFile, 0, SEEK_SET); /* reset */

  //fontBuffer = (unsigned char*)malloc(size);

  //fread(fontBuffer, size, 1, fontFile);
  //fclose(fontFile);

  ///* prepare font */
  //stbtt_fontinfo info;
  //if (!stbtt_InitFont(&info, fontBuffer, 0))
  //{
  //  printf("failed\n");
  //}

  //int b_w = 512; /* bitmap width */
  //int b_h = 128; /* bitmap height */
  //int l_h = 64; /* line height */

  ///* create a bitmap for the phrase */
  //unsigned char* bitmap = (unsigned char *)malloc(b_w * b_h);

  ///* calculate font scaling */
  //float scale = stbtt_ScaleForPixelHeight(&info, l_h);

  //const char* word = "testing ! :)";

  //int x = 0;

  //int ascent, descent, lineGap;
  //stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);

  //ascent *= scale;
  //descent *= scale;

  //int i;
  //for (i = 0; i < strlen(word); ++i)
  //{
  //  /* get bounding box for character (may be offset to account for chars that dip above or below the line */
  //  int c_x1, c_y1, c_x2, c_y2;
  //  stbtt_GetCodepointBitmapBox(&info, word[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

  //  /* compute y (different characters have different heights */
  //  int y = ascent + c_y1;

  //  /* render character (stride and offset is important here) */
  //  int byteOffset = x + (y  * b_w);
  //  stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, word[i]);

  //  /* how wide is this character */
  //  int ax;
  //  stbtt_GetCodepointHMetrics(&info, word[i], &ax, 0);
  //  x += ax * scale;

  //  /* add kerning */
  //  int kern;
  //  kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
  //  x += kern * scale;
  //}

  ///* save out a 1 channel image */
  //stbi_write_png("out.png", b_w, b_h, 1, bitmap, b_w);

  //free(fontBuffer);
  //free(bitmap);

  //return;
}

Font::~Font()
{
}

void Font::Initialize(std::string filename)
{
  //FILE *ttf = fopen(filename.c_str(), "rb");
  //GLuint fontTex;


}
