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

	//position of the missle in z coordinates
	int num = 4850;
	

	//rotation of the missle
	glm::vec3 rotate;

	// radius of the planet
	float radius;

	// the model's file
	char * modelFile;

	//planet's model matrix
	glm::mat4 missleModelMatrix, orientationMatrix;


public:

	float rotation;

	Missle::Missle(glm::vec3 translate, glm::vec3 scale, int vertices, float rotation, float radius, char * modelFile) : Object3D(translate, scale, vertices, rotation, radius, modelFile)
	{
		this->translate = translate;
		this->scale = scale;
		this->vertices = vertices;
		this->rotation = rotation;
		this->radius = radius;
		this->modelFile = modelFile;
	}


	void Missle::createModelMatrix(void)
	{
		missleModelMatrix = glm::translate(glm::mat4(), translate) *
			glm::scale(glm::mat4(), scale);

	}

	//helps with the translation of the missle position
	void setMisslePosition(glm::vec3 position)
	{
		
	}

	glm::vec3 getMisslePosition()
	{

	}

	

	//helps set the scale of the missle
	void setMissleScale(glm::vec3  model_size)
	{
		scaleMatrix = glm::scale(glm::mat4(), model_size);
	}

	void update(glm::mat4 Target)
	{

		showMat4("target", Target);
		showMat4("translate", translationMatrix);
		glm::vec3 targetVector = glm::normalize(glm::vec3(Target[2]));
		
		glm::vec3 missleVector = glm::normalize(glm::vec3(orientationMatrix[2]));

		
	   
		float angle = angleBetween(missleVector, targetVector, glm::vec3(0));

		//glm::vec3 axis = glm::normalize(glm::cross(missleVector, targetVector));
		glm::vec3 axis = glm::vec3(0, 1, 0);
		if (axis != glm::vec3(0))
		{

			rotationMatrix = glm::rotate(rotationMatrix, angle, axis);
		}

		rotationMatrix = glm::rotate(rotationMatrix, angle, axis);

		showMat4("rotation",rotationMatrix);
			orientationMatrix = translationMatrix*rotationMatrix;

		/*
		if (num>=0){
			translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(4950, 100, num));
			rotationMatrix = glm::rotate(glm::mat4(1.0), PI/16, glm::vec3(0, 1,0));
			num = num - 100;
			printf("%d \n",num);
			//showMat4("translate", translationMatrix);
			
		} 
		*/
	}





	glm::mat4 getDirectionMatrix(const glm::vec3& object, const glm::vec3& target) {
		glm::vec3 up;
		glm::vec3 direction(glm::normalize(target - object));
		up = glm::vec3(0.0, 1.0, 0.0);
		up = glm::normalize(up);

		glm::vec3 right = glm::normalize(glm::cross(up, direction));
		up = glm::normalize(glm::cross(direction, right));

		return glm::mat4(right.x, right.y, right.z, 0.0f,
			up.x, up.y, up.z, 0.0f,
			direction.x, direction.y, direction.z, 0.0f,
			object.x, object.y, object.z, 1.0f);
	}







	glm::mat4 getModelMatrix() {
		return(rotationMatrix * translationMatrix * scaleMatrix);
	}

	float angleBetween(glm::vec3 a,glm::vec3 b,glm::vec3 origin) 
	{
		glm::vec3 da = glm::normalize(a - origin);
		glm::vec3 db = glm::normalize(b - origin);
		return acos(glm::dot(da, db));
	}
	

	glm::mat4 Missle::getMissleMatrix(void)
	{
		return missleModelMatrix;
	}

};
# endif