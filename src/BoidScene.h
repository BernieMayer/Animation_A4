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


struct CircleObstacle
{
	vec3 center;
	float radius;
};



class BoidScene {
public:
	BoidScene();
	BoidScene(string filename);
	virtual ~BoidScene();

	void setTargetLocation(vec3 aLocation);
	vec3 getTargetLocation();

	mat4 getModelMatrix(int i);
	vec3 getBoidCenter(int i);
	vector<vec3> getBoidGeometry(int i);


	vec3 getObstacleLocation(int i);
	float getObstacleRadius(int i);


	int getNumberOfBoids();
	int getNumberOfObstacles();

	void updateScene();
	string extractTagValue(string fullTag, string tag);

	void refresh();
private:


	float timeSinceStart;

	float calculateX(float r);
	float linearWeighting(float x);
	float quadraticWeighting(float x);
	float inverseWeighting(float x);

	float generateRandomFloat(float min, float max);
	void initFromConfigFile();
	string extractValueFromTag(string tag);




	vec3 h_c = vec3(0, 0, 0);

	float maxVelocity = 20.0f;
	float maxAcceleration = 10.0f;
	float fieldOfView;
	float alpha_n;
	float alpha_a;
	float alpha_v;
	float alpha_p = 0.5;
	float targetDistance = 10.0f;
	float neighbourDistance;
	float boundRadius;


	float avoidDistance;
	string parameterFileName;
	ParamaterFileReader* paramReader;
	vector<Boid*> boids;

	vector<CircleObstacle> circleObstacles;


	vec3 targetLocation;



};

#endif /* BOIDSCENE_H_ */
