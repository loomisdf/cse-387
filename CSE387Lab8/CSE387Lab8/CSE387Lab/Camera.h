#pragma once

#include "VisibleObject.h"

class Camera : public VisibleObject
{
public:

	/**
	* Default Constuctor. Positions the Camera as the origin of
	* World coordinate system looking in the -Z direction with the
	* positive Y direction up.
	*/
	Camera();

	/**
	* Positions the Camera relative to the Local coordinate frame.
	* @param position the Local coordinate position of the Camera.
	* @param viewingDirection viewing direction relative to the Local coordinate frame
	* @param upDirection up direction relative to the Local coordinate frame
	*/
	Camera(glm::vec3 position, glm::vec3 viewingDirection, glm::vec3 upDirection);

	~Camera();

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
	virtual void setViewPort(GLfloat xLowerLeft, GLfloat yLowerLeft, GLfloat viewPortWidth, GLfloat viewPortHeight);

	/**
	* Sets the viewport, projection matrix, and viewing transformation for the Camera so
	* that the scene can be rendered in the specified view port without distorion from the
	* perspective of the Camera.
	*/
	void setViewingTransformation();

	/**
	* Sets the Local coordinate viewing position of the Camera.
	* @param position the Local coordinate position of the Camera.
	*/
	void setViewingPosition(glm::vec3 position);

	/**
	* Sets the Local coordinate viewing direction of the Camera.
	* @param viewingDirection viewing direction relative to the Local coordinate frame
	*/
	void setViewingDirection(glm::vec3 viewingDirection);

	/**
	* Sets the Local coordinate up direction of the Camera.
	* @param upDirection up direction relative to the Local coordinate frame
	*/
	void setUpDirection(glm::vec3 upDirection);

	/**
	* Enable and diables the Camera. When an Camera is active the scene will be rendered
	* from the view point of the Camera. Disables Cameras continue to be updated, but the
	* scene is not rendered from the view point of the Camera. Active cameras are added to the
	* activeCamera vector.
	* @param active true if the Camera is enabled.
	*/
	void enable(bool active);

	// Vector containing the Cameras that are enabled.
	static vector<Camera*> activeCameras;

	virtual void initialize();

protected:

	void setLocalTransformation();

	// Data members and protected methods.
	float xCorner, yCorner, width, height;

	glm::vec3 up, right, front, position;

	bool active;


}; // end Camera class

