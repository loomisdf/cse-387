#include "Camera.h"

vector<Camera*> Camera::activeCameras;

void Camera::initialize() {

}

Camera::Camera()
	:xCorner(0.0f), yCorner(0.0f), width(1.0f), height(1.0f),
	up(0.0f, 1.0, 0.0f), right(1.0f, 0.0, 0.0f), front(0.0f, 0.0, -1.0f), position(0.0f, 0.0f, 20.0f),
	active(false)
{
	setLocalTransformation();
}


Camera::~Camera()
{
}

/**
* Sets the rendering area for the camera and adjusts the project matrix to avoid
* distortion of objects in the scene. Add input parameters are normalized. In
* normalized coordinate the width and height of the viewport are always 1.0.
* (Use glutGet to determine the width and height of the viewport in pixels.
* @param xLowerLeft normalized x coordinate of the lower left hand corner or the viewport
* @param yLowerLeft normalized y coordinate of the lower left hand corner or the viewport
* @param viewPortWidth normalized width of the view port in pixels
* @param viewPortHeight normalized height of the view port in pixels
*/
void Camera::setViewPort(GLfloat xLowerLeft, GLfloat yLowerLeft, GLfloat viewPortWidth, GLfloat viewPortHeight)
{
	this->xCorner = xLowerLeft;
	this->yCorner = yLowerLeft;
	this->width = viewPortWidth;
	this->height = viewPortHeight;

} // end setViewPort

/**
* Sets the viewport, projection matrix, and viewing transformation for the Camera so
* that the scene can be rendered in the specified view port without distorion from the
* perspective of the Camera.
*/
void Camera::setViewingTransformation()
{
	int pixelWidth = glutGet(GLUT_WINDOW_WIDTH);
	int pixelHeight = glutGet(GLUT_WINDOW_HEIGHT);

	glViewport(xCorner * pixelWidth, yCorner * pixelHeight, (width * pixelWidth), (height * pixelHeight));

	SharedProjectionAndViewing::setProjectionMatrix(
		glm::perspective(M_PI / 4.0f, (width * pixelWidth) / (height * pixelHeight), 1.0f, 100.0f));

	SharedProjectionAndViewing::setViewMatrix(glm::inverse(getWorldTransformation()));

} // end setViewingTransformation

/**
* Sets the Local coordinate viewing position of the Camera.
* @param position the Local coordinate position of the Camera.
*/
void Camera::setViewingPosition(glm::vec3 position)
{
	this->position = position;

	setLocalTransformation();

} // end setViewingPosition

/**
* Sets the Local coordinate viewing direction of the Camera.
* @param viewingDirection viewing direction relative to the Local coordinate frame
*/
void Camera::setViewingDirection(glm::vec3 viewingDirection)
{
	this->front = viewingDirection;

	setLocalTransformation();

} // end setViewingDirection

/**
* Sets the Local coordinate up direction of the Camera.
* @param upDirection up direction relative to the Local coordinate frame
*/
void Camera::setUpDirection(glm::vec3 upDirection)
{
	this->up = upDirection;

	setLocalTransformation();

} // end setUpDirection



void Camera::setLocalTransformation()
{
	this->right = glm::cross(front, up);

	this->up = glm::cross(this->right, this->front);

	glm::mat4 viewTrans = glm::lookAt(this->position, this->position + this->front, this->up);

	VisibleObject::setLocalTransformation(glm::inverse(viewTrans));

}

/**
* Enable and diables the Camera. When an Camera is active the scene will be rendered
* from the view point of the Camera. Disables Cameras continue to be updated, but the
* scene is not rendered from the view point of the Camera. Active cameras are added to the
* activeCamera vector.
* @param active true if the Camera is enabled.
*/
void Camera::enable(bool on)
{
	if (on == true && this->active == false) {

		this->active = on;
		Camera::activeCameras.push_back(this);
	}
	else {

		for (unsigned int camIdx = 0; camIdx < Camera::activeCameras.size(); camIdx) {

			if (Camera::activeCameras[camIdx] == this){

				Camera::activeCameras.erase(Camera::activeCameras.begin() + camIdx);
				break;
			}
		}
	}

} // end enable
