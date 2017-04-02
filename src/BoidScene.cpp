/*
 * BoidScene.cpp
 *
 *  Created on: Apr 2, 2017
 *      Author: bemayer
 */

#include "BoidScene.h"

BoidScene::BoidScene() {
	// TODO Auto-generated constructor stub

}

BoidScene::BoidScene(string filename)
{
	paramReader = new ParamaterFileReader(filename);
	paramReader->echoFileContents();


	string queryNumBoids = paramReader->queryTag("numberOfBoids");
	string numberOfBoidsStr = queryNumBoids.erase(queryNumBoids.find("numberOfBoids"), queryNumBoids.length() - 1);
	cout << "The number of boids is " << numberOfBoidsStr << "\n";
	int numBoids = stoi(numberOfBoidsStr);

	for (int i = 0; i < numBoids; i++)
	{
		Boid* boid = new Boid();

		boids.push_back(boid);
	}
}

BoidScene::~BoidScene() {
	// TODO Auto-generated destructor stub
}

vector<vec3> BoidScene::getBoidGeometry(int i)
{
	return boids.at(i)->getGeometry();
}

int BoidScene::getNumberOfBoids()
{
	return boids.size();
}
