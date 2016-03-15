#include "Lab.h"


Scene scene;

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted. Registered with
* GLUT by calling glutDisplayFunc.
*/
static void RenderSceneCB()
{
	// Clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.draw();

	// Flush all drawing commands and swap the front and back buffers
	glutSwapBuffers();

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
	default:
		cout << key << " key pressed." << endl;
	}

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end KeyboardCB


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

	default:
		cout << "Invalid view selection " << endl; 
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end viewMenu

void polygonMode(int value)
{
	// Set the polygon rendering mode based on the value of the value parameter
	switch (value) {

	case(1) :

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case(2) :

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;

	case(3) :

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;

	default:
		cout << "Invalid Polygon Selection " << endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();
}

void pointSize(int value)
{
	// Set the size of points based on value
	glPointSize((GLfloat)value);

	// Signal GLUT to call display callback
	glutPostRedisplay();
}

void lineSize(int value)
{
	// Set the size of lines based on value
	glLineWidth((GLfloat)value);

	// Signal GLUT to call display callback
	glutPostRedisplay();
}

void enableAA(int enabled) {
	if (enabled) {
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
	}
	else {
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_LINE_SMOOTH);
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();
}

void faceCull(int enabled) {
	if (enabled) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();
}

void faceCullMode(int value) {
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
		cout << "Invalid Polygon Selection " << endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();
}

void windingOrder(int value) {
	switch (value) {

	case(1) :

		glFrontFace(GL_CW);
		break;

	case(2) :

		glFrontFace(GL_CCW);
		break;

	default:
		cout << "Invalid Polygon Selection " << endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();
}

void depthTest(int enabled) {
	if (enabled) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

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
	GLuint world_Window = glutCreateWindow("CSE 387 Lab 2");

	// Register callback functions to handle various window events.
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutIdleFunc(IdleCB);

	// Create polygon submenu
	GLuint menu1id = glutCreateMenu(polygonMode);

	// Specify menu items and integer identifiers
	glutAddMenuEntry("Filled", 1);
	glutAddMenuEntry("Outline", 2);
	glutAddMenuEntry("Point", 3);

	// Create additional sub menus
	GLuint menu2id = glutCreateMenu(pointSize);
	glutAddMenuEntry("1", 1.0);
	glutAddMenuEntry("10", 10.0);
	glutAddMenuEntry("50", 50.0);
	glutAddMenuEntry("100", 100.0);

	GLuint menu3id = glutCreateMenu(lineSize);
	glutAddMenuEntry("1", 1.0);
	glutAddMenuEntry("10", 10.0);
	glutAddMenuEntry("50", 50.0);
	glutAddMenuEntry("100", 100.0);

	GLuint menu4id = glutCreateMenu(enableAA);
	glutAddMenuEntry("Enable", 1.0);
	glutAddMenuEntry("Disable", 0.0);

	GLuint menu5id = glutCreateMenu(faceCull);
	glutAddMenuEntry("Enable", 1.0);
	glutAddMenuEntry("Disable", 0.0);

	GLuint menu6id = glutCreateMenu(faceCullMode);
	glutAddMenuEntry("Front", 1.0);
	glutAddMenuEntry("Back", 2.0);
	glutAddMenuEntry("Front and back", 3.0);

	GLuint menu7id = glutCreateMenu(windingOrder);
	glutAddMenuEntry("CW", 1.0);
	glutAddMenuEntry("CCW", 2.0);

	GLuint menu8id = glutCreateMenu(depthTest);
	glutAddMenuEntry("Enable", 1.0);
	glutAddMenuEntry("Disable", 0.0);

	// Create main menu
	GLuint topMenu = glutCreateMenu(mainMenu);
	// Attach polygon Menu
	glutAddSubMenu("Polygon Mode", menu1id);

	// Attach additional sub menus
	glutAddSubMenu("Point Size", menu2id);
	glutAddSubMenu("Line Size", menu3id);
	glutAddSubMenu("AA", menu4id);
	glutAddSubMenu("Face Culling", menu5id);
	glutAddSubMenu("Face Culling mode", menu6id);
	glutAddSubMenu("Front face winding order", menu7id);
	glutAddSubMenu("Depth Test", menu8id);

	// Specify menu items and integer identifiers
	glutAddMenuEntry("Quit", 0);
	// Attach menu to right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

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
	scene.initialize();

	// Enter the GLUT main loop. Control will not return until the
	// window is closed unless glutSetOption has been called with the 
	// appropriate options
	glutMainLoop();

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.
	return 0;

} // end main


