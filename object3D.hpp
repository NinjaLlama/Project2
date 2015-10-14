/*

*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Object3D {

private:

public:

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
	bool isDuo = false;
	glm::mat4 duoTranslate = glm::translate(glm::mat4(), glm::vec3(9000, 0, 0));

	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;
	glm::vec3 rotationAxis = glm::vec3(0.0, 1.0, 0.0);
	glm::mat4 DuoRotation = glm::rotate(glm::mat4(1.0), PI, glm::vec3(0, 1, 0));

	Object3D(){}

	Object3D(glm::vec3 translate, glm::vec3 scale, int vertices, float rotation, float radius, char * modelFile)
	{
		this->translate = translate;
		this->scale = scale;
		this->vertices = vertices;
		this->rotation = rotation;
		this->radius = radius;
		this->modelFile = modelFile;

		rotationMatrix = glm::mat4();
		scaleMatrix = glm::scale(glm::mat4(), scale);
		translationMatrix = glm::translate(glm::mat4(), translate);
	}

	glm::mat4 getModelMatrix() {
	return(rotationMatrix * translationMatrix * scaleMatrix);
	}

	void update() {
		if (isDuo)
		{
			rotationMatrix = glm::rotate(rotationMatrix, rotation, rotationAxis) * DuoRotation;
			isDuo = false;
		}
		else
		{
			rotationMatrix = glm::rotate(rotationMatrix, rotation, rotationAxis);
		}
	}

	void Duo(void)
	{
		this->isDuo = true;
	}
};