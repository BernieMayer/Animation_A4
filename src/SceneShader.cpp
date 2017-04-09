/*
 * SceneShader.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: acarocha
 */

#include "SceneShader.h"
static float PI = 3.14159265359;


SceneShader::SceneShader(): Shader()
{
	_programPlane = 0;
	_programMesh = 0;
	_planeVertexArray = -1;
	_planeVertexArray = -1;
	_mvUniform = -1;
	_projUniform = -1;
	_zTranslation = 1.0;
	_aspectRatio = 1.0;
	_xRot = 0.0;
	_yRot = 0.0;
	lightPosition = glm::vec3(0.5, 0.5, 0.5);

	boidScene = new BoidScene("file.txt");

	boidScene->setTargetLocation(lightPosition);


}


void SceneShader::readMesh( std::string filename )
{
	_mesh = trimesh::TriMesh::read(filename);

	_mesh->need_bbox();
	_mesh->need_faces();
	_mesh->need_normals();
	_mesh->need_bsphere();

	for(unsigned int i = 0; i < _mesh->faces.size(); i++)
	{
   	_triangleIndices.push_back(_mesh->faces[i][0]);
		_triangleIndices.push_back(_mesh->faces[i][1]);
		_triangleIndices.push_back(_mesh->faces[i][2]);
	}
}



void SceneShader::createVertexBuffer()
{
	//create plane geometry
	static const GLfloat quadData[] =
	{
                        -1.0f, 0.0f, -1.0f,
                        -1.0f, 0.0f, 1.0f,
                        1.0f, 0.0f, -1.0f,
                        1.0f, 0.0f, 1.0f,
	};

	//passing model attributes to the GPU
	//plane
	glGenVertexArrays(1, &_planeVertexArray);
	glBindVertexArray(_planeVertexArray);

	glGenBuffers(1, &_planeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _planeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (quadData), quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//read and create mesh geometry
	readMesh("./models/bunny.ply"); //normalized vertices coordinates

	//triangle mesh
	glGenVertexArrays(1, &_meshVertexArray);
	glBindVertexArray(_meshVertexArray);

	glGenBuffers(1, &_meshVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _meshVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  _mesh->vertices.size() * sizeof (trimesh::point), _mesh->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	
	//TODO normals

        glGenBuffers(1, &_meshNormalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _meshNormalBuffer);
        glBufferData(GL_ARRAY_BUFFER,  _mesh->normals.size() * sizeof (trimesh::vec), _mesh->normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);


	glGenBuffers(1, &_meshIndicesBuffer );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _meshIndicesBuffer );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangleIndices.size()*sizeof(unsigned int), _triangleIndices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	glGenVertexArrays(1, &_boidVertexArray);
	glBindVertexArray(_boidVertexArray);


	glGenBuffers(1, &_boidsVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _boidsVertexBuffer);

	vector<vec3> geometry = boidScene->getBoidGeometry(0);
	glBufferData(GL_ARRAY_BUFFER, geometry.size() * sizeof(glm::vec3), geometry.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &_boidColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _boidColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, geometry.size() * sizeof(glm::vec3), geometry.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

}


void SceneShader::startup()
{
	_programPlane = compile_shaders("./shaders/plane.vert", "./shaders/plane.frag");

	_programMesh = compile_shaders("./shaders/mesh.vert", "./shaders/mesh.frag");

	_programLight = compile_shaders("./shaders/light.vert", "./shaders/light.frag");

	_programBoids = compile_shaders("./shaders/simple.vert", "./shaders/simple.frag");

	createVertexBuffer();

}

void SceneShader::refreshScene()
{
	boidScene->refresh();
}

void SceneShader::renderBoids()
{
	//for each boid...
	boidScene->updateScene();
	int numBoids = boidScene->getNumberOfBoids();
	vector<vec3> geometry = boidScene->getBoidGeometry(0);

	glBindVertexArray(_boidVertexArray);
	glUseProgram(_programBoids);


	vector<vec3> colors;

		for (auto i :geometry)
		{

			colors.push_back(vec3(1.0, 0.0, 0.0));

		}




	for (int i  = 0; i < numBoids; i++)
	{




		//scene matrices and camera setup
		glm::vec3 eye(0.0f, 0.3f, 2.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::vec3 center(0.0f, 0.0f, 0.0f);

		_modelview = glm::lookAt( eye, center, up);

		_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

		glm::mat4 identity(1.0f);

		glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		_modelview *=  rotationX;



		//vector<vec3> geometry = boidScene->getBoidGeometry(i);
		_modelview *= glm::translate(identity, boidScene->getBoidCenter(i));

		_modelview = _modelview * boidScene->getModelMatrix(i);

		//uniform variables
		glUniformMatrix4fv(glGetUniformLocation(_programBoids, "modelviewMatrix"), 1, GL_FALSE, glm::value_ptr(_modelview));
		glUniformMatrix4fv(glGetUniformLocation(_programBoids, "perspectiveMatrix"), 1, GL_FALSE, glm::value_ptr(_projection));


		glBindBuffer(GL_ARRAY_BUFFER, _boidsVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,  geometry.size() * sizeof (glm::vec3), geometry.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, _boidColorBuffer);
		glBufferData(GL_ARRAY_BUFFER,  colors.size() * sizeof (glm::vec3), colors.data(), GL_STATIC_DRAW);

		glDrawArrays(GL_TRIANGLES, 0, geometry.size());

	}

	//glDrawArraysInstanced(GL_TRIANGLES, 0, geometry.size(), boidScene->getNumberOfBoids());
	//glDrawArraysInstanced(GL_TRIANGLES, 0, geometry.size(), boidScene->getNumberOfBoids());
	//glBindVertexArray(0);

}

void SceneShader::renderPlane()
{
	glBindVertexArray(_planeVertexArray);

	glUseProgram(_programPlane);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f,0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	glm::mat4 identity(1.0f);
	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

   _modelview *=  rotationX;
	
	//Uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programPlane, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programPlane, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programPlane, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}

void SceneShader::renderMesh()
{
	glBindVertexArray(_meshVertexArray);

	glUseProgram(_programMesh);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

   _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programMesh, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glUniform3fv(glGetUniformLocation(_programMesh, "lightPosition"), 1, glm::value_ptr(lightPosition) );

	glDrawElements( GL_TRIANGLES, _mesh->faces.size()*3, GL_UNSIGNED_INT, 0 );

	glBindVertexArray(0);
}


void SceneShader::renderLight()
{
	glUseProgram(_programLight);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

   _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));


	glUniform3fv(glGetUniformLocation(_programLight, "lightPosition"), 1, glm::value_ptr(lightPosition) );
	glUniform3fv(glGetUniformLocation(_programLight, "lightColor"), 1, glm::value_ptr(vec3(1, 1, 1)));

	glPointSize(30.0f);
	glDrawArrays( GL_POINTS, 0, 1);

}

void SceneShader::renderObstacles()
{
	glUseProgram(_programLight);

	for (int i = 0; i < boidScene->getNumberOfObstacles(); i++)
	{

	//scene matrices and camera setup
	glm::vec3 eye(0.0f, 0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	_modelview = glm::lookAt( eye, center, up);

	_projection = glm::perspective( 45.0f, _aspectRatio, 0.01f, 100.0f);

	glm::mat4 identity(1.0f);

	glm::mat4 rotationX = glm::rotate(identity, _yRot  * PI/180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

   _modelview *=  rotationX;

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "modelview"), 1, GL_FALSE, glm::value_ptr(_modelview));
	glUniformMatrix4fv(glGetUniformLocation(_programLight, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));


	glm::vec3 obstacleLocation = boidScene->getObstacleLocation(i);
	glUniform3fv(glGetUniformLocation(_programLight, "lightPosition"), 1, glm::value_ptr(obstacleLocation) );
	glUniform3fv(glGetUniformLocation(_programLight, "lightColor"), 1, glm::value_ptr(vec3(0, 1, 0)));

	float scaleFactor = 10.0;
	glPointSize(scaleFactor * boidScene->getObstacleRadius(i));
	glDrawArrays( GL_POINTS, 0, 1);
	glBindVertexArray(0);
	}

}


void SceneShader::render()
{
	renderPlane();
	renderBoids();
	renderObstacles();
	//renderMesh(); //this can be commented back on to show the bunny rendering
	renderLight();
}

void SceneShader::setZTranslation(float z)
{
	_zTranslation = z;
}

void SceneShader::setAspectRatio(float ratio)
{
	_aspectRatio = ratio;
}

void SceneShader::setRotationX( float x )
{
	_xRot = x;
}

void SceneShader::setRotationY( float y )
{
	_yRot = y;
}

void SceneShader::shutdown()
{
	glDeleteBuffers(1, &_meshVertexBuffer);
	glDeleteBuffers(1, &_meshNormalBuffer);
	glDeleteBuffers(1, &_meshIndicesBuffer );
	glDeleteVertexArrays(1, &_meshVertexArray);
	glDeleteVertexArrays(1, &_planeVertexArray);
}

void SceneShader::updateLightPositionX(float x)
{
	lightPosition.x += x;
	boidScene->setTargetLocation(lightPosition);
}

void SceneShader::updateLightPositionY(float y)
{
	lightPosition.y += y;
	boidScene->setTargetLocation(lightPosition);
}

void SceneShader::updateLightPositionZ(float z)
{
	lightPosition.z += z;
	boidScene->setTargetLocation(lightPosition);
}

SceneShader::~SceneShader()
{
	shutdown();
}


