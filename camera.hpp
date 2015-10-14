/*

*/


class Camera : public Object3D
{
private:


public:

	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	float rotation;
	char * name;
	glm::mat4 viewMatrix;

	Camera()
	{

	}

	Camera(glm::vec3 eye, glm::vec3 at, glm::vec3 up, float rotation, char * name)
	{
		this->eye = eye;
		this-> at = at;
		this->up = up;
		this->rotation = rotation;
		this->name = name;

	}

	glm::mat4 getViewMatrix(void)
	{
		return viewMatrix = glm::lookAt(eye, at, up);
	}

	char * getCameraString(void)
	{
		return this->name;
	}


};