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
	vec3 p0 = vec3(0,0,0);
	vec3 p1	= vec3(0.5f, 0.5f, 0);
	vec3 p2 = vec3(1,0,0);

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
