/*

*/

# ifndef PLANET_H_
# define PLANET_H_

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif


class Planet : public Object3D
{
private:
	// distance from the sun
	glm::vec3 translate;

	//model's scale value
	glm::vec3 scale;

	//number of vertices in model
	int vertices;

	//planet's rotation from starting point
	float rotation;

	// time it takes to orbit sun
	float orbit;

	// radius of the planet
	float radius;

	// the model's file
	char * modelFile;

	//planet's model matrix
	glm::mat4 planetModelMatrix;

	//planet's rotation matrix
	glm::mat4 planetRotationMatrix;


public:

	Planet(glm::vec3 translate, glm::vec3 scale, int vertices, float rotation, float radius, char * modelFile) : Object3D(translate, scale,  vertices, rotation, radius, modelFile)
	{
		this->translate = translate;
		this->scale = scale;
		this->vertices = vertices;
		this->rotation = rotation;
		this->radius = radius;
		this->modelFile = modelFile;

	}

	void DuoRotate180(void)
	{
		rotationMatrix = glm::rotate(rotationMatrix, rotation, rotationAxis) * glm::rotate(glm::mat4(1.0), PI, glm::vec3(0, 1, 0));
	}

	glm::mat4 Moon(glm::mat4 &planetMatrix, glm::mat4 &moonMatrix)
	{
		return glm::translate(glm::mat4(), glm::vec3(planetMatrix[3])) * moonMatrix;
	}

	glm::mat4 MissleSite(glm::mat4 &planetMatrix, glm::mat4 &missleMatrix)
	{
		return glm::translate(glm::mat4(), glm::vec3(planetMatrix[3])) * missleMatrix;
	}

	glm::mat4 getModelMatrix() {
		return(rotationMatrix * translationMatrix * scaleMatrix);
	}

	void update() {

		rotationMatrix = glm::rotate(rotationMatrix, rotation, rotationAxis);

	}

	/*void Planet::createModelMatrix(void)
	{
		planetModelMatrix = planetRotationMatrix * glm::translate(glm::mat4(), translate) *
			glm::scale(glm::mat4(), scale);

	}

	void Planet::createRotationMatrix(void)
	{
		rotation += orbit;
		if (rotation >  2 * PI) rotation = 0.0f;
		glm::mat4 planetRotationMatrix = glm::rotate(planetRotationMatrix, rotation, glm::vec3(0, 1, 0));

	}


	glm::mat4 Planet::getPlanetMatrix(void)
	{
		return planetModelMatrix;
	}*/


};
# endif