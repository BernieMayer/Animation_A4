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

	fieldOfView = 2 * M_PI;

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

	/*
	Boid* keyBoid = boids.at(0);

	vec3 velocity = keyBoid->getVelocity();

	keyBoid->setVelocity(vec3(0, 0, 0));
	keyBoid->setCenter(keyBoid->getCenter() + vec3( neighbourDistance * cos(timeSinceStart), 0, 0 ));
	timeSinceStart += 1/30.0f;
	*/

	for (int i = 0; i < boids.size(); i++)
	{


		Boid* boid_i = boids.at(i);
		vec4 totalNeighbourVelocity = vec4(vec3(0, 0, 0), 0);
		vec4 totalNeighbour = vec4(boid_i->getCenter(), 1);
		vec4 totalAvoidance = vec4(0, 0, 0, 0);
		vec4 totalFollowing = vec4(vec3(0, 0, 0), 0);
		//vec3 h_a = vec3(0,0,0);
		int num_a = 1;
		//float avgVelocity = boid_i->getVelocity();
		vec3 h_t;
		int numNeighbours = 1;

		float distance_n  = length(boid_i->getCenter() - targetLocation);





		if (distance_n < targetDistance)
		{
			float x =  calculateX(length(targetLocation - boid_i->getCenter()));
			//totalFollowing += vec4(targetLocation - boid_i->getCenter(), 1);


			//h_t = targetLocation - boid_i->getCenter();
		}


		for (int j = 0; j < boids.size() ; j++)
		{
			Boid* boid_j = boids.at(j);
			if (j != i)
			{
				vec3 vectorI_J = boid_j->getCenter() - boid_i->getCenter();
				//check boid_i with boid_j and see if they are close etc
				float distance_n = length(boid_j->getCenter() - boid_i->getCenter());

				float cos_theta = dot(normalize(vectorI_J), normalize(boid_i->getHeading()));
				//printf("The heading is is %f , %f, %f, \n",
                //boid_i->getHeading().x, boid_i->getHeading().y, boid_i->getHeading().z);

				if (distance_n < avoidDistance && acos(cos_theta) >  fieldOfView/2.0f)
				{


					vec3 i_to_j = normalize(boid_i->getCenter() - boid_j->getCenter());
					//vec3 current_h_a = vec3(1, 1, 1) - i_to_j;

					//This is a vector circle that defines the radius of avoidance
					vec3 R = boid_i->getCenter() + avoidDistance * normalize(vectorI_J);


					float x = calculateX(length(i_to_j));


					vec4 current_h_a =   vec4(i_to_j, 1);

					totalAvoidance += linearWeighting(x) * current_h_a;

					//totalNeighbourVelocity += linearWeighting(x) * vec4(boid_j->getVelocity(), 1);

					//h_a = h_a + current_h_a;
					//totalNeighbourVelocity += vec4(boid_j->getVelocity(), 1);
					//totalNeighbour += vec4(boid_j->getCenter(), 1);



				}else if (distance_n < neighbourDistance && acos(cos_theta) >  fieldOfView/2.0f )
				{

					float x =  calculateX(length(vectorI_J));

					//totalFollowing += vec4(boid_j->getHeading(), 1);
					totalFollowing +=  vec4(boid_j->getHeading(), 1);
					totalNeighbourVelocity +=  vec4(boid_j->getVelocity(), 1);


					//Old way of doing neighbours
					//totalNeighbour += linearWeighting(x) * vec4(boid_j->getCenter(), 1);

				} else if (distance_n < neighbourDistance)
				{
					float x =  calculateX(length(vectorI_J));
					totalNeighbourVelocity +=  vec4(boid_j->getVelocity(),1);



				}


			}
		}


		//printf("Total Following is %f, %f, %f, %f \n", totalFollowing.x, totalFollowing.y, totalFollowing.z, totalFollowing.w);
		//h_a = (h_a)/((float) num_a);
		//printf("num neighbours is %i \n", numNeighbours );
		//avgVelocity = avgVelocity/((float) numNeighbours);
		//avgNeighbour = avgNeighbour * (1.0f/(float) numNeighbours);


		vec3 h_f;
		if (totalFollowing.w < 0.000001 && totalFollowing.w > -0.000001)
		{
			//printf("H_f is boids heading \n");
			h_f = boid_i->getHeading();
		} else {
			totalFollowing -= vec4(boid_i->getCenter(), 0);
			totalFollowing /= totalFollowing.w;

			h_f = vec3(totalFollowing.x, totalFollowing.y, totalFollowing.z);
		}

		//totalFollowing -= vec4(boid_i->getCenter(), 0);





		totalFollowing /= totalFollowing.w;



		totalNeighbour /= totalNeighbour.w;
		vec3 avgNeighbour = vec3(totalNeighbour.x,totalNeighbour.y, totalNeighbour.z );


 		vec3 averageVelocity;

 		if (totalNeighbourVelocity.w < 0.000001 && totalNeighbourVelocity.w > -0.000001) {
 			averageVelocity = boid_i->getVelocity();

 		} else {
 			totalNeighbourVelocity /= totalNeighbourVelocity.w;
 			averageVelocity = vec3(totalNeighbourVelocity.x , totalNeighbourVelocity.y, totalNeighbourVelocity.z);
 		}

 		vec3 h_a;
 		if (totalAvoidance.w < 0.000001 && totalAvoidance.w > -0.000001)
 		{
 			h_a = vec3(0, 0, 0);
 		} else {
 			totalAvoidance /= totalAvoidance.w;
 			h_a = vec3(totalAvoidance.x , totalAvoidance.y, totalAvoidance.z);
 		}

 		vec4 h_obstactles;

 		for (int o = 0; o < circleObstacles.size(); o++)
 				{
 					CircleObstacle circle = circleObstacles.at(o);


 					vec3 circleToBoid = boid_i->getCenter() - circle.center;

 					vec3 closestPoint = circle.center + normalize(circleToBoid) * circle.radius;

 					float distanceToCircle = length(boid_i->getCenter() - closestPoint);

 					if (distanceToCircle < avoidDistance)
 					{
 						float x = calculateX(distanceToCircle);

 						h_obstactles +=   linearWeighting(x) * vec4(closestPoint - boid_i->getCenter(), 1);
 					}

 				}


 		//h_obstactles /= h_obstactles.w;

 		vec3 h_ob = vec3(h_obstactles.x, h_obstactles.y, h_obstactles.z);

 		float distance_TargetLocation = length( targetLocation - boid_i->getCenter());
 		vec3 h_p = targetLocation - boid_i->getCenter();




		vec3 heading = alpha_n * (h_f)
		+ alpha_a  * -1.0f *h_a + alpha_v * (averageVelocity - boid_i->getVelocity())
	    + alpha_p * h_p + 0.1f * -1.0f * h_ob + boid_i->getH_C();

		if (length(heading) > maxAcceleration)
		{
			heading = (maxAcceleration) * normalize(heading);
		}

 		boid_i->setHeading(heading);


 		//boid_i->setVelocity(averageVelocity);
	}

	for (auto boid:boids)
	{
		vec3 center = boid->getCenter();

		//boid->setCenter(center + boid->getHeading() *0.002f);
		float dt = 1.0/30.0f;
		vec3 newVelocity = boid->getVelocity() + boid->getHeading() * dt;


		if (length(newVelocity) > maxVelocity)
		{
			newVelocity = (float)(maxVelocity) * normalize(newVelocity);
		}



		boid->setVelocity(newVelocity);
		boid->setCenter(center + dt * boid->getVelocity());

		if (length(boid->getCenter()) > boundRadius)
		{
			vec3 h_c = normalize(- boid->getCenter()) * (length(boid->getCenter() - boundRadius));
			boid->setH_C(h_c);
		} else
		{
			vec3 h_c = vec3(0, 0, 0);
			boid->setH_C(h_c);
		}
			//boid->setCenter(boundRadius * normalize(boid->getCenter()) );
			//boid->setVelocity(-1.0f * boid->getVelocity());
			//boid->setPosition(vec3(0,0,0));
			//boid->setVelocity(-1.0f * boid->getVelocity());



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

	srand( time( NULL ) );
	delete paramReader;
	boids.clear();
	circleObstacles.clear();

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


	float max,min;

	string maxSpawnDistanceStr = extractValueFromTag("maxSpawnDistance:");
	max = stof(maxSpawnDistanceStr);

	string minSpawnDistanceStr = extractValueFromTag("minSpawnDistance:");
	min = stof(minSpawnDistanceStr);

	vec3 translationVector = vec3(0, 0, 0);
	vec3 translationVector2 = vec3(0, 1, 0);
	for (int i = 0; i < numBoids; i++)
	{



		float x = generateRandomFloat(min, max);
		float y = generateRandomFloat(min, max);
		float z = generateRandomFloat(min, max);

		translationVector = vec3(x, y , z);


		Boid* boid = new Boid(translationVector);


		float h_x = generateRandomFloat(-1.0, 1.0);
		float h_y = generateRandomFloat(-1.0, 1.0);
		float h_z = generateRandomFloat(-1.0, 1.0);

		//boid->setHeading(vec3(h_x, h_y, h_z));

		//boid->setVelocity(vec3(h_x, h_y, h_z));
		//boid->setVelocity(0.1f * translationVector);
		boids.push_back(boid);

		/*
		if (i%2 == 0) {
		Boid* boid = new Boid(translationVector);
		translationVector.x += 1.0;

		boids.push_back(boid);
		} else {
			Boid* boid = new Boid(translationVector2);
			translationVector2.x += 1.0;

			boids.push_back(boid);
		}
		*/
	}

	CircleObstacle obstacle1 = CircleObstacle();
	obstacle1.center = vec3(0, 0, 0);
	obstacle1.radius = 1.0f;

	circleObstacles.push_back(obstacle1);

	CircleObstacle obstacle2 = CircleObstacle();
	obstacle2.center = vec3(0, 0.5, 0);
	obstacle2.radius = 8.0f;

	//circleObstacles.push_back(obstacle2);


	CircleObstacle obstacle3 = CircleObstacle();
	obstacle3.center = vec3(1, 0.5, 0);
	obstacle3.radius = 4.0f;

	//circleObstacles.push_back(obstacle3);

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

	string alpha_pString = extractValueFromTag("alpha_p:");
	alpha_p = stof(alpha_pString);

	string fieldOfViewMultString = extractValueFromTag("fieldOfViewMultiplier:");
	float multiplier = stof(fieldOfViewMultString);

	fieldOfView = multiplier * M_PI;


	string boundRadiusString = extractValueFromTag("boundRadius:");
	boundRadius = stof(boundRadiusString);

	string maxVelocityString = extractValueFromTag("maxVelocity:");
	maxVelocity = stof(maxVelocityString);

	string maxAccelerationString = extractValueFromTag("maxAcceleration:");
	maxAcceleration = stof(maxAccelerationString);




}

mat4 BoidScene::getModelMatrix(int i)
{

	vec3 velocity =normalize( boids.at(i)->getVelocity());
	vec3 right = cross(velocity, vec3(0, 1, 0));
	vec3 up = cross(right, velocity);

	mat4 modelMatrix = mat4(vec4(velocity, 0), vec4(normalize(up), 0), vec4(normalize(right), 0),
						vec4(boids.at(i)->getCenter(), 1));

	return modelMatrix;





}

float BoidScene::generateRandomFloat(float min, float max)
{
	   float random = ((float) rand()) / (float) RAND_MAX;
	   float diff = max - min;
	   float r = random * diff;
	   return min + r;
}

float BoidScene::linearWeighting(float x)
{
	return 1.0 - x;
}

float BoidScene::quadraticWeighting(float x)
{
	return 1.0 - x * x;
}

float BoidScene::inverseWeighting(float x)
{
	if (x < 0.0000001 || x > -0.0000001)
	{
		return 100000.0f;
	} else {
		return 1.0f/x;
	}
}

float BoidScene::calculateX(float r)
{
	return 1.0 - (r - avoidDistance)/(neighbourDistance + avoidDistance);
}

void BoidScene::setTargetLocation(vec3 aLocation)
{
	targetLocation = aLocation;
}

vec3 BoidScene::getTargetLocation() {
	return targetLocation;
}

vec3 BoidScene::getObstacleLocation(int i)
{
	return circleObstacles.at(i).center;
}

float BoidScene::getObstacleRadius(int i)
{
	return circleObstacles.at(i).radius;
}

int BoidScene::getNumberOfObstacles()
{
	return circleObstacles.size();
}

string BoidScene::extractValueFromTag(string tag)
{

	string query = paramReader->queryTag(tag);
	return extractTagValue(query, tag);
}
