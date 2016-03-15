#pragma once

/*
* Preprocessor statement for text substitution
*/
#define WINDOW_WIDTH 1024 // Default window width
#define WINDOW_HEIGHT 768 // Default window height

#include <iostream>
#include <vector>

using namespace std;
#include <GL/glew.h> // For extensions to OpenGL
#include <GL/freeglut.h> // Includes OpenGL.h

// GLM includes (See http://glm.g-truc.net/0.9.7/glm-0.9.7.pdf)
#define GLM_SWIZZLE  // Enable "swizzle" operators
#define GLM_FORCE_RADIANS // Force all GLM functions to adopt radians

#include <glm/glm.hpp> // Basic glm functionality

// Stable glm extensions
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

// Experimental glm extensions.
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

#define color glm::vec4

#define M_PI 3.1415926535897932384626433832795f

// Simple functions for printing vectors and matrices to the console
void print(const glm::vec2 & v0);

void print(const glm::vec3 & v0);

void print(const glm::vec4 & v0);

void print(const glm::mat2 & m);

void print(const glm::mat3 & m);

void print(const glm::mat4 & m);

// Give three vertices on the face of a polygon in counter clockwise 
// order calculates a normal vector for the polygon.
glm::vec3 findUnitNormal(glm::vec3 pZero, glm::vec3 pOne, glm::vec3 pTwo);

// Creates a color with random red, green, and blue components. Alpha is
// set to 1.0.
color getRandomColor();

/**
Function: Displays OpengGL and GLSL version information.
*/
void displayOpenGlInfo(void);

/**
* Checks the OpenGL state machine for errors. Prints out a message and clears the error state.
* It is called once per render update cycle by default, but could be called more often
* @param methodName name of the method in which the function is called.
*/
void checkOpenGLErrors(const GLchar* methodName);

