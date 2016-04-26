#include "WaypointBehavior.h"


WaypointBehavior::WaypointBehavior(vector<glm::vec3> points, GLfloat speed)
	:waypoints(points), speed(speed), targetWaypointIndex(0)
{
	position = waypoints[0];

	targetWaypointIndex = getNexWaypointIndex();
	updateVeolcityForNewWaypoint();
}


void WaypointBehavior::updateVeolcityForNewWaypoint() {
	velocity = glm::normalize(waypoints[targetWaypointIndex] - position) * speed;
}

GLfloat WaypointBehavior::distanceToTargetWaypoint() {
	return glm::distance(waypoints[targetWaypointIndex], position);
}

int WaypointBehavior::getNexWaypointIndex() {
	return (targetWaypointIndex + 1) % waypoints.size();
}

void WaypointBehavior::update(float elapsedTimeSeconds)
{
	if (distanceToTargetWaypoint() < (speed * elapsedTimeSeconds)) {
		targetWaypointIndex = getNexWaypointIndex();
		updateVeolcityForNewWaypoint();
	}
	position = position + velocity * elapsedTimeSeconds;
	target->setLocalTransformation(glm::translate(position));
} // end update
