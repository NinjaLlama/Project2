/*

*/
# ifndef missile_H_
# define missile_H_

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif
class missile : public Object3D
{
private:
	// distance from the sun
	glm::vec3 translate;

	//model's scale value
	glm::vec3 scale;


	//number of vertices in model
	int vertices;

	//position of the missile in z coordinates
	int num = 4850;


	//rotation of the missile
	glm::vec3 rotate;

	// radius of the planet
	float radius;

	// the model's file
	char * modelFile;

	//planet's model matrix
	glm::mat4 missileModelMatrix, orientationMatrix;


public:

	float rotation;
	int missileUpdate = 0;
	int maxUpdate = 2000;
	bool destroymissile = false;

	missile(glm::vec3 translate, glm::vec3 scale, int vertices, float rotation, float radius, char * modelFile) : Object3D(translate, scale, vertices, rotation, radius, modelFile)
	{
		this->translate = translate;
		this->scale = scale;
		this->vertices = vertices;
		this->rotation = rotation;
		this->radius = radius;
		this->modelFile = modelFile;
	}


	void missile::createModelMatrix(void)
	{
		missileModelMatrix = glm::translate(glm::mat4(), translate) *
			glm::scale(glm::mat4(), scale);

	}

	//helps with the translation of the missile position
	void setmissilePosition(glm::vec3 position)
	{

	}

	glm::vec3 getmissilePosition()
	{

	}



	//helps set the scale of the missile
	void setmissileScale(glm::vec3  model_size)
	{
		scaleMatrix = glm::scale(glm::mat4(), model_size);
	}

	void update(glm::mat4 Target, float missileSpeed)
	{

		//showMat4("target", Target);
		//showMat4("orientation", getModelMatrix());
		glm::vec3 targetVector = glm::vec3(Target[3]);

		glm::vec3 missileLatVector = glm::normalize(glm::vec3(getModelMatrix()[2]));

		glm::vec3 missileVector = glm::vec3(getModelMatrix()[3]);
		glm::vec3 directionVector = glm::normalize(targetVector - missileVector);
		float distance = pow(abs(targetVector.x - missileVector.x), 2) + pow(abs(targetVector.y - missileVector.y), 2) + pow(abs(targetVector.z - missileVector.z), 2);
		//printf("distance = %f", distance);
		/*targetVector = glm::normalize(targetVector);
		missileVector = glm::normalize(missileVector);*/
		if (missileUpdate == maxUpdate)
		{
			destroymissile = true;
		}
		else if (missileUpdate < 200 || distance > pow(5000, 2))
		{
			translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);
			missileUpdate++;
		}
		else
		{

			float radian = 0;

			float angle = acos(glm::dot(glm::normalize(targetVector - missileVector), -missileLatVector));
			/*if (angle >= PI)
				angle = 2 * PI - acos(angle);
			else
				angle = acos(angle);*/
			//printf("angle = %f\n", angle);
			angle = angle / 3;

			if (colinear(directionVector, -missileLatVector, .2f) == true)
			{
				targetVector = glm::vec3(targetVector.x + .1f, targetVector.y + (-0.2f), targetVector.z + 0.1f);
			}
			glm::vec3 axis = glm::normalize(glm::cross(glm::normalize(targetVector - missileVector), -missileLatVector));
			//glm::vec3 axis = glm::vec3(0, 1, 0);
			float axisDirection = axis.x + axis.y + axis.z;
			if (axisDirection >= 0) // adjust rotational value
				radian = angle;
			else
				radian = 2 * PI - angle;
			//printf("radian = %f\n", radian);
			if (dot(directionVector, -missileLatVector) < .98f)
			{

				rotationMatrix = glm::rotate(rotationMatrix, radian, axis);
			}


			//showMat4("rotation", rotationMatrix);



			translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);

			missileUpdate++;
		}


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
		return(translationMatrix * rotationMatrix * scaleMatrix);
	}

	float angleBetween(glm::vec3 a, glm::vec3 b, glm::vec3 origin)
	{
		glm::vec3 da = glm::normalize(a - origin);
		glm::vec3 db = glm::normalize(b - origin);
		return acos(glm::dot(da, db));
	}


	glm::mat4 missile::getmissileMatrix(void)
	{
		return missileModelMatrix;
	}

	void update(glm::mat4 Target1, glm::mat4 Target2, float missileSpeed)
	{

		//showMat4("target", Target);
		//showMat4("orientation", orientationMatrix);

		glm::vec3 missileLatVector = glm::normalize(glm::vec3(getModelMatrix()[2]));
		glm::vec3 missileVector = glm::vec3(getModelMatrix()[3]);

		glm::vec3 target1Vector = glm::vec3(Target1[3]);
		glm::vec3 direction1Vector = glm::normalize(target1Vector - missileVector);
		float distance1 = pow(abs(target1Vector.x - missileVector.x), 2) + pow(abs(target1Vector.y - missileVector.y), 2) + pow(abs(target1Vector.z - missileVector.z), 2);

		glm::vec3 target2Vector = glm::vec3(Target2[3]);
		glm::vec3 direction2Vector = glm::normalize(target2Vector - missileVector);
		float distance2 = pow(abs(target2Vector.x - missileVector.x), 2) + pow(abs(target2Vector.y - missileVector.y), 2) + pow(abs(target2Vector.z - missileVector.z), 2);
		//printf("distance = %f", distance1);
		/*target1Vector = glm::normalize(target1Vector);
		missileVector = glm::normalize(missileVector);
		target2Vector = glm::normalize(target2Vector);*/
		if (missileUpdate == maxUpdate)
		{
			destroymissile = true;
		}
		else
		{
			if (distance1 < pow(5000, 2) && distance1 < distance2)
			{
				if (missileUpdate < 200)
				{
					translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);
					missileUpdate++;
				}
				else
				{

					float radian = 0;

					float angle = acos(glm::dot(glm::normalize(target1Vector - missileVector), -missileLatVector));
					/*if (angle >= 180)
						angle = 2 * PI - acos(angle);
						else
						angle = acos(angle);*/
					//printf("angle = %f\n", angle);
					angle = angle / 3;

					if (colinear(direction1Vector, -missileLatVector, .2f) == true)
					{
						target1Vector = glm::vec3(target1Vector.x + .1f, target1Vector.y + (-0.2f), target1Vector.z + 0.1f);
					}
					glm::vec3 axis = glm::normalize(glm::cross(glm::normalize(target1Vector - missileVector), -missileLatVector));
					//glm::vec3 axis = glm::vec3(0, 1, 0);
					float axisDirection = axis.x + axis.y + axis.z;
					if (axisDirection >= 0) // adjust rotational value
						radian = angle;
					else
						radian = 2 * PI - angle;
					//printf("radian = %f\n", radian);
					if (dot(direction1Vector, -missileLatVector) < .98f)
					{

						rotationMatrix = glm::rotate(rotationMatrix, radian, axis);
					}


					//showMat4("rotation", rotationMatrix);



					translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);

					missileUpdate++;
				}


			}
			else if (distance2 < pow(5000, 2) && distance2 < distance1)
			{
				if (missileUpdate < 200)
				{
					translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);
					missileUpdate++;
				}
				else
				{

					float radian = 0;

					float angle = acos(glm::dot(glm::normalize(target2Vector - missileVector), -missileLatVector));
					/*if (angle >= PI)
						angle = 2 * PI - acos(angle);
						else
						angle = acos(angle);*/
					//printf("angle = %f\n", angle);
					angle = angle / 3;

					if (colinear(direction2Vector, -missileLatVector, .2f) == true)
					{
						target2Vector = glm::vec3(target2Vector.x + .1f, target2Vector.y + (-0.2f), target2Vector.z + 0.1f);
					}
					glm::vec3 axis = glm::normalize(glm::cross(glm::normalize(target2Vector - missileVector), -missileLatVector));
					//glm::vec3 axis = glm::vec3(0, 1, 0);
					float axisDirection = axis.x + axis.y + axis.z;
					if (axisDirection >= 0) // adjust rotational value
						radian = angle;
					else
						radian = 2 * PI - angle;
					//printf("radian = %f\n", radian);
					if (dot(direction2Vector, -missileLatVector) < .98f)
					{

						rotationMatrix = glm::rotate(rotationMatrix, radian, axis);
					}


					//showMat4("rotation", rotationMatrix);



					translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);

					missileUpdate++;
				}
			}
			else
			{
				translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);
				missileUpdate++;
			}
		}
	}


};
# endif