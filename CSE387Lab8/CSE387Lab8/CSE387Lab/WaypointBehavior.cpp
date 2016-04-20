#include "WaypointBehavior.h"


WaypointBehavior::WaypointBehavior(vector<glm::vec3> points, GLfloat speed)
	:waypoints(points), speed(speed), targetWaypointIndex(0)
{
}


WaypointBehavior::~WaypointBehavior()
{
}

void WaypointBehavior::updateVeolcityForNewWaypoint() {

}

GLfloat WaypointBehavior::distanceToTargetWaypoint() {
	return glm::distance(waypoints[targetWaypointIndex], position);
}

int WaypointBehavior::getNexWaypointIndex() {
	if (targetWaypointIndex + 1 < waypoints.size()) {
		targetWaypointIndex++;
		return targetWaypointIndex;
	}
	else {
		targetWaypointIndex = 0;
		return targetWaypointIndex;
	}
}

void WaypointBehavior::update(float elapsedTimeSeconds)
{
	if (distanceToTargetWaypoint() < (speed * elapsedTimeSeconds)) {
		position = waypoints[getNexWaypointIndex()];
		updateVeolcityForNewWaypoint();
	}
	position = position + velocity * elapsedTimeSeconds;
	target->setLocalTransformation(glm::translate(glm::mat4(1.0f), position));
} // end update
