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
private:
	vector<vec3> geometry;

};

#endif /* BOID_H_ */
