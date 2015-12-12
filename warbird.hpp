/*
warbird.hpp

Warbird class inherits from Object3D.

update() rotates or translates warbird based on keypress (+ gravity if active)
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

	glm::mat4 orientationMatrix;
	int step = 0;
	int pitch, roll, yaw = 0;
	float radians = 0;
	glm::vec3 axis;
	float distance;


public:

	float stepDistance = 10;
	const float G = 90000000.0f;
	glm::vec3 gravityVec;
	float distanceSquared;;
	float gravityMagnitude;

	Warbird(glm::vec3 translate, glm::vec3 scale, int vertices, float radius, char * modelFile) : Object3D(translate, scale, vertices, 0, radius, modelFile)
	{
		this->translate = translate;
		this->scale = scale;
		this->vertices = vertices;
		this->radius = radius;
		this->modelFile = modelFile;
	}

	void setMove(int i)
	{
		this->step = i;
	}

	void setPitch(int i)
	{
		this->pitch = i;
		this->radians = 0.02f;
	}

	void setRoll(int i)
	{
		this->roll = i;
		this->radians = 0.02f;
	}

	void setYaw(int i)
	{
		this->yaw = i;
		this->radians = 0.02f;
	}

	void update(bool gravity)
	{
		glm::vec3 zWarbird = glm::vec3(-orientationMatrix[2]);
		zWarbird = glm::normalize(zWarbird);
		distance = step * stepDistance;
		axis = glm::vec3(pitch, yaw, roll);
		//showMat4("warbird", rotationMatrix);
		if (axis != glm::vec3(0))
			rotationMatrix = glm::rotate(rotationMatrix, radians, axis);
		//showMat4("warbird", rotationMatrix);
		if (gravity)
		{
			gravityVec = glm::vec3(-orientationMatrix[3]);
			distanceSquared = pow(gravityVec.x, 2) + pow(gravityVec.y, 2) + pow(gravityVec.z, 2);
			gravityMagnitude = G / distanceSquared;
			gravityVec = normalize(gravityVec) * gravityMagnitude;
			translationMatrix = glm::translate(translationMatrix, (zWarbird * distance) + gravityVec);
		}
		else
		{
			translationMatrix = glm::translate(translationMatrix, zWarbird * distance);
		}
		orientationMatrix = translationMatrix * rotationMatrix;
		step = pitch = roll = yaw = 0;

	}


	glm::mat4 Warbird::getModelMatrix(void)
	{
		return translationMatrix * rotationMatrix * scaleMatrix;

	}

	glm::mat4 Warbird::getTranslationMatrix(void)
	{
		return translationMatrix;

	}


};
# endif