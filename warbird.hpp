/*

*/
# ifndef WARBIRD_H_
# define WARBIRD_H_

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Warbird : public Object3D
{
private:
	// distance from the sun
	glm::vec3 translate;

	//model's scale value
	glm::vec3 scale;

	//number of vertices in model
	int vertices;

	// radius of the planet
	float radius;

	// the model's file
	char * modelFile;

	//planet's model matrix
	glm::mat4 warbirdModelMatrix;


public:

	Warbird(glm::vec3 translate, glm::vec3 scale, int vertices, float radius, char * modelFile)
	{
		this->translate = translate;
		this->scale = scale;
		this->vertices = vertices;
		this->radius = radius;
		this->modelFile = modelFile;
	}


	void Warbird::createModelMatrix(void)
	{
		warbirdModelMatrix = glm::translate(glm::mat4(), translate) *
			glm::scale(glm::mat4(), scale);

	}

	glm::mat4 Warbird::getWarbirdMatrix(void)
	{
		return warbirdModelMatrix;
	}

};
# endif