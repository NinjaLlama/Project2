/*

*/

# ifndef MOON_H_
# define MOON_H_

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Moon
{
private:
	// distance from the planet
	glm::vec3 translate;

	//model's scale value
	glm::vec3 scale;

	//number of vertices in model
	int vertices;

	//moon's rotation from starting point
	float rotation;

	// time it takes to orbit planet
	float orbit;

	// radius of the moon
	float radius;

	// the model's file
	char * modelFile;

	//moon's model matrix
	glm::mat4 moonModelMatrix;


public:

	Moon(glm::vec3 translate, glm::vec3 scale, int vertices, float rotation, float orbit, float radius, char * modelFile);

	void createModelMatrix(void);

	void createRotationMatrix(void);

	glm::mat4 getMoonMatrix(void);

};
# endif