#include <stdio.h>
#include <string.h>

#include <XPLMGraphics.h>
#include <XPLMDisplay.h>

#include "graphics.hpp"
#include "letters.h"

#include <glew.h>
//#include <GL/gl.h>
#include <math.h>
// Our texture dimensions.  Textures MUST be powers of 2 in OpenGL - if you don't need that much space,
// just round up to the nearest power of 2.
#define WIDTH 2280
#define HEIGHT 2280


// This is our texture ID.  Texture IDs in OpenGL are just ints...but this is a global for the life of our plugin.
static int                g_tex_num = 0;

// We use this memory to prep the buffer.  Note that this memory DOES NOT have to be global - the memory is FULLY
// read by OpenGL before glTexSubImage2D or glTexImage2D return, so you could use local or temporary storage, or
// change the image AS SOON as the call returns!  4 bytes for R,G,B,A 32-bit pixels.
static unsigned char    buffer[WIDTH*HEIGHT*4];
   static unsigned char * c = buffer;
   static float circlex[20];
   static float circley[20];

static float charA[] = {
    0,      0,
    .5,     1,
    1,      0,
    .83,    .33,
    .17,    .33,
};

static int my_draw_tex(
                                   XPLMDrawingPhase     inPhase,
                                   int                  inIsBefore,
                                   void *               inRefcon)
{
   // A really dumb bitmap generator - just fill R and G with x and Y based color watch, and the B and alpha channels
   // based on mouse position.
   int mx, my, sx, sy;
   XPLMGetMouseLocation(&mx, &my);
   XPLMGetScreenSize(&sx,&sy);
   // The drawing part.
   XPLMSetGraphicsState(
      0,        // No fog, equivalent to glDisable(GL_FOG);
      0,        // One texture, equivalent to glEnable(GL_TEXTURE_2D);
      0,        // No lighting, equivalent to glDisable(GL_LIGHT0);
      0,        // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
      1,        // Use alpha blending, e.g. glEnable(GL_BLEND);
      0,        // No depth read, e.g. glDisable(GL_DEPTH_TEST);
      0);        // No depth write, e.g. glDepthMask(GL_FALSE);

   glColor3f(1,1,1);        // Set color to white.
   
   int num_segments = 20;
   glLineWidth(5);
   glEnable(GL_LINE_SMOOTH); // TODO: this is a hardware dependency which could change for each driver!!!
   for (int i = 0; i < 1; i++)
   {
   glBegin(GL_LINE_STRIP);
    for(int ii = 1; ii < LETTER_O_SZ - 1;)
    {
        glVertex2f(500 + (float)LETTER_O[ii-1]/20, 500 + (float)LETTER_O[ii+=2]/20 );//output vertex

    }
    glEnd();
   }
   glBegin(GL_TRIANGLE_FAN);
    for(int ii = 0; ii < 8; ii++)
        glVertex2f(400 + (float)circlex[ii]*20, 400 + (float)circley[ii]*20);//output vertex
   glEnd();
   glDisable(GL_LINE_SMOOTH);
   glLineWidth(0);
}

PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
   strcpy(name,"Texture example");
   strcpy(sig,"xpsdk.test.texture_example");
   strcpy(desc,"Shows how to use textures.");
   
   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
       printf("GLEW NOT WORKING!!!\n");
       return 2; // errored
   }
   GLuint VertexArrayID;
   glGenVertexArrays(1, &VertexArrayID);
   glBindVertexArray(VertexArrayID);

   // Initialization: allocate a textiure number.
   XPLMGenerateTextureNumbers(&g_tex_num, 1);
   XPLMBindTexture2d(g_tex_num,0);
   // Init to black for now.
   memset(buffer,0,WIDTH*HEIGHT*4);
   // The first time we must use glTexImage2D.
   glTexImage2D(
           GL_TEXTURE_2D,
           0,                   // mipmap level
           GL_RGBA,             // internal format for the GL to use.  (We could ask for a floating point tex or 16-bit tex if we were crazy!)
           WIDTH,
           HEIGHT,
           0,                   // border size
           GL_RGBA,             // format of color we are giving to GL
           GL_UNSIGNED_BYTE,    // encoding of our data
           buffer);

   // Note: we must set the filtering params to SOMETHING or OpenGL won't draw anything!
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

   int cx = 1000;
   int cy = 1000;
   int num_segments = 8;
   int i = 0;
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

        float x = cosf(theta);//calculate the x component
        float y = sinf(theta);//calculate the y component

        circlex[ii] = x;
        circley[ii] = y;

    }
   
   for(int y = 0; y < HEIGHT; ++y)
   for(int x = 0; x < WIDTH; ++x)
   {
      *c++ = 1; // r
      *c++ = 1; // g
      *c++ = 1; // b
      *c++ = 1;
   }


   XPLMRegisterDrawCallback(my_draw_tex, xplm_Phase_Gauges, 0, NULL);
   return 1;
}

PLUGIN_API void XPluginStop(void)
{
    //XPLMUnregisterDrawCallback(my_draw_tex,xplm_Phase_Gauges, 0, NULL);
    XPLMUnregisterDrawCallback(my_draw_tex,xplm_Phase_Gauges, 0, NULL);
    XPLMBindTexture2d(g_tex_num,0);
    GLuint t=g_tex_num;
    glDeleteTextures(1,&t);
}

PLUGIN_API int XPluginEnable(void)
{
    return 1;
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID from, long msg, void * p)
{
}

