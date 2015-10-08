/*

*/
# define __Windows__
# include "missle.h"

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