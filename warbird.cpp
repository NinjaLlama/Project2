/*

*/
# define __Windows__
# include "warbird.h"

Warbird::Warbird(glm::vec3 translate, glm::vec3 scale, int vertices, float radius, char * modelFile)
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