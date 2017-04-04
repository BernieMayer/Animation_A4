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

	initFromConfigFile();

	alpha_n = 0.4f;
	alpha_t = 0.4f;

	avoidDistance = 2.0f;
	targetDistance = 5.0f;
	targetLocation =  glm::vec3(1.0, 1.0, 0.0);


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

void BoidScene::updateScene()
{
	//For each boid check for its neighbours
	//Then match location of its neighbours
	//Match velocity


	for (int i = 0; i < boids.size(); i++)
	{
		Boid* boid_i = boids.at(i);
		vec3 avgNeighbour = vec3(0, 0, 0);
		vec3 h_a = vec3(0,0,0);
		int num_a = 0;
		float avgVelocity = 0.0f;
		vec3 h_t;
		int numNeighbours = 0;

		float distance_n  = length(boid_i->getCenter() - targetLocation);

		if (distance_n < targetDistance)
		{
			//h_t = targetLocation - boid_i->getCenter();
		}

		for (int j = 0; j < boids.size() ; j++)
		{
			Boid* boid_j = boids.at(j);
			if (j != i)
			{
				//check boid_i with boid_j and see if they are close etc
				float distance_n = length(boid_j->getCenter() - boid_i->getCenter());

				if (distance_n < avoidDistance)
				{
					/*
					avgNeighbour = avgNeighbour - boid_j->getCenter();
					numNeighbours++;
					avgVelocity += boid_j->getVelocity();
					*/

					h_a = h_a + boid_j->getCenter();
					num_a++;
					numNeighbours++;
					avgVelocity += boid_j->getVelocity();

				}else if (distance_n < neighbourDistance)
				{
					avgNeighbour = avgNeighbour + boid_j->getCenter();
					numNeighbours++;
					avgVelocity += boid_j->getVelocity();
				}

			}
		}

		h_a = (h_a)/((float) num_a);
		avgVelocity = avgVelocity/((float) numNeighbours);
		avgNeighbour = avgNeighbour * (1.0f/(float) numNeighbours);

		vec3 heading = alpha_n * avgNeighbour + alpha_t * ( -1.0f * h_a) + 0.5f * boid_i->getHeading();
 		boid_i->setHeading(normalize(heading));
 		boid_i->setVelocity(avgVelocity);
	}

	for (auto boid:boids)
	{
		vec3 center = boid->getCenter();

		//boid->setCenter(center + boid->getHeading() *0.002f);
		boid->setCenter(center + boid->getHeading() * boid->getVelocity());
	}
}


//fullTag is the name of the full tag eg <tag>:<tagValue>
//
string BoidScene::extractTagValue(string fullTag, string tag) {

	return fullTag.replace(fullTag.find(tag), tag.length(), "");
}

vec3 BoidScene::getBoidCenter(int i)
{
	return boids.at(i)->getCenter();
}

//Custom code used to refresh the boid scene
void BoidScene::refresh()
{
	delete paramReader;
	boids.clear();

	initFromConfigFile();
}

void BoidScene::initFromConfigFile()
{

	paramReader = new ParamaterFileReader(parameterFileName);
	paramReader->echoFileContents();	//for testing purposes

	string tag = "numberOfBoids:";
	string queryNumBoids = paramReader->queryTag("numberOfBoids:");

	string numberOfBoidsStr = extractTagValue(queryNumBoids, tag);

	cout << "The number of boids is " << numberOfBoidsStr << "\n";
	int numBoids = stoi(numberOfBoidsStr);

	string neigbour_tag = "neigbourDistance:";
	string queryNeighDistance = paramReader->queryTag("neigbourDistance:");

	string neigbourDistanceStr = extractTagValue(queryNeighDistance, neigbour_tag );

	neighbourDistance = stof(neigbourDistanceStr);

	vec3 translationVector = vec3(-1, 0, 0);
	for (int i = 0; i < numBoids; i++)
	{
		Boid* boid = new Boid(translationVector);
		translationVector.x += 1.0;

		boids.push_back(boid);
	}
}
