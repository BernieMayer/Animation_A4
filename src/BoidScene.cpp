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
	alpha_a = 0.4f;
	alpha_v = 0.2f;

	fieldOfView = (M_PI/4.0);

	avoidDistance = 2.0f;
	targetDistance = 5.0f;
	targetLocation =  glm::vec3(0.5, 0.5, 0.5);


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
		vec4 totalNeighbourVelocity = vec4(boid_i->getVelocity(), 1);
		vec4 totalNeighbour = vec4(boid_i->getCenter(), 1);
		vec3 h_a = vec3(0,0,0);
		int num_a = 1;
		//float avgVelocity = boid_i->getVelocity();
		vec3 h_t;
		int numNeighbours = 1;

		float distance_n  = length(boid_i->getCenter() - targetLocation);

		if (distance_n < targetDistance)
		{
			h_t = targetLocation - boid_i->getCenter();
		}

		for (int j = 0; j < boids.size() ; j++)
		{
			Boid* boid_j = boids.at(j);
			if (j != i)
			{
				vec3 vectorI_J = boid_j->getCenter() - boid_i->getCenter();
				//check boid_i with boid_j and see if they are close etc
				float distance_n = length(boid_j->getCenter() - boid_i->getCenter());

				float arc_angle = dot(normalize(vectorI_J), normalize(boid_i->getHeading()));
				if (distance_n < avoidDistance && acos(arc_angle) <  fieldOfView/2.0f)
				{


					vec3 i_to_j = normalize(boid_i->getCenter() - boid_j->getCenter());
					vec3 current_h_a = vec3(1, 1, 1) - i_to_j;

					h_a = h_a + current_h_a;
					totalNeighbourVelocity += vec4(boid_j->getVelocity(), 1);
					totalNeighbour += vec4(boid_j->getCenter(), 1);



				}else if (distance_n < neighbourDistance && acos(arc_angle) < fieldOfView/2.0f )
				{

					totalNeighbourVelocity += vec4(boid_j->getVelocity(), 1);
					totalNeighbour += vec4(boid_j->getCenter(), 1);

				}


			}
		}


		//h_a = (h_a)/((float) num_a);
		//printf("num neighbours is %i \n", numNeighbours );
		//avgVelocity = avgVelocity/((float) numNeighbours);
		//avgNeighbour = avgNeighbour * (1.0f/(float) numNeighbours);
		totalNeighbour /= totalNeighbour.w;
		vec3 avgNeighbour = vec3(totalNeighbour.x,totalNeighbour.y, totalNeighbour.z );

 		totalNeighbourVelocity /= totalNeighbourVelocity.w;
 		vec3 averageVelocity = vec3(totalNeighbourVelocity.x , totalNeighbourVelocity.y, totalNeighbourVelocity.z);

		vec3 heading = alpha_n * avgNeighbour
		+ alpha_a * normalize( -1.0f * h_a) + alpha_v * normalize(boid_i->getVelocity() - averageVelocity);

 		boid_i->setHeading(normalize(heading));


 		boid_i->setVelocity(averageVelocity);
	}

	for (auto boid:boids)
	{
		vec3 center = boid->getCenter();

		//boid->setCenter(center + boid->getHeading() *0.002f);

		float dt = 1.0/30.0f;
		boid->setCenter(center + dt * boid->getVelocity());
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

	string avoid_tag = "avoidDistance:";
	string queryAvoidDistance = paramReader->queryTag("avoidDistance:");

	string avoidDistanceStr = extractTagValue(queryAvoidDistance, avoid_tag );

	avoidDistance = stof(avoidDistanceStr);

	vec3 translationVector = vec3(0, 0, 0);
	vec3 translationVector2 = vec3(0, 1, 0);
	for (int i = 0; i < numBoids; i++)
	{
		if (i%2 == 0) {
		Boid* boid = new Boid(translationVector);
		translationVector.x += 1.0;

		boids.push_back(boid);
		} else {
			Boid* boid = new Boid(translationVector2);
			translationVector2.x += 1.0;

			boids.push_back(boid);
		}
	}

	string alpha_n_tag = "alpha_n:";
	string query_alpha_n = paramReader->queryTag(alpha_n_tag);

	string alpha_n_str = extractTagValue(query_alpha_n, alpha_n_tag);

	alpha_n = stof(alpha_n_str);

	string alpha_a_tag = "alpha_a:";
	string query_alpha_a = paramReader->queryTag(alpha_a_tag);

	string alpha_a_str = extractTagValue(query_alpha_a, alpha_a_tag);

	alpha_a = stof(alpha_a_str);

	string alpha_v_tag = "alpha_v:";
	string query_alpha_v = paramReader->queryTag(alpha_v_tag);

	string alpha_v_str = extractTagValue(query_alpha_v, alpha_v_tag);

	alpha_a = stof(alpha_v_str);
}
