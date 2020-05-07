#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMDefs.h"
#include "XPLMUtilities.h"
#include "XPLMDataAccess.h"
#include <string.h>

/* This example plugin demonstrates how to use a 2-d drawing callback to draw
 * to the screen in a way that matches the 3-d coordinate system.  Add-ons that
 * need to add 3-d labels, coach marks, or other non-3d graphics that "match"
 * the real world can use this technique to draw on with Metal and Vulkan. */

// Datarefs for the aircraft position.
static XPLMDataRef	s_pos_x = NULL; 
static XPLMDataRef	s_pos_y = NULL; 
static XPLMDataRef	s_pos_z = NULL; 

// Transform matrices - we will use these to figure out where we shuold should have drawn.
static XPLMDataRef	s_matrix_wrl = NULL;
static XPLMDataRef	s_matrix_proj = NULL;
static XPLMDataRef	s_screen_width = NULL;
static XPLMDataRef	s_screen_height = NULL;

// 4x4 matrix transform of an XYZW coordinate - this matches OpenGL matrix conventions.
static void mult_matrix_vec(float dst[4], const float m[16], const float v[4])
{
	dst[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + v[3] * m[12];
	dst[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + v[3] * m[13];
	dst[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + v[3] * m[14];
	dst[3] = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + v[3] * m[15];
}

// This drawing callback will draw a label to the screen where the 

static int DrawCallback1(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon)
{
	// Read the ACF's OpengL coordinates
	float acf_wrl[4] = {	
		XPLMGetDataf(s_pos_x),
		XPLMGetDataf(s_pos_y),
		XPLMGetDataf(s_pos_z),
		1.0f };
		
	float mv[16], proj[16];
	
	// Read the model view and projection matrices from this frame
	XPLMGetDatavf(s_matrix_wrl,mv,0,16);
	XPLMGetDatavf(s_matrix_proj,proj,0,16);
	
	float acf_eye[4], acf_ndc[4];
	
	// Simulate the OpenGL transformation to get screen coordinates.
	mult_matrix_vec(acf_eye, mv, acf_wrl);
	mult_matrix_vec(acf_ndc, proj, acf_eye);
	
	acf_ndc[3] = 1.0f / acf_ndc[3];
	acf_ndc[0] *= acf_ndc[3];
	acf_ndc[1] *= acf_ndc[3];
	acf_ndc[2] *= acf_ndc[3];
	
	float screen_w = XPLMGetDatai(s_screen_width);
	float screen_h = XPLMGetDatai(s_screen_height);
	
	float final_x = screen_w * (acf_ndc[0] * 0.5f + 0.5f);
	float final_y = screen_h * (acf_ndc[1] * 0.5f + 0.5f);

	// Now we have something in screen coordinates, which we can then draw a label on.

	XPLMDrawTranslucentDarkBox(final_x - 5, final_y + 10, final_x + 100, final_y - 10);

	float colWHT[] = { 1.0, 1.0, 1.0 };
	XPLMDrawString(colWHT, final_x, final_y, "TEST STRING 1", NULL, xplmFont_Basic);		
	
	return 1;
}

PLUGIN_API int XPluginStart(char * outName, char * outSig, char * outDesc)
{
	strcpy(outName,"Example label drawing");
	strcpy(outSig,"com.laminar.example_label_drawing");
	strcpy(outDesc,"A plugin that shows how to draw a 3-d-referenced label in 2-d");
	
	XPLMRegisterDrawCallback(DrawCallback1, xplm_Phase_Window, 0, NULL);
	
	s_pos_x = XPLMFindDataRef("sim/flightmodel/position/local_x");
	s_pos_y = XPLMFindDataRef("sim/flightmodel/position/local_y");
	s_pos_z = XPLMFindDataRef("sim/flightmodel/position/local_z");

	// These datarefs are valid to read from a 2-d drawing callback and describe the state
	// of the underlying 3-d drawing environment the 2-d drawing is layered on top of.
	s_matrix_wrl = XPLMFindDataRef("sim/graphics/view/world_matrix");
	s_matrix_proj = XPLMFindDataRef("sim/graphics/view/projection_matrix_3d");

	// This describes the size of the current monitor at the time we draw.
	s_screen_width = XPLMFindDataRef("sim/graphics/view/window_width");
	s_screen_height = XPLMFindDataRef("sim/graphics/view/window_height");
	
	return 1;
}

PLUGIN_API int XPluginEnable()
{
	return 1;
}

PLUGIN_API void XPluginStop()
{
}

PLUGIN_API void XPluginDisable()
{
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMessage, void * inParam )
{
}
