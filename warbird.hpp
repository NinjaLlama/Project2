/*

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

	/*// has OM an OrientationMatrix stores update
// has RM rotation and TM translation matrices, why 2 ?
int step // -1, 0, 1 values
int pitch, roll, yaw // -1, 0, 1
float radians // to rotate on axis
vec3 axis // axis = vec3(pitch, yaw, roll)
vec3 distance // vector to move
...
void setMove(int i) // step = i
void setPitch(int i) // pitch = i
...
void update()
distance = vec3(0, 0, step * stepDistance)
axis = vec3(pitch, yaw, roll)
RM = rotate(RM, axis, radians)
TM = translate(TM, distance)
OM = TM * RM
step = pitch = yaw = roll = 0*/


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
		/*distance = vec3(0, 0, step * stepDistance)
			axis = vec3(pitch, yaw, roll)
			RM = rotate(RM, axis, radians)
			TM = translate(TM, distance)
			OM = TM * RM
			step = pitch = yaw = roll = 0*/
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

	float angleBetween(
		glm::vec3 a,
		glm::vec3 b,
		glm::vec3 origin
		){
		glm::vec3 da = glm::normalize(a - origin);
		glm::vec3 db = glm::normalize(b - origin);
		return acos(glm::dot(da, db));
	}

};
# endif