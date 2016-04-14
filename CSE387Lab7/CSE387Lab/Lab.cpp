#include "Lab.h"


MyScene scene;

const GLint FPS = 60; // Desired maximum number of frames per second
const GLint FRAME_INTERVAL = 1000 / FPS;

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted. Registered with
* GLUT by calling glutDisplayFunc.
*/
static void RenderSceneCB()
{
	// time in milliseconds of last frame render
	static GLint lastRenderTime = 0;
	int currentTime = glutGet(GLUT_ELAPSED_TIME); // Get current time
	int elapsedTime = currentTime - lastRenderTime; // Calc time since last frame
	
	// Check if enough time has elapsed since the last render.
	if (elapsedTime >= FRAME_INTERVAL) {
		// Save time for this frame render
		lastRenderTime = currentTime;
		checkOpenGLErrors("RenderSceneCB1");
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		checkOpenGLErrors("RenderSceneCB2");
		// Draw the scene objects
		scene.draw();

		checkOpenGLErrors("RenderSceneCB3");
		// Flush all drawing commands and swapbuffers
		glutSwapBuffers();

		scene.update((float)elapsedTime / 1000);
		// Query OpenGL for errors.
		checkOpenGLErrors("RenderSceneCB4");
	}


} // end RenderSceneCB


/**
* Function:
* Handle changes in window size. Called when the window is first opened and
* whenever the window is moved or resized. Registered with GLUT by calling
* glutReshapeFunc.
* @param width - new window width in pixels,
* @param height - new window hieght in pixels
*/
static void ResizeCB(int width, int height)
{
	// Set viewport to entire client area of the window
	glViewport(0, 0, width, height);
	
	// Set the projection matrix based don the window aspect ratio
	scene.resize(width, height);

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB


/**
* Function:
* Calls a method of the the OpenGLApplicationBase class. Registered with GLUT by calling glutKeyboardFunc.
* @param Key The generated ASCII character for the key whose press triggered the callback.
* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed.
* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
*/
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch (key) {

	case('f') : case('F') : // 'f' key to toggle full screen
		glutFullScreenToggle();
		break;
	case(27) : // Escape key
		glutLeaveMainLoop();
		break;
	case('0') :
		scene.toggleLights(0);
		break;
	case('1') :
		scene.toggleLights(1);
		break;
	case('2') :
		scene.toggleLights(2);
		break;
	case('m') :
		scene.detachMoon();
		break;
	case('l') :
		scene.reparentNewPlanet();
		break;
	default:
		cout << key << " key pressed." << endl;
	}

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end KeyboardCB


/**
* Function:
* Responds to special  key presses.
* @param key - GLUT_KEY_* constant for the special key pressed. 
*  See https://www.opengl.org/resources/libraries/glut/spec3/node54.html
* @param x The x - coordinate of the mouse relative to the window at the time the key is pressed.
* @param y The y - coordinate of the mouse relative to the window at the time the key is pressed.
*/
static void SpecialKeysCB(int key, int x, int y)
{
	switch (key) {

	case(GLUT_KEY_RIGHT) :


		break;
	case(GLUT_KEY_LEFT) :


		break;
	case(GLUT_KEY_UP) :


		break;
	case(GLUT_KEY_DOWN) :


		break;

	default:
		cout << key << " key pressed." << endl;
	}

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end SpecialKeysCB


/**
* Function:
* Registered as an idle function. Repeatedly called by GLUT when not
* responding to other events in order to drive the animation loop and
* get the scene rendered repeatedly. Registered with GLUT by calling glutIdleFunc.
*/
void IdleCB()
{
	glutPostRedisplay(); // Mark the current window as needing to be redisplayed.

} // end IdleCB


void mainMenu(int value)
{
	switch (value) {

	case(0) :

		// "Quit" selected on the menu
		glutLeaveMainLoop();
		break;
	case(1) :

		// Toggle full screen
		glutFullScreenToggle();
		break;

	default:
		cout << "Invalid view selection " << endl; 
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end viewMenu

void polygonMode(int value)
{
	switch (value) {
	case(1) :
		// Render polygon fronts and fill them
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case(2) :
		// Render polygon fronts in wire frame
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case(3) :
		// Render polygon fronts in wire frame
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	default:
		cout << "Unknown Polygon Mode!" << endl;
	}
	// Signal GLUT to call display callback
	glutPostRedisplay();
}


void pointSize(int value)
{
	glPointSize((GLfloat)value);
	cout << "Point Size is " << value << " pixels." << endl;
	glutPostRedisplay();
}

void lineWidthMenu(int value)
{
	glLineWidth((GLfloat)value);
	cout << "Line width is " << value << " pixels." << endl;
}

void antiAliasMenu(int value)
{
	switch (value) {
	case(1) :
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		break;
	case(2) :
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
		break;
	default:
		cout << "Unknown Main Menu Selection!" << endl;
	}
}


void frontFaceMenu(int value)
{
	switch (value) {
	case(1) :
		glFrontFace(GL_CCW);
		break;
	case(2) :
		glFrontFace(GL_CW);
		break;

	default:
		cout << "Unknown winding order!" << endl;
	}
	// Signal GLUT to call display callback
	glutPostRedisplay();
}


void cullFaceMenu(int value)
{
	switch (value) {
	case(1) :

		glCullFace(GL_FRONT);

		break;
	case(2) :

		glCullFace(GL_BACK);

		break;
	case(3) :
		glCullFace(GL_FRONT_AND_BACK);
		break;

	default:
		cout << "Unknown Face Culling Mode!" << endl;
	}
	// Signal GLUT to call display callback
	glutPostRedisplay();


}


void faceCulling(int value)
{
	switch (value) {
	case(1) :

		glEnable(GL_CULL_FACE);

		break;
	case(2) :

		glDisable(GL_CULL_FACE);

		break;

	default:
		cout << "Unknown Face Culling Mode!" << endl;
	}
}

void depthTesting(int value)
{
	switch (value) {
	case(1) :

		glEnable(GL_DEPTH_TEST);

		break;
	case(2) :

		glDisable(GL_DEPTH_TEST);

		break;

	default:
		cout << "Unknown DEPTH TEST Mode!" << endl;
	}
}

void shader(int value)
{
	scene.selectShader(value);
}


void mode(int value)
{
	scene.setTextureMode(value);
}
void createMenus()
{
	// Create polygon submenu
	GLuint menu1id = glutCreateMenu(polygonMode);
	// Specify menu items and integer identifiers
	glutAddMenuEntry("Filled", 1);
	glutAddMenuEntry("Outline", 2);
	glutAddMenuEntry("Point", 3);

	// Create point size submenu
	GLuint menu2id = glutCreateMenu(pointSize);
	// Specify menu items and  integer identifiers
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("50", 50);
	glutAddMenuEntry("100", 100);
	glutAddMenuEntry("500", 500);

	GLuint menu3id = glutCreateMenu(lineWidthMenu);
	glutAddMenuEntry("1", 1);
	glutAddMenuEntry("10", 10);
	glutAddMenuEntry("20", 20);
	glutAddMenuEntry("50", 50);

	GLuint menu4id = glutCreateMenu(frontFaceMenu);
	glutAddMenuEntry("CCW", 1);
	glutAddMenuEntry("CW", 2);

	GLuint menu5id = glutCreateMenu(cullFaceMenu);
	glutAddMenuEntry("Front", 1);
	glutAddMenuEntry("Back", 2);
	glutAddMenuEntry("Front and Back", 3);

	GLuint menu6id = glutCreateMenu(faceCulling);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);

	GLuint menu7id = glutCreateMenu(depthTesting);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);

	GLuint menu8id = glutCreateMenu(antiAliasMenu);
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);

	GLuint menu9id = glutCreateMenu(shader);
	glutAddMenuEntry("PerPixel", 0);
	glutAddMenuEntry("PerVertex", 1);

	GLuint menu10id = glutCreateMenu(mode);
	glutAddMenuEntry("None", 0);
	glutAddMenuEntry("Decal", 1);
	glutAddMenuEntry("Replace Ambient and Diffuse", 2);
	
	// Create main menu
	GLuint topMenu = glutCreateMenu(mainMenu);
	// Attach polygon Menu
	glutAddSubMenu("Polygon Mode", menu1id);
	// Attach point size Menu
	glutAddSubMenu("Point Size", menu2id);
	glutAddSubMenu("Line Width", menu3id);
	glutAddSubMenu("Point and Line Antialisin", menu8id);
	glutAddSubMenu("Front Face", menu4id);
	glutAddSubMenu("Face to be Culled", menu5id);
	glutAddSubMenu("Face Culling", menu6id);
	glutAddSubMenu("Depth Testing", menu7id);
	glutAddSubMenu("Shader", menu9id);
	glutAddSubMenu("Texture Mode", menu10id);

	// Specify menu items and integer identifiers
	glutAddMenuEntry("Toggle Full Screen", 1);
	glutAddMenuEntry("Quit", 0);
	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



int main(int argc, char** argv)
{
	// freeGlut and Window initialization ***********************

	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set initial display mode for double buffering and RGBA color.
	// Explicitly request double buffering, RGBA (Red Green Blue Alpha) color mode and
	// a depth buffer for hidden surface removal.
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGBA | GLUT_DEPTH);

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Specify window placement relative to the upper left hand corner of the screen.
	glutInitWindowPosition(100, 100);

	// Create a window using the string contained in pTitle and
	// make it the current window. glutCreateWindow does return a
	// unique integer identifier for the created window. Here the
	// identifier is not saved under the assumption that there will only
	// be one rendering window.
	GLuint world_Window = glutCreateWindow("CSE 387 Lab 7");

	// Request that the window be made full screen
	//glutFullScreenToggle();

	// GLEW does not entirely support the Core GLUT Profile out of the box.
	// The following statement fixes the problem.
	glewExperimental = GL_TRUE;
	// Intilize GLEW. This must be done after glut is initialized.
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false; // GLEW could not be initialized.
	}

	// OpenGL initialization ***********************
	displayOpenGlInfo();

	// Initialize the scene
	scene.initialize();

	// Create popup menus to control rendering pipeline
	createMenus();

	// Register callback functions to handle various window events.
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutIdleFunc(IdleCB);

	checkOpenGLErrors("main");

	// Enter the GLUT main loop. Control will not return until the
	// window is closed unless glutSetOption has been called with the 
	// appropriate options
	glutMainLoop();

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.
	return 0;

} // end main


