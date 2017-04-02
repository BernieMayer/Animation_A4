/*
 * BoidScene.h
 *
 *  Created on: Apr 2, 2017
 *      Author: bemayer
 */

#ifndef BOIDSCENE_H_
#define BOIDSCENE_H_

#include "Boid.h"
#include <string>
#include "ParamaterFileReader.h"
#include <glm/glm.hpp>

class BoidScene {
public:
	BoidScene();
	BoidScene(string filename);
	virtual ~BoidScene();

	vector<vec3> getBoidGeometry(int i);
	int getNumberOfBoids();
private:
	ParamaterFileReader* paramReader;
	vector<Boid*> boids;



};

#endif /* BOIDSCENE_H_ */
