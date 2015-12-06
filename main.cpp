/*

Comp 465 Warbird Simulation Phase 1
Therese Horey
Jesus Moran Perez

Project1.cpp

This file contains all the C++ code for Phase 1.
All models are loaded into an array and are given an initial size and translate vector.
The init() method loads the files and sets the initial view (front view).
The display() method handles the planet and moon rotation animations, as well as dynamic cameras for Unum and Duo.
The update() method creates the two rotation matrices for Unum and Primus, and Duo and Secundus.
The keyboard() method handles keypress controls.

Controls:
- v to toggle camera
- d to toggle debug options *not fully implemented*
- q to exit program

TODO : Change program to an object oriented style. Write classes for Planet, Moon, Warbird, and Camera(?).


*** If 'Start Without Debugging' is unselectable,
go to the Solution properties in the Solution Explorer
and make sure 'Single Startup Project' is selected ***

*/

# define __Windows__
# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif
# include "../includes465/texture.hpp"
# include "object3D.hpp"
# include "planet.hpp"
# include "warbird.hpp"
# include "missile.hpp"
# include "camera.hpp"


const int X = 0, Y = 1, Z = 2, W = 3, START = 0, STOP = 1;
// constants for models:  file names, vertex count, model display size
const int nModelsLoaded = 10;  // number of model files
const int nModels = 13;  // number of models in this scene (two missile sites and up to three active missiles)
const int nCameras = 5;
const int nCollisions = 9;
char * modelFile[nModelsLoaded] = { "Ruber.tri", "Unum.tri ", "Duo.tri", "Primus.tri",
"Segundus.tri", "BattleCruiser.tri", "missile.tri", "axes-r100.tri", "missileBase.tri", "plane.tri" };
float modelBR[nModelsLoaded];       // model's bounding radius
float scaleValue[nModelsLoaded];    // model's scaling "size" value
const int nVertices[nModelsLoaded] = { 264 * 3, 264 * 3, 278 * 3, 264 * 3, 264 * 3, 2772 * 3, 644 * 3, 120 * 3, 1382 * 3, 2*3 };
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint shaderProgram;
GLuint VAO[nModelsLoaded];      // Vertex Array Objects
GLuint buffer[nModelsLoaded];   // Vertex Buffer Objects

int width = 640, height = 480;    
char * fileName = "rawFile2.raw";   
GLuint texture, Texture, vTexCoord, showTexture;  // texture id

static const GLfloat point[] = {
	0.615, 0.435, 0, -0.615, -0.435, 0, -0.615, 0.435,0
	-0.615, -0.435, 0, 0.615, 0.435, 0, 0.615, -0.435,0 };

// Texture Coordinates for each vertex
// points * 2 (s, t)
static const GLfloat texCoords[] = {
	0.0f, 0.0f,     // 0
	1.0f, 0.0f,     // 1
	1.0f, 1.0f,     // 2
	0.0f, 1.0f     // 3
};   

//// vectors and values for lookAt
//glm::vec3 eye, at, up;

Camera * camera[nCameras];
int toggleCam = 0;

float shipmissileSpeed = 20.0f;
float sitemissileSpeed = 5.0f;

int missileCountWarbird = 10;
int missileCountUnum = 5;
int missileCountSecundus = 5;

//camera view booleans
bool cycleForward = true;
bool cycleBackward = false;

//ship warp boolean
bool atUnum = false;

//collision booleans
bool warbirdCollision[nCollisions];
bool warbirdMissileCollision[nCollisions];
bool siteUnumMissileCollision[nCollisions];
bool siteSecundusMissileCollision[nCollisions];

bool siteUnumDestroyed = false;
bool siteSecundusDestroyed = false;
bool warbirdDestroyed = false;

bool gravity = false;

//active missile booleans
bool activemissileWarbird = false;
bool activemissileUnum = false;
bool activemissileSecundus = false;

//debug boolean
bool debug = false;

// window title strings
char baseStr[50] = "465 Project 1 : ";
char viewStr[15] = "  View Front";
char warbirdStr[15] = "  Warbird 10";
char unumStr[15] = "  Unum 5";
char secundusStr[15] = "  Secundus 5";
char fpsStr[15] = "  F/S ??";
char updateStr[20] = "  U/S ??";
char titleStr[500];

// rotation variables  -- the modelMatrix
GLfloat rotateRadianOne = 0.0f;
GLfloat rotateRadianTwo = 0.0f;
glm::mat4 identity(1.0f);
glm::mat4 rotationOne;  // the modelMatrix
glm::mat4 rotationTwo;
//update rate: ace, pilot, trainee, debug
int timerDelay[] = { 40, 100, 250, 500 };
int nTimers = 4;
int timer = 0;
int frameCount = 0;  // for idle fps
double currentTime, lastTime, timeInterval;
int updateCount = 0;  // for update rate
double currentUpdateTime, lastUpdateTime, timeIntervalUpdate;

// Shader handles, matrices, etc
GLuint MVP, model_location; // Model View Projection matrix's handle
GLuint vPosition[nModelsLoaded], vColor[nModelsLoaded], vNormal[nModelsLoaded];   // vPosition, vColor, vNormal handles for models
// model, view, projection matrices and values to create modelMatrix.
//loaded in order of Ruber, Umun, Duo, Primus, Secundus, Warbird, missiles
float modelSize[nModelsLoaded] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 150.0f, 25.0f, 150.0f, 30.0f, 2000.0f };   // size of model
glm::vec3 scale[nModelsLoaded];       // set in init()
glm::vec3 translate[nModelsLoaded] = { glm::vec3(0, 0, 0), glm::vec3(4000, 0, 0), glm::vec3(9000, 0, 0),
glm::vec3(900, 0, 0), glm::vec3(1750, 0, 0),
glm::vec3(5000, 1000, 5000), glm::vec3(4900, 1000, 4850), glm::vec3(5000, 1000, 5000), glm::vec3(5000, 1000, 5000)


};


Planet * ruber;
Planet * unum;
Planet * duo;
Planet * primus;
Planet * secundus;
Warbird * warbird;
Warbird * axes;
missile * missileWarbird;
missile * missileSiteUnum;
missile * missileSiteSecundus;
Planet * siteUnum;
Planet * siteSecundus;
Planet * plane;

glm::mat4 modelMatrix[nModels];          // set in display()
glm::mat4 viewMatrix;           // set in init()
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();




void reshape(int width, int height) {
	float aspectRatio = (float)width / (float)height;
	float FOVY = glm::radians(60.0f);
	glViewport(0, 0, width, height);
	printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n",
		FOVY, width, height, aspectRatio);
	projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f);
}

// update and display animation state in window title
void updateTitle() {
	strcpy(titleStr, baseStr);
	strcat(titleStr, warbirdStr);
	strcat(titleStr, unumStr);
	strcat(titleStr, secundusStr);
	strcat(titleStr, updateStr);
	strcat(titleStr, fpsStr);
	strcat(titleStr, viewStr);
	//printf("title string = %s \n", titleStr);
	if (warbirdDestroyed)
		strcpy(titleStr, "Your ship has been destroyed!");
	if ((missileCountWarbird == 0 && !activemissileWarbird) && !(siteUnumDestroyed && siteSecundusDestroyed))
		strcpy(titleStr, "Cadet resigns from War College");
	if (siteUnumDestroyed && siteSecundusDestroyed)
		strcpy(titleStr, "Cadet passes flight training");
	glutSetWindowTitle(titleStr);
}

bool collision(glm::mat4 object1, glm::mat4 object2, float radius1, float radius2)
{
	bool collision = false;
	glm::vec3 position1 = glm::vec3(object1[3]);
	glm::vec3 position2 = glm::vec3(object2[3]);

	float distance = pow(abs(position1.x - position2.x), 2) + pow(abs(position1.y - position2.y), 2) + pow(abs(position1.z - position2.z), 2);

	float sumRadius = (radius1 + radius2)*(radius1 + radius2);

	//printf("%f, %f\n", distance, sumRadius);

	if (distance < sumRadius)
		collision = true;
	else
		collision = false;
	return collision;
}

float distance(glm::mat4 object1, glm::mat4 object2)
{
	glm::vec3 position1 = glm::vec3(object1[3]);
	glm::vec3 position2 = glm::vec3(object2[3]);

	float distance = pow(abs(position1.x - position2.x), 2) + pow(abs(position1.y - position2.y), 2) + pow(abs(position1.z - position2.z), 2);
	//printf("distance = %f", distance);
	return distance;
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// update model matrix
	glUniform1f(showTexture, 0);
	modelMatrix[0] = ruber->getModelMatrix();
	modelMatrix[1] = unum->getModelMatrix();
	modelMatrix[2] = duo->getModelMatrix();
	modelMatrix[3] = primus->Moon(duo->getModelMatrix(), primus->getModelMatrix());
	modelMatrix[4] = secundus->Moon(duo->getModelMatrix(), secundus->getModelMatrix());
	modelMatrix[5] = warbird->getModelMatrix();
	modelMatrix[6] = siteUnum->missileSite(unum->getModelMatrix(), siteUnum->getModelMatrix());
	modelMatrix[7] = siteSecundus->missileSite(modelMatrix[4], siteSecundus->getModelMatrix());
	modelMatrix[8] = missileWarbird->getModelMatrix();
	modelMatrix[9] = missileSiteUnum->getModelMatrix();
	modelMatrix[10] = missileSiteSecundus->getModelMatrix();
	modelMatrix[11] = missileSiteSecundus->getModelMatrix();
	modelMatrix[12] = plane->getModelMatrix();

	//showMat4("duo", modelMatrix[2]);
	for (int m = 0; m < nModels; m++) {
		//dynamic cameras
		//warbird camera

		
		if (cycleForward && toggleCam == 3 || cycleBackward && toggleCam == 2)
		{
			//(glm::vec3(5000.0f, 1300.0f, 6000.0f), glm::vec3(5000.0f, 1000.0f, 5000.0f), glm::vec3(0.0f, 1.0f, 0.0f)
			glm::vec3 zWarbird = glm::vec3(modelMatrix[5][2][0], modelMatrix[5][2][1], modelMatrix[5][2][2]);
			zWarbird = glm::normalize(zWarbird);
			glm::vec3 upWarbird = glm::vec3(modelMatrix[5][1]);
			upWarbird = glm::normalize(upWarbird);
			camera[2]->eye = glm::vec3(modelMatrix[5][3]) + upWarbird * 300.0f + zWarbird * 1000.0f;
			camera[2]->at = glm::vec3(modelMatrix[5][3]) + upWarbird * 300.0f;
			camera[2]->up = upWarbird;
			viewMatrix = camera[2]->getViewMatrix();
		}
		//Unum camera
		if (cycleForward && toggleCam == 4 || cycleBackward && toggleCam == 3)
		{
			//90 degrees CW about y-axis: (x, y, z) -> (-z, y, x) -- this is how to get z-axis from x-axis --
			glm::vec3 zUnum = glm::vec3(unum->getModelMatrix()[0][2], unum->getModelMatrix()[0][1] * -1, unum->getModelMatrix()[0][0] * -1);
			zUnum = glm::normalize(zUnum);
			camera[3]->eye = glm::vec3(unum->getModelMatrix()[3]) + zUnum * 8000.0f;        // camera is 4000 units out along Unum's -z axis        
			camera[3]->at = glm::vec3(unum->getModelMatrix()[3]);							// camera is looking at Unum
			camera[3]->up = glm::vec3(0.0f, 1.0f, 0.0f);             // camera's up is Y
			viewMatrix = camera[3]->getViewMatrix();
		}
		//Duo camera
		if (cycleForward && toggleCam == 5 || cycleBackward && toggleCam == 4)
		{
			//90 degrees CW about y-axis: (x, y, z) -> (-z, y, x) -- this is how to get z-axis from x-axis --
			//glm::vec3 zDuo = glm::vec3(duo->getModelMatrix()[0][2], duo->getModelMatrix()[0][1] * -1, duo->getModelMatrix()[0][0] * -1);
			//oh, this works too: (-1)at vector for -z axis
			glm::vec3 zDuo = glm::vec3(duo->getModelMatrix()[2][0] * -1, duo->getModelMatrix()[2][1] * -1, duo->getModelMatrix()[2][2] * -1);
			//showVec3("Duo", zDuo);
			zDuo = glm::normalize(zDuo);
			camera[4]->eye = glm::vec3(duo->getModelMatrix()[3]) + zDuo * 8000.0f;				// camera is 4000 units out along Duo's -z axis
			camera[4]->at = glm::vec3(duo->getModelMatrix()[3]);								// camera is looking at Duo
			camera[4]->up = glm::vec3(0.0f, 1.0f, 0.0f);                 // camera's up is Y
			viewMatrix = camera[4]->getViewMatrix();
		}
		
		// glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr( modelMatrix)); 
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix[m];
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(modelMatrix[m]));
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
        model_location=glGetUniformLocation(shaderProgram,"model");
		if (m < 6)
		{
			glBindVertexArray(VAO[m]);
			glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);
		}
		else if (m == 6 || m == 7) //missile sites
		{
			glBindVertexArray(VAO[8]);
			glDrawArrays(GL_TRIANGLES, 0, nVertices[8]);
		}
		else if (m == 8 ) //site Unum missile
		{
			glBindVertexArray(VAO[6]);
			glDrawArrays(GL_TRIANGLES, 0, nVertices[6]);
		}
		else if (m == 9 ) //site Secundus missile
		{
			glBindVertexArray(VAO[6]);
			glDrawArrays(GL_TRIANGLES, 0, nVertices[6]);
		}
		else if (m == 10 ) //warbird missile
		{
			glBindVertexArray(VAO[6]);
			glDrawArrays(GL_TRIANGLES, 0, nVertices[6]);
		}
		else if (debug && m == 11) //debug axes
		{
			glBindVertexArray(VAO[7]);
			glDrawArrays(GL_TRIANGLES, 0, nVertices[7]);
		}
		else if (m == 12)
		{
			glUniform1f(showTexture, 1);
			glBindVertexArray(VAO[9]);
			glDrawArrays(GL_TRIANGLES, 0, nVertices[9]);
		}
		else
		{
			//do nothing
		}

	}
	glutSwapBuffers();
	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if (timeInterval >= 1000) {
		sprintf(fpsStr, "  F/S %4d", (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}

// to set rotation
void update(void){
	unum->update();
	duo->update();
	primus->update();
	secundus->update();
	warbird->update(gravity);
	axes->update(gravity);
	//if peron presses f then the fire missile function is called
	if (activemissileWarbird)
	{
		if (siteUnumDestroyed) //missile site Unum destroyed, missle should only seek Secundus site
			missileWarbird->update(modelMatrix[7], shipmissileSpeed);
		else if (siteSecundusDestroyed)  //missile site Secundus destroyed, missle should only seek Unum site
			missileWarbird->update(modelMatrix[6], shipmissileSpeed);
		else //missile can seek either Unum or Secundus site
			missileWarbird->update(modelMatrix[6], modelMatrix[7], shipmissileSpeed);

		warbirdMissileCollision[0] = collision(modelMatrix[8], modelMatrix[6], modelSize[8] + 10.0f, modelSize[6] + 20.0f); //missile hits site Unum
		warbirdMissileCollision[1] = collision(modelMatrix[8], modelMatrix[7], modelSize[8] + 10.0f, modelSize[7] + 20.0f); //missile hits site Secundus
		//warbird missile collision with planets
		warbirdMissileCollision[2] = collision(modelMatrix[8], ruber->getModelMatrix(), modelSize[8] + 10.0f, modelSize[0]);
		warbirdMissileCollision[3] = collision(modelMatrix[8], unum->getModelMatrix(), modelSize[8] + 10.0f, modelSize[1]);
		warbirdMissileCollision[4] = collision(modelMatrix[8], duo->getModelMatrix(), modelSize[8] + 10.0f, modelSize[2]);
		warbirdMissileCollision[5] = collision(modelMatrix[8], primus->Moon(duo->getModelMatrix(), primus->getModelMatrix()), modelSize[8] + 10.0f, modelSize[3]);
		warbirdMissileCollision[6] = collision(modelMatrix[8], secundus->Moon(duo->getModelMatrix(), secundus->getModelMatrix()), modelSize[8] + 10.0f, modelSize[4]);
		//warbird missile collision with warbird
		if (missileWarbird->missileUpdate > 200) //after missile is live
		{
			warbirdCollision[7] = collision(modelMatrix[8], modelMatrix[5], modelSize[8] + 10.0f, modelSize[5] + 20.0f);
		}

	}
	//check if unum missile site should fire a missile
	if (distance(warbird->getModelMatrix(), siteUnum->missileSite(unum->getModelMatrix(), siteUnum->getModelMatrix())) < pow(5000, 2) && (!activemissileUnum && missileCountUnum != 0))
	{
		activemissileUnum = true;
		missileSiteUnum->translationMatrix = glm::translate(identity, glm::vec3(siteUnum->missileSite(unum->getModelMatrix(), siteUnum->getModelMatrix())[3]));
		missileSiteUnum->rotationMatrix = siteUnum->rotationMatrix * glm::rotate(identity, PI/2, glm::vec3(1,0,0));
		missileCountUnum--;
		sprintf(unumStr, "  Unum %2d", missileCountUnum);

	}
	//update unum missile
	if (activemissileUnum)
	{
		missileSiteUnum->update(warbird->getModelMatrix(), sitemissileSpeed);
		warbirdCollision[5] = collision(modelMatrix[9], modelMatrix[5], modelSize[9] + 10.0f, modelSize[5] + 20.0f); //Unum missile hits warbird
		//missile Unum collision with planets
		if (missileSiteUnum->missileUpdate > 200)
		{
			siteUnumMissileCollision[0] = collision(modelMatrix[9], ruber->getModelMatrix(), modelSize[9] + 10.0f, modelSize[0]);
			siteUnumMissileCollision[1] = collision(modelMatrix[9], unum->getModelMatrix(), modelSize[9] + 10.0f, modelSize[1]);
			siteUnumMissileCollision[2] = collision(modelMatrix[9], duo->getModelMatrix(), modelSize[9] + 10.0f, modelSize[2]);
			siteUnumMissileCollision[3] = collision(modelMatrix[9], primus->Moon(duo->getModelMatrix(), primus->getModelMatrix()), modelSize[9] + 10.0f, modelSize[3]);
			siteUnumMissileCollision[4] = collision(modelMatrix[9], secundus->Moon(duo->getModelMatrix(), secundus->getModelMatrix()), modelSize[9] + 10.0f, modelSize[4]);
		}

	}
	//check if secundus missile site should fire a missile
	if (distance(warbird->getModelMatrix(), modelMatrix[7]) < pow(5000, 2) && (!activemissileSecundus && missileCountSecundus != 0))
	{
		activemissileSecundus = true;
		missileSiteSecundus->translationMatrix = glm::translate(identity, glm::vec3(modelMatrix[7][3]));
		missileSiteSecundus->rotationMatrix = siteSecundus->rotationMatrix * glm::rotate(identity, PI / 2, glm::vec3(1, 0, 0));
		missileCountSecundus--;
		sprintf(secundusStr, "  Secundus %2d", missileCountSecundus);

	}
	//update secundus missile
	if (activemissileSecundus)
	{
		missileSiteSecundus->update(warbird->getModelMatrix(), sitemissileSpeed);
		warbirdCollision[6] = collision(modelMatrix[10], modelMatrix[5], modelSize[10] + 10.0f, modelSize[5] + 20.0f);  //Secundus missile hits warbird
		//missile Secundus collision with planets
		if (missileSiteSecundus->missileUpdate > 200)
		{
			siteSecundusMissileCollision[0] = collision(modelMatrix[10], ruber->getModelMatrix(), modelSize[10] + 10.0f, modelSize[0]);
			siteSecundusMissileCollision[1] = collision(modelMatrix[10], unum->getModelMatrix(), modelSize[10] + 10.0f, modelSize[1]);
			siteSecundusMissileCollision[2] = collision(modelMatrix[10], duo->getModelMatrix(), modelSize[10] + 10.0f, modelSize[2]);
			siteSecundusMissileCollision[3] = collision(modelMatrix[10], primus->Moon(duo->getModelMatrix(), primus->getModelMatrix()), modelSize[10] + 10.0f, modelSize[3]);
			siteSecundusMissileCollision[4] = collision(modelMatrix[10], secundus->Moon(duo->getModelMatrix(), secundus->getModelMatrix()), modelSize[10] + 10.0f, modelSize[4]);
		}
	}
	//handle missile destruction
	if (missileWarbird->destroymissile)
	{
		activemissileWarbird = false;
		missileWarbird->translationMatrix = glm::translate(identity, glm::vec3(0));
		missileWarbird->destroymissile = false;
	}
	if (missileSiteUnum->destroymissile)
	{
		activemissileUnum = false;
		missileSiteUnum->translationMatrix = glm::translate(identity, glm::vec3(0));
		missileSiteUnum->destroymissile = false;
		missileSiteUnum->missileUpdate = 0;
	}
	if (missileSiteSecundus->destroymissile)
	{
		activemissileSecundus = false;
		missileSiteSecundus->translationMatrix = glm::translate(identity, glm::vec3(0));
		missileSiteSecundus->destroymissile = false;
		missileSiteSecundus->missileUpdate = 0;
	}
	//handle missile site destruction
	if (siteUnumDestroyed)
	{
		siteUnum->translationMatrix = glm::translate(identity, glm::vec3(0));
	}
	if (siteSecundusDestroyed)
	{
		siteSecundus->translationMatrix = glm::translate(identity, glm::vec3(0));
	}

	//check for warbird collisions with planets
	warbirdCollision[0] = collision(warbird->getModelMatrix(), ruber->getModelMatrix(), modelSize[5] + 10.0f, modelSize[0]);
	warbirdCollision[1] = collision(warbird->getModelMatrix(), unum->getModelMatrix(), modelSize[5] + 10.0f, modelSize[1]);
	warbirdCollision[2] = collision(warbird->getModelMatrix(), duo->getModelMatrix(), modelSize[5] + 10.0f, modelSize[2]);
	warbirdCollision[3] = collision(warbird->getModelMatrix(), primus->Moon(duo->getModelMatrix(), primus->getModelMatrix()), modelSize[5] + 10.0f, modelSize[3]);
	warbirdCollision[4] = collision(warbird->getModelMatrix(), secundus->Moon(duo->getModelMatrix(), secundus->getModelMatrix()), modelSize[5] + 10.0f, modelSize[4]);
	
	for (int i = 0; i < nCollisions; i++)
	{
		if (warbirdCollision[i])
		{
			if (i == 5)
				missileSiteUnum->destroymissile = true;
			if (i == 6)
				missileSiteSecundus->destroymissile = true;
			else
				warbirdDestroyed = true;
			
		}
		if (warbirdMissileCollision[i] && (i == 0 || i == 1))
		{
			if (i == 0)
				siteUnumDestroyed = true;
			else
				siteSecundusDestroyed = true;
			//strcpy(titleStr, "Missile site destroyed!");
			missileWarbird->destroymissile = true;
		}
		if (siteUnumMissileCollision[i])
		{
			//strcpy(titleStr, "Unum missile destroyed!");
			missileSiteUnum->destroymissile = true;
		}
		if (siteSecundusMissileCollision[i])
		{
			//strcpy(titleStr, "Secundus missile  destroyed!");
			missileSiteSecundus->destroymissile = true;
		}
	}

	updateCount++;
	// see if a second has passed to set estimated fps information
	currentUpdateTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeIntervalUpdate = currentUpdateTime - lastUpdateTime;
	if (timeIntervalUpdate >= 1000) {
		sprintf(updateStr, "  U/S %4d", (int)(updateCount / (timeIntervalUpdate / 1000.0f)));
		lastUpdateTime = currentUpdateTime;
		updateCount = 0;
		updateTitle();
	}
	glutPostRedisplay();

}


// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer(int i) {
	glutTimerFunc(timerDelay[timer], intervalTimer, 1);
	/*bool collision = false;
	for (int i = 0; i < nCollisions; i++)
	{
		if (collide[i])
		{
			collision = true;
		}
	}
	if (!collision)*/
	//if (!warbirdDestroyed)
	update();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
		
	case 't': case 'T': //toggle update rate
		timer++;
		if (timer == nTimers)
			timer = 0;
			
		break;

	case 'v': case 'V': //toggle camera
		if (cycleForward)
		{
			if (toggleCam == nCameras)
				toggleCam = 0;
			viewMatrix = camera[toggleCam]->getViewMatrix();
			strcpy(viewStr, camera[toggleCam]->getCameraString());
			toggleCam++;
			cycleForward = true;
			cycleBackward = false;
		}
		else
		{
			toggleCam++;
			if (toggleCam == nCameras)
				toggleCam = 0;
			viewMatrix = camera[toggleCam]->getViewMatrix();
			strcpy(viewStr, camera[toggleCam]->getCameraString());
			toggleCam++;
			cycleForward = true;
			cycleBackward = false;
		}

		break;

	case 'x': case 'X': //toggle previous camera
		if (cycleBackward)
		{
			//printf("togglecam1: %i", toggleCam);
			--toggleCam;
			if (toggleCam < 0)
				toggleCam = nCameras-1;
			//printf("togglecam1: %i\n", toggleCam);
			viewMatrix = camera[toggleCam]->getViewMatrix();
			strcpy(viewStr, camera[toggleCam]->getCameraString());
			cycleBackward = true;
			cycleForward = false;
		}
		else
		{
			//printf("togglecam2: %i", toggleCam);
			toggleCam = toggleCam-2;
			if (toggleCam < 0)
				toggleCam = nCameras-1;
			//printf("togglecam2: %i\n", toggleCam);
			viewMatrix = camera[toggleCam]->getViewMatrix();
			strcpy(viewStr, camera[toggleCam]->getCameraString());
			cycleBackward = true;
			cycleForward = false;
		}

		break;

	case 's': case 'S':  // toggle ship speed

		if (warbird->stepDistance == 10)
		{
			warbird->stepDistance = 50;
		}
		else if (warbird->stepDistance == 50)
		{
			warbird->stepDistance = 200;
		}
		else
			warbird->stepDistance = 10;

		if (debug)
		{

			if (axes->stepDistance == 10)
			{
				axes->stepDistance = 50;
			}
			else if (axes->stepDistance == 50)
			{
				axes->stepDistance = 200;
			}
			else
				axes->stepDistance = 10;
		}

		break;

	case 'w': case 'W' : //warp the ship to Unum or Duo

		if (!atUnum) // warp to Unum
		{
			glm::vec3 zUnum = glm::vec3(unum->getModelMatrix()[0][2], unum->getModelMatrix()[0][1] * -1, unum->getModelMatrix()[0][0] * -1);
			zUnum = glm::normalize(zUnum);
			warbird->rotationMatrix = unum->rotationMatrix * glm::rotate(identity, PI, glm::vec3(0, 1, 0));
			warbird->translationMatrix = glm::translate(identity, glm::vec3(unum->getModelMatrix()[3]) + zUnum * 8000.0f);
			//float angle = warbird->angleBetween(glm::vec3(warbird->translationMatrix[2]), glm::vec3(unum->getModelMatrix()[2]), glm::vec3(0));
			////showMat4("umum: ", unum->getModelMatrix());
			//if (unum->getModelMatrix()[3][2] > 0)
			//{
			//	angle = 2 * PI - angle;
			//}
			////printf("angle %f", angle);
			//warbird->translationMatrix = warbird->translationMatrix * glm::rotate(identity, PI, glm::vec3(0, 1, 0));
			//showMat4("warbird", warbird->getModelMatrix());
			//showMat4("unum", unum->getModelMatrix());
			atUnum = true;
		}
		else //warp to Duo
		{
			glm::vec3 zDuo = glm::vec3(duo->getModelMatrix()[0][2], duo->getModelMatrix()[0][1] * -1, duo->getModelMatrix()[0][0] * -1);
			zDuo = glm::normalize(zDuo);
			warbird->rotationMatrix = duo->rotationMatrix * glm::rotate(identity, PI, glm::vec3(0, 1, 0));;
			warbird->translationMatrix = glm::translate(identity, glm::vec3(duo->getModelMatrix()[3]) + zDuo * 8000.0f);
			/*warbird->translationMatrix = warbird->getDirectionMatrix(glm::vec3(warbird->translationMatrix[3]), glm::vec3(duo->getModelMatrix()[3]));
			warbird->translationMatrix = warbird->translationMatrix * glm::rotate(identity, PI, glm::vec3(0, 1, 0));*/
			/*float angle = warbird->angleBetween(glm::vec3(warbird->translationMatrix[2]), glm::vec3(duo->getModelMatrix()[2]), glm::vec3(0));
			if (duo->getModelMatrix()[3][2] > 0)
			{
				angle = 2 * PI - angle;
			}
			warbird->translationMatrix = warbird->translationMatrix * glm::rotate(identity, angle + PI, glm::vec3(0, 1, 0));*/
			atUnum = false;
		}

		break;

		//fires the ships missiles
		//work in progress...
	case 'f': case 'F':
	
		if ((activemissileWarbird == false && missileCountWarbird != 0) && !(siteUnumDestroyed && siteSecundusDestroyed))
		{
			activemissileWarbird = true;
			missileWarbird->translationMatrix = warbird->translationMatrix;
			missileWarbird->rotationMatrix = warbird->rotationMatrix;
			missileWarbird->destroymissile = false;
			missileWarbird->missileUpdate = 0;
			missileCountWarbird--;
			sprintf(warbirdStr, "  Warbird %2d", missileCountWarbird);
		}
		
		break;

	case 'd': case 'D':  // debug case

		if (!debug)
		{
			debug = true;
		}
		else
		{
			debug = false;
		}

		break;

	case 'g': case 'G':  // toggle gravity, affects warbird

		if (!gravity)
		{
			gravity = true;
		}
		else
		{
			gravity = false;
		}

		break;
	}
	updateTitle();
}


void specialKeyEvent(int key, int x, int y) {

	if (key == GLUT_KEY_UP && glutGetModifiers() != GLUT_ACTIVE_CTRL) 
		warbird->setMove(1);
	else if (key == GLUT_KEY_DOWN && glutGetModifiers() != GLUT_ACTIVE_CTRL)
		warbird->setMove(-1);
	else if (key == GLUT_KEY_LEFT && glutGetModifiers() != GLUT_ACTIVE_CTRL)
		warbird->setYaw(1);
	else if (key == GLUT_KEY_RIGHT && glutGetModifiers() != GLUT_ACTIVE_CTRL)
		warbird->setYaw(-1);
	else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL)
		warbird->setPitch(-1);
	else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL)
		warbird->setPitch(1);
	else if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
		warbird->setRoll(-1);
	else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
		warbird->setRoll(1);
	else
		;
	if (debug)
	{
		if (key == GLUT_KEY_UP && glutGetModifiers() != GLUT_ACTIVE_CTRL)
			axes->setMove(1);
		else if (key == GLUT_KEY_DOWN && glutGetModifiers() != GLUT_ACTIVE_CTRL)
			axes->setMove(-1);
		else if (key == GLUT_KEY_LEFT && glutGetModifiers() != GLUT_ACTIVE_CTRL)
			axes->setYaw(1);
		else if (key == GLUT_KEY_RIGHT && glutGetModifiers() != GLUT_ACTIVE_CTRL)
			axes->setYaw(-1);
		else if (key == GLUT_KEY_UP && glutGetModifiers() == GLUT_ACTIVE_CTRL)
			axes->setPitch(-1);
		else if (key == GLUT_KEY_DOWN && glutGetModifiers() == GLUT_ACTIVE_CTRL)
			axes->setPitch(1);
		else if (key == GLUT_KEY_LEFT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
			axes->setRoll(-1);
		else if (key == GLUT_KEY_RIGHT && glutGetModifiers() == GLUT_ACTIVE_CTRL)
			axes->setRoll(1);
		else
			;
	}
}

// load the shader programs, vertex data from model files, create the solids, set initial view
void init() {
	// load the shader programs
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	/*char * modelFile[nModels] = { "Ruber.tri", "Unum.tri ", "Duo.tri", "Primus.tri",
		"Segundus.tri", "BattleCruiser.tri", "missile.tri" };
	float modelBR[nModels];       // model's bounding radius
	float scaleValue[nModels];    // model's scaling "size" value
	const int nVertices[nModels] = { 264 * 3, 264 * 3, 278 * 3, 264 * 3, 264 * 3, 2772 * 3, 644 * 3 };
	float modelSize[nModels] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 100.0f, 25.0f };   // size of model
glm::vec3 scale[nModels];       // set in init()
glm::vec3 translate[nModels] = { glm::vec3(0, 0, 0), glm::vec3(4000, 0, 0), glm::vec3(9000, 0, 0),
glm::vec3(900, 0, 0), glm::vec3(1750, 0, 0),
glm::vec3(5000, 1000, 5000), glm::vec3(4900, 1000, 4850)


};*/

	// generate VAOs and VBOs
	glGenVertexArrays(nModelsLoaded, VAO);
	glGenBuffers(nModelsLoaded, buffer);
	// load the buffers from the model files
	for (int i = 0; i < nModelsLoaded; i++) {
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] * 1.0f / modelBR[i]);
	}


	for (int i = 0; i < nModelsLoaded; i++) {
		printf("model size: %f, %f\n", modelBR[i], scale[i]);
	}

	ruber = new Planet(glm::vec3(0, 0, 0), scale[0], 264 * 3, 0.0f, 2000.0f, "Ruber.tri");
	unum = new Planet(glm::vec3(4000, 0, 0), scale[1], 264 * 3, 0.004f, 200.0f, "Unum.tri");
	duo = new Planet(glm::vec3(9000, 0, 0), scale[2], 278 * 3, 0.002f, 400.0f, "Duo.tri");
	duo->DuoRotate180();
	primus = new Planet(glm::vec3(900, 0, 0), scale[3], 264 * 3, 0.004f, 100.0f, "Primus.tri");
	secundus = new Planet(glm::vec3(1750, 0, 0), scale[4], 264 * 3, 0.002f, 150.0f, "Secundus.tri");
	warbird = new Warbird(glm::vec3(5000, 1000, 5000), scale[5], 2772 * 3, 100.0f, "Warbird.tri");
	axes = new Warbird(glm::vec3(5000, 1000, 5000), scale[7], 120 * 3, 100.0f, "axes-r100.tri");
	missileWarbird = new missile(glm::vec3(0, 0, 0), scale[6], 644 * 3, 0.0f, 25.0f, "missile.tri");
	missileSiteUnum = new missile(glm::vec3(0, 0, 0), scale[6], 644 * 3, 0.0f, 25.0f, "missile.tri");
	missileSiteSecundus = new missile(glm::vec3(0, 0, 0), scale[6], 644 * 3, 0.0f, 25.0f, "missile.tri");
	siteUnum = new Planet(glm::vec3(0, 200, 0), scale[8], 1382 * 3, 0.004f, 30.0f, "missileBase.tri");
	siteSecundus = new Planet(glm::vec3(0, 150, 0), scale[8], 1382 * 3, 0.002f, 30.0f, "missileBase.tri");

	plane = new Planet(glm::vec3(4000, 0, 0), scale[9], 2 * 3, 0.000f, 2000.0f, "plane.tri");

	//missileWarbird->setmissileScale(glm::vec3(20.0));
	missileSiteUnum->setmissileScale(glm::vec3(20.0));
	missileSiteSecundus->setmissileScale(glm::vec3(20.0));


	//Set the positonal lighting

	GLint light_Position_location = glGetUniformLocation(shaderProgram, "Light_Position");
	glm::vec3 light_position = glm::vec3(0.0f, 5000.0f, 0.0f);
	glUniform3f(light_Position_location, light_position.x, light_position.y, light_position.z);

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
	model_location = glGetUniformLocation(shaderProgram, "Model_Location");

	//// initially use a front view
	//eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // camera's position
	//at = glm::vec3(0);						   // position camera is looking at
	//up = glm::vec3(0.0f, 1.0f, 0.0f);            // camera'a up vector
	
	showTexture = glGetUniformLocation(shaderProgram, "IsTexture");
	/*glBufferData(GL_ARRAY_BUFFER, sizeof(nVertices[9]) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(nVertices[9]), point);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(nVertices[9]), sizeof(texCoords), texCoords);*/
	// glBufferSubData( GL_ARRAY_BUFFER, sizeof(point) + sizeof(texCoords), sizeof(normal), normal );

	vTexCoord = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(nVertices[9])));
	glEnableVertexAttribArray(vTexCoord);
	// load texture
	texture = loadRawTexture(texture, fileName, width, height);
	if (texture != 0) {
		printf("texture file, read, texture generated and bound  \n");
		//  Texture = glGetUniformLocation(shaderProgram, "Texture"); 
	}
	else  // texture file loaded
		printf("Texture in file %s NOT LOADED !!! \n");

	camera[0] = new Camera(glm::vec3(0.0f, 10000.0f, 20000.0f), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f), 0, " View Front");
	camera[1] = new Camera(glm::vec3(0.0f, 20000.0f, 0.0f), glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), 0, " View Top");
	camera[2] = new Camera(glm::vec3(0), glm::vec3(0), glm::vec3(0), 0, " View Warbird");
	camera[3] = new Camera(glm::vec3(0), glm::vec3(0), glm::vec3(0), 0, " View Unum");
	camera[4] = new Camera(glm::vec3(0), glm::vec3(0), glm::vec3(0), 0, " View Duo");

	viewMatrix = camera[0]->getViewMatrix();
	showMat4("front view: ", viewMatrix);
	toggleCam++;

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 0.7f);

	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("465 Project 1");
	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	// initialize scene
	init();
	// set glut callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyEvent);
	glutIdleFunc(display);  
	glutTimerFunc(timerDelay[timer], intervalTimer, 1);
	glutMainLoop();
	printf("done\n");
	return 0;
}


