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
#include <math.h>
#include "ParamaterFileReader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class BoidScene {
public:
	BoidScene();
	BoidScene(string filename);
	virtual ~BoidScene();

	mat4 getModelMatrix(int i);
	vec3 getBoidCenter(int i);
	vector<vec3> getBoidGeometry(int i);
	int getNumberOfBoids();

	void updateScene();
	string extractTagValue(string fullTag, string tag);

	void refresh();
private:


	float generateRandomFloat(float min, float max);
	void initFromConfigFile();
	string extractValueFromTag(string tag);

	float fieldOfView;
	float alpha_n;
	float alpha_a;
	float alpha_v;
	float targetDistance;
	float neighbourDistance;
	float avoidDistance;
	string parameterFileName;
	ParamaterFileReader* paramReader;
	vector<Boid*> boids;

	vec3 targetLocation;



};

#endif /* BOIDSCENE_H_ */
