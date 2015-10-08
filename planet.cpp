/*

*/
# define __Windows__
# include "planet.h"

Planet::Planet(glm::vec3 translate, glm::vec3 scale, int vertices, float rotation, float orbit, float radius, char * modelFile)
{
	this->translate = translate;
	this->scale = scale;
	this->vertices = vertices;
	this->rotation = rotation;
	this->orbit = orbit;
	this->radius = radius;
	this->modelFile = modelFile;
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
	glm::mat4 planetRotationMatrix = glm::rotate(glm::mat4(1.0), rotation, glm::vec3(0, 1, 0));

}


glm::mat4 Planet::getPlanetMatrix(void)
{
	return planetModelMatrix;
}