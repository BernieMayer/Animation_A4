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
	parameterFileName = filename;
	paramReader = new ParamaterFileReader(filename);
	paramReader->echoFileContents();


	string tag = "numberOfBoids:";
	string queryNumBoids = paramReader->queryTag("numberOfBoids:");
	string numberOfBoidsStr = queryNumBoids.replace(queryNumBoids.find(tag), tag.length(), "");//queryNumBoids.erase(queryNumBoids.find("numberOfBoids:"), queryNumBoids.find(":") + 1);
	cout << "The number of boids is " << numberOfBoidsStr << "\n";
	int numBoids = stoi(numberOfBoidsStr);

	vec3 translationVector = vec3(-1, 0, 0);
	for (int i = 0; i < numBoids; i++)
	{
		Boid* boid = new Boid(translationVector);
		translationVector.x += 1.0;

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

//Custom code used to refresh the boid scene
void BoidScene::refresh()
{
	delete paramReader;
	boids.clear();

	paramReader = new ParamaterFileReader(parameterFileName);
	paramReader->echoFileContents();

	string tag = "numberOfBoids:";
		string queryNumBoids = paramReader->queryTag("numberOfBoids:");
		string numberOfBoidsStr = queryNumBoids.replace(queryNumBoids.find(tag), tag.length(), "");//queryNumBoids.erase(queryNumBoids.find("numberOfBoids:"), queryNumBoids.find(":") + 1);
		cout << "The number of boids is " << numberOfBoidsStr << "\n";
		int numBoids = stoi(numberOfBoidsStr);

		vec3 translationVector = vec3(-1, 0, 0);
		for (int i = 0; i < numBoids; i++)
		{
			Boid* boid = new Boid(translationVector);
			translationVector.x += 1.0;

			boids.push_back(boid);
		}
}
