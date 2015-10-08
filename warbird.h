/*

*/
# ifndef WARBIRD_H_
# define WARBIRD_H_

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Warbird
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

	//planet's model matrix
	glm::mat4 warbirdModelMatrix;


public:

	Warbird(glm::vec3 translate, glm::vec3 scale, int vertices, float radius, char * modelFile);

	void createModelMatrix(void);

	glm::mat4 getWarbirdMatrix(void);

};
# endif