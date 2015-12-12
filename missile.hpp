/*
missile.hpp

Missile class inherits from Object3D.

Two update() methods, one for one possible target and one for two possible targets.
Both operate in the same manner: find target, determine angle of roation and axis of rotation, then move forward at set speed.
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


	//set the scale of the missile
	void setmissileScale(glm::vec3  model_size)
	{
		scaleMatrix = glm::scale(glm::mat4(), model_size);
	}

	void update(glm::mat4 Target, float missileSpeed)
	{

		//showMat4("target", Target);
		//showMat4("orientation", getModelMatrix());
		glm::vec3 targetVector = glm::vec3(Target[3]); //target's position

		glm::vec3 missileLatVector = glm::normalize(glm::vec3(getModelMatrix()[2])); //missile looking at vector

		glm::vec3 missileVector = glm::vec3(getModelMatrix()[3]); //missile's position
		glm::vec3 directionVector = glm::normalize(targetVector - missileVector); //vector leading from missile to target
		//distance squared between target and missile
		float distance = pow(abs(targetVector.x - missileVector.x), 2) + pow(abs(targetVector.y - missileVector.y), 2) + pow(abs(targetVector.z - missileVector.z), 2);
		//printf("distance = %f", distance);
		/*targetVector = glm::normalize(targetVector);
		missileVector = glm::normalize(missileVector);*/
		if (missileUpdate == maxUpdate) //missile's life is over
		{
			destroymissile = true;
		}
		else if (missileUpdate < 200 || distance > pow(5000, 2)) //no target within detection range
		{
			translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);
			missileUpdate++;
		}
		else //target detected
		{

			float radian = 0;
			// find the angle to rotate from the dot of the direction vector the the missile looking at vector
			float angle = acos(glm::dot(glm::normalize(targetVector - missileVector), -missileLatVector));
			//printf("angle = %f\n", angle);
			angle = angle / 3;
			if (colinear(directionVector, -missileLatVector, .2f) == true && dot(directionVector, -missileLatVector) == 1.0f)
			{
				//the vectors are colinear and facing the same direction, don't do any rotations
			}
			else
			{
				if (colinear(directionVector, -missileLatVector, .2f) == true) //if colinear adjust the target vector a bit
				{
					targetVector = glm::vec3(targetVector.x + .1f, targetVector.y + (-0.2f), targetVector.z + 0.1f);
				}
				// find the axis to rotate from the cross of the direction vector and the missile looking at vector
				glm::vec3 axis = glm::normalize(glm::cross(glm::normalize(targetVector - missileVector), -missileLatVector));
				//glm::vec3 axis = glm::vec3(0, 1, 0);
				float axisDirection = axis.x + axis.y + axis.z;
				if (axisDirection >= 0) // adjust rotational value
					radian = angle;
				else
					radian = 2 * PI - angle;
				//printf("radian = %f\n", radian);
				//if the angle to rotate if large enough, rotate
				//seems to help steady the missile
				if (dot(glm::normalize(targetVector - missileVector), -missileLatVector) < .98f) 
				{

					rotationMatrix = glm::rotate(rotationMatrix, radian, axis);
				}


				//showMat4("rotation", rotationMatrix);

			}

			translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);

			missileUpdate++;
		}

	}


	glm::mat4 getModelMatrix() {
		return(translationMatrix * rotationMatrix * scaleMatrix);
	}

	glm::mat4 missile::getmissileMatrix(void)
	{
		return missileModelMatrix;
	}

	void update(glm::mat4 Target1, glm::mat4 Target2, float missileSpeed)
	{
		//follows same procedure as previous update()

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
			if (distance1 < pow(5000, 2) && distance1 < distance2) //check is target1 is in range and closer than target2
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
					//printf("angle = %f\n", angle);
					angle = angle / 3;
					if (colinear(direction1Vector, -missileLatVector, .2f) == true && dot(direction1Vector, -missileLatVector) == 1.0f)
					{
						//nothing
					}
					else
					{
						if (colinear(direction1Vector, -missileLatVector, .2f) == true)
						{
							printf("here!\n");
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
						if (dot(glm::normalize(target1Vector - missileVector), -missileLatVector) < .98f)
						{

							rotationMatrix = glm::rotate(rotationMatrix, radian, axis);
						}


						//showMat4("rotation", rotationMatrix);


					}
					translationMatrix = glm::translate(translationMatrix, -missileLatVector*missileSpeed);

					missileUpdate++;
				}


			}
			else if (distance2 < pow(5000, 2) && distance2 < distance1) //check is target2 is in range and closer than target1
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
					//printf("angle = %f\n", angle);
					angle = angle / 3;
					if (colinear(direction2Vector, -missileLatVector, .2f) == true && dot(direction2Vector, -missileLatVector) == 1.0f)
					{
						//nothing
					}
					else
					{
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
						if (dot(glm::normalize(target2Vector - missileVector), -missileLatVector) < .98f)
						{

							rotationMatrix = glm::rotate(rotationMatrix, radian, axis);
						}


						//showMat4("rotation", rotationMatrix);

					}

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