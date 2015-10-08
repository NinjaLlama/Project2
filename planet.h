/*

*/

# ifndef PLANET_H_
# define PLANET_H_

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif


class Planet
{
private:
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

	//planet's model matrix
	glm::mat4 planetModelMatrix;

	//planet's rotation matrix
	glm::mat4 planetRotationMatrix;


public:

	Planet(glm::vec3 translate, glm::vec3 scale, int vertices, float rotation, float orbit, float radius, char * modelFile);

	void createModelMatrix(void);

	void createRotationMatrix(void);

	glm::mat4 getPlanetMatrix(void);


};
# endif