/*
 * Boid.h
 *
 *  Created on: Apr 2, 2017
 *      Author: bemayer
 */

#ifndef BOID_H_
#define BOID_H_

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class Boid {
public:
	Boid();
	Boid(vec3 translation);
	virtual ~Boid();


	vector<vec3> getGeometry();
	vec3 getCenter();

	float getVelocity();
	void setVelocity(float _velocity);

	void setHeading(vec3 _heading);
	vec3 getHeading();

	void setCenter(const vec3& center) {
		this->center = center;
	}

private:
	vec3 heading;	//direction the boid is heading

	float velocity;
	vec3 center;
	vector<vec3> geometry;

};

#endif /* BOID_H_ */
