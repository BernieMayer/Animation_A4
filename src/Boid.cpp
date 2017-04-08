/*
 * Boid.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: bemayer
 */

#include "Boid.h"

Boid::Boid() {


	vec3 p0 = vec3(0,0,0);
	vec3 p1	= vec3(0.5f, 0.5f, 0);
	vec3 p2 = vec3(1,0,0);

	geometry.push_back(p0);
	geometry.push_back(p1);
	geometry.push_back(p2);
}

Boid::Boid(vec3 translation)
{
	float scaleFactor = 0.2f;
	vec3 p0 = scaleFactor * (vec3(0,0,0) + translation);
	vec3 p1	= scaleFactor * (vec3(0.5f, 0.5f, 0) + translation);
	vec3 p2 = scaleFactor * (vec3(1,0,0) + translation);

	velocity = vec3(0.1, 0.0, 0.0);
	heading = vec3(1, 0,0.0);


	center = (p0 + p1 + p2)/(3.0f);

	geometry.push_back(p0);
	geometry.push_back(p1);
	geometry.push_back(p2);
}

Boid::~Boid() {
	// TODO Auto-generated destructor stub
}

vector<vec3> Boid::getGeometry()
{
	return geometry;
}

vec3 Boid::getCenter()
{
	return center;
}

vec3 Boid::getVelocity()
{
	return velocity;
}

void Boid::setVelocity(vec3 _velocity)
{
	velocity = _velocity;
}

void Boid::setHeading(vec3 _heading)
{
	heading = _heading;
}

vec3 Boid::getHeading()
{
	return heading;
}
