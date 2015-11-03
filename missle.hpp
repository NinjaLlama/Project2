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
	glm::mat4 missleModelMatrix;


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

	

	//helps set the scale of the missle
	void setMissleScale(glm::vec3  model_size)
	{
		scaleMatrix = glm::scale(glm::mat4(), model_size);
	}

	void update()
	{
		if (num>=-100){
			translationMatrix = glm::translate(glm::mat4(1.0), glm::vec3(4950, 100, num));
			num = num - 100;
			printf("%d \n",num);
			//showMat4("translate", translationMatrix);
		} 
	}

	glm::mat4 getModelMatrix() {
		return(rotationMatrix * translationMatrix * scaleMatrix);
	}

	

	glm::mat4 Missle::getMissleMatrix(void)
	{
		return missleModelMatrix;
	}

};
# endif