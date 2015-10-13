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
		

	}

	void Planet::createModelMatrix(void)
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
	}


};
# endif