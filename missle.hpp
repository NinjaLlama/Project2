/*

*/
# ifndef MISSLE_H_
# define MISSLE_H_

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif
class Missle : public Object3D
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
	glm::mat4 missleModelMatrix;


public:

	Missle::Missle(glm::vec3 translate, glm::vec3 scale, int vertices, float radius, char * modelFile)
	{
		this->translate = translate;
		this->scale = scale;
		this->vertices = vertices;
		this->radius = radius;
		this->modelFile = modelFile;
	}


	void Missle::createModelMatrix(void)
	{
		missleModelMatrix = glm::translate(glm::mat4(), translate) *
			glm::scale(glm::mat4(), scale);

	}

	glm::mat4 Missle::getMissleMatrix(void)
	{
		return missleModelMatrix;
	}

};
# endif