#include "Lab.h"


/********************** GLOBALS ******************************/

GLuint shaderProgram; // Variable to hold the integer identifier for the shader program

GLuint vertexArrayObject; // Variable to hold the integer identifier for the vertex array object

/**
* Make initial OpenGL settings. Build a shader program and generate a
* vertex array object.
*/
void initialize()
{
	// Initialize OpenGL 
	glEnable(GL_DEPTH_TEST); // Turn on depth testing
	glClearColor(0.1f, 0.1f, 0.5f, 0.0f); // Set the window clear color

	// Build shader proram
	shaderProgram = glCreateProgram();

	// Make the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertexShaderSource = ReadShader("vertexShader.vs.glsl");

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	glCompileShader(vertexShader);

	GLint compiled;

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);

	//If compilation failed
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(vertexShader, len, &len, log);
		cout << "shader compilation failed: " << log << endl;
		delete[] log;
	}
	else {
		cout << "shader compilation successful" << endl;
	}

	glAttachShader(shaderProgram, vertexShader);

	// Make the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSource = ReadShader("fragmentShader.fs.glsl");

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

	//If compilation failed
	if (!compiled) {
		GLsizei len;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(vertexShader, len, &len, log);
		cout << "fragment shader compilation failed: " << log << endl;
		delete[] log;
	}
	else {
		cout << "fragment shader compilation successful" << endl;
	}

	glAttachShader(shaderProgram, fragmentShader);

	// Link the shader porgram and check for errors

	glLinkProgram(shaderProgram);

	GLint linked;

	glGetProgramiv(vertexShader, GL_COMPILE_STATUS, &linked);

	//If linking failed
	if (!linked) {
		GLsizei len;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(vertexShader, len, &len, log);
		cout << "shader linking failed: " << log << endl;
		delete[] log;
	}
	else {
		cout << "shader linking successful" << endl;
	}

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);

	glBindVertexArray(vertexArrayObject);

	// Delete the shader
	glDeleteShader(vertexShader);

}




/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted. Registered with
* GLUT by calling glutDisplayFunc.
*/
static void RenderSceneCB()
{
	// Clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Alternative method for clearing only the color buffer
	//static const GLfloat blue[] = { 0.1f, 0.1f, 0.5f, 0.0f };
	//glClearBufferfv(GL_COLOR, 0, blue);


	// Bind vertex array object
	glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Fetch input data for pipeling
	glDrawArrays(GL_TRIANGLES, 0, 3);


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


void Menu(int value)
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
	GLuint world_Window = glutCreateWindow("Lab 1 Building Shaders and Vertex ArrayObjects");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Register callback functions to handle various window events.
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	glutIdleFunc(IdleCB);

	// Create a Menu
	int menu1id = glutCreateMenu(Menu);

	// Specify menu items and integer identifiers
	glutAddMenuEntry("Toggle Full Screen", 1);
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
	initialize();

	// Enter the GLUT main loop. Control will not return until the
	// window is closed unless glutSetOption has been called with the 
	// appropriate options
	glutMainLoop();

	// Delete shader programs and vertex array object
	// TODO 

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.
	return 0;

} // end main


