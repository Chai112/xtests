
#include <stdio.h>
#include <string.h>

#include <XPLMGraphics.h>
#include <XPLMDisplay.h>

#include <GL/gl.h>

// Our texture dimensions.  Textures MUST be powers of 2 in OpenGL - if you don't need that much space,
// just round up to the nearest power of 2.
#define WIDTH 128
#define HEIGHT 128

// This is our texture ID.  Texture IDs in OpenGL are just ints...but this is a global for the life of our plugin.
static int                g_tex_num = 0;

// We use this memory to prep the buffer.  Note that this memory DOES NOT have to be global - the memory is FULLY
// read by OpenGL before glTexSubImage2D or glTexImage2D return, so you could use local or temporary storage, or
// change the image AS SOON as the call returns!  4 bytes for R,G,B,A 32-bit pixels.
static unsigned char    buffer[WIDTH*HEIGHT*4];

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
   unsigned char * c = buffer;
   for(int y = 0; y < HEIGHT; ++y)
   for(int x = 0; x < WIDTH; ++x)
   {
      *c++ = x * 255 / WIDTH;
      *c++ = y * 255 / HEIGHT;
      *c++ = mx * 255 / sx;
      *c++ = my * 255 / sy;
   }
   XPLMBindTexture2d(g_tex_num,0);
   // Note: if the tex size is not changing, glTexSubImage2D is faster than glTexImage2D.
   glTexSubImage2D(GL_TEXTURE_2D,
                  0,                       // mipmap level
                  0,                       // x-offset
                  0,                       // y-offset
                  WIDTH,
                  HEIGHT,
                  GL_RGBA,                 // color of data we are seding
                  GL_UNSIGNED_BYTE,        // encoding of data we are sending
                  buffer);

   // The drawing part.
   XPLMSetGraphicsState(
      0,        // No fog, equivalent to glDisable(GL_FOG);
      1,        // One texture, equivalent to glEnable(GL_TEXTURE_2D);
      0,        // No lighting, equivalent to glDisable(GL_LIGHT0);
      0,        // No alpha testing, e.g glDisable(GL_ALPHA_TEST);
      1,        // Use alpha blending, e.g. glEnable(GL_BLEND);
      0,        // No depth read, e.g. glDisable(GL_DEPTH_TEST);
      0);        // No depth write, e.g. glDepthMask(GL_FALSE);

   glColor3f(1,1,1);        // Set color to white.
   glLineWidth(5);
   int buffer[] = {
       1, 2, 1
   };
   for (int i = 0; i < 100000; i++)
   {
       glBegin(GL_LINES);
       glVertex2f(buffer[0],buffer[0]);        // We draw one textured quad.  Note: the first numbers 0,1 are texture coordinates, which are ratios.
       glVertex2f(buffer[i % 2],buffer[i % 2]);        // lower left is 0,0, upper right is 1,1.  So if we wanted to use the lower half of the texture, we
       glEnd();
   }
   glLineWidth(1);
}

PLUGIN_API int XPluginStart(char * name, char * sig, char * desc)
{
   strcpy(name,"Texture example");
   strcpy(sig,"xpsdk.test.texture_example");
   strcpy(desc,"Shows how to use textures.");

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
