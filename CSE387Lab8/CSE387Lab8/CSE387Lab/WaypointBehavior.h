#pragma once
#include "Behavior.h"
class WaypointBehavior : public Behavior
{
public:

	WaypointBehavior(vector< glm::vec3> points, GLfloat spd = 1.0f);

	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);

	glm::vec3 getPosition(){ return position; };

	glm::vec3 getVelocity(){ return velocity; };

protected:

	int getNexWaypointIndex();

	void updateVeolcityForNewWaypoint();

	GLfloat distanceToTargetWaypoint();

	vector<glm::vec3> waypoints;

	glm::vec3 position;
	glm::vec3 velocity;

	GLfloat speed;

	int targetWaypointIndex;
};

