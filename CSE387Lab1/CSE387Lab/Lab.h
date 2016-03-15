#pragma once

#include "BasicIncludesAndDefines.h"

#include "ReadShader.h"


/**
* Declarations for functions that act as "call backs" for various
* events. FreeGLUT commands are used to register these functions
* as event handlers.
*/

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted. Registered with 
* GLUT by calling glutDisplayFunc.
*/
static void RenderSceneCB();

/**
* Called when the window is first opened and
* whenever the window is moved or resized. Registered with GLUT by calling
* glutReshapeFunc. Resets the graphics viewport limits for full window rendering
* each time the window is resized.
* 
* @param width of the window in pixels
* @param height of the window in pixels
*/
static void ResizeCB(int width, int height);

/**
* Calls a method of the the OpenGLApplicationBase class. Registered with GLUT by calling glutKeyboardFunc.
* @param Key The generated ASCII character for the key whose press triggered the callback.
* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed.
* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
*/
static void KeyboardCB(unsigned char key, int x, int y);

/**
* Responds to special  key presses.
* @param key - GLUT_KEY_* constant for the special key pressed.
*  See https://www.opengl.org/resources/libraries/glut/spec3/node54.html
* @param x The x - coordinate of the mouse relative to the window at the time the key is pressed.
* @param y The y - coordinate of the mouse relative to the window at the time the key is pressed.
*/
static void SpecialKeysCB(int key, int x, int y);

/**
* Registered as an idle function. Repeatedly called by GLUT when not
* responding to other events in order to drive the animation loop and
* get the scene rendered repeatedly. Registered with GLUT by calling glutIdleFunc.
*/
void IdleCB();


void Menu(int value);
