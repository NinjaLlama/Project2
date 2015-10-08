/*

*/
# define __Windows__
# include "moon.h"

Moon::Moon(glm::vec3 translate, glm::vec3 scale, int vertices, float rotation, float orbit, float radius, char * modelFile)
{
	this->translate = translate;
	this->scale = scale;
	this->vertices = vertices;
	this->rotation = rotation;
	this->orbit = orbit;
	this->radius = radius;
	this->modelFile = modelFile;
}


void Moon::createModelMatrix(void)
{
	moonModelMatrix = rotation * glm::translate(glm::mat4(), translate) *
		glm::scale(glm::mat4(), scale);

}

void Moon::createRotationMatrix(void)
{
	rotation += orbit;
	if (rotation >  2 * PI) rotation = 0.0f;
	glm::mat4 planetRotationMatrix = glm::rotate(glm::mat4(1.0), rotation, glm::vec3(0, 1, 0));

}

glm::mat4 Moon::getMoonMatrix(void)
{
	return moonModelMatrix;
}