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
# include "object3D.hpp"
# include "planet.hpp"
# include "warbird.hpp"
# include "missle.hpp"
# include "camera.hpp"


const int X = 0, Y = 1, Z = 2, W = 3, START = 0, STOP = 1;
// constants for models:  file names, vertex count, model display size
const int nModels = 8;  // number of models in this scene
const int nCameras = 5;
char * modelFile[nModels] = { "Ruber.tri", "Unum.tri ", "Duo.tri", "Primus.tri",
"Segundus.tri", "BattleCruiser.tri", "Missle.tri", "axes-r100.tri" };
float modelBR[nModels];       // model's bounding radius
float scaleValue[nModels];    // model's scaling "size" value
const int nVertices[nModels] = { 264 * 3, 264 * 3, 278 * 3, 264 * 3, 264 * 3, 2772 * 3, 644 * 3, 120 * 3 };
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint shaderProgram;
GLuint VAO[nModels];      // Vertex Array Objects
GLuint buffer[nModels];   // Vertex Buffer Objects

//// vectors and values for lookAt
//glm::vec3 eye, at, up;

Camera * camera[nCameras];
int toggleCam = 0;

//camera view booleans
bool cycleForward = true;
bool cycleBackward = false;

bool atUnum = false;

//debug boolean
bool debug = false;

// window title strings
char baseStr[50] = "465 Project 1 : ";
char viewStr[15] = "  View Front";
char warbirdStr[15] = "  Warbird ??";
char unumStr[15] = "  Unum ?";
char secundusStr[15] = "  Secundus ?";
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
GLuint MVP;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
// model, view, projection matrices and values to create modelMatrix.
//loaded in order of Ruber, Umun, Duo, Primus, Secundus, Warbird, missiles
float modelSize[nModels] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 500.0f, 25.0f, 500.0f };   // size of model
glm::vec3 scale[nModels];       // set in init()
glm::vec3 translate[nModels] = { glm::vec3(0, 0, 0), glm::vec3(4000, 0, 0), glm::vec3(9000, 0, 0),
glm::vec3(900, 0, 0), glm::vec3(1750, 0, 0),
glm::vec3(5000, 1000, 5000), glm::vec3(4900, 1000, 4850), glm::vec3(5000, 1000, 5000)


};

//Unum and Duo matrices
glm::mat4 DuoMatrix;
glm::vec3 DuoTranslate;
/*glm::mat4 axesMatrix;		//for debugging
glm::vec3 axesTranslate;
glm::mat4 axesRotation = glm::rotate(identity, PI, glm::vec3(0, 1, 0));*/
glm::mat4 DuoRotation = glm::rotate(identity, PI, glm::vec3(0, 1, 0)); //used to rotate Duo 180 degrees CCW, since Duo is initial placed at +9000 on X
glm::mat4 UnumMatrix;
glm::vec3 UnumTranslate;

Planet * ruber;
Planet * unum;
Planet * duo;
Planet * primus;
Planet * secundus;
Warbird * warbird;
Warbird * axes;
Missle * missle;

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
	glutSetWindowTitle(titleStr);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// update model matrix
	
	missle->createModelMatrix();

	modelMatrix[0] = ruber->getModelMatrix();
	modelMatrix[1] = unum->getModelMatrix();
	modelMatrix[2] = duo->getModelMatrix();
	modelMatrix[3] = primus->Moon(duo->getModelMatrix(), primus->getModelMatrix());
	modelMatrix[4] = secundus->Moon(duo->getModelMatrix(), secundus->getModelMatrix());
	modelMatrix[5] = warbird->getModelMatrix();
	modelMatrix[6] = missle->getMissleMatrix();

	//showMat4("duo", modelMatrix[2]);
	if (debug)
	{
		modelMatrix[7] = warbird->getModelMatrix();
		for (int m = 0; m < nModels; m++) {
			//dynamic cameras
			if (cycleForward && toggleCam == 3 || cycleBackward && toggleCam == 2)
			{
				//(glm::vec3(5000.0f, 1300.0f, 6000.0f), glm::vec3(5000.0f, 1000.0f, 5000.0f), glm::vec3(0.0f, 1.0f, 0.0f)
				glm::vec3 zWarbird = glm::vec3(modelMatrix[5][0][2] * -1, modelMatrix[5][0][1], modelMatrix[5][0][0]);
				zWarbird = glm::normalize(zWarbird);
				glm::vec3 upWarbird = glm::vec3(modelMatrix[5][1]);
				upWarbird = glm::normalize(upWarbird);
				camera[2]->eye = glm::vec3(modelMatrix[5][3]) + upWarbird * 300.0f + zWarbird * 1000.0f;
				camera[2]->at = glm::vec3(modelMatrix[5][3]);
				camera[2]->up = glm::vec3(0.0f, 1.0f, 0.0f);
				viewMatrix = camera[2]->getViewMatrix();
			}
			//Unum camera
			if (cycleForward && toggleCam == 4 || cycleBackward && toggleCam == 3)
			{
				//90 degrees CW about y-axis: (x, y, z) -> (-z, y, x) -- this is how to get z-axis from x-axis --
				glm::vec3 zUnum = glm::vec3(unum->getModelMatrix()[0][2], unum->getModelMatrix()[0][1] * -1, unum->getModelMatrix()[0][0] * -1);
				zUnum = glm::normalize(zUnum);
				camera[3]->eye = glm::vec3(unum->getModelMatrix()[3]) + zUnum * 4000.0f;        // camera is 4000 units out along Unum's -z axis        
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
				camera[4]->eye = glm::vec3(duo->getModelMatrix()[3]) + zDuo * 4000.0f;				// camera is 4000 units out along Duo's -z axis
				camera[4]->at = glm::vec3(duo->getModelMatrix()[3]);								// camera is looking at Duo
				camera[4]->up = glm::vec3(0.0f, 1.0f, 0.0f);                 // camera's up is Y
				viewMatrix = camera[4]->getViewMatrix();
			}

			// glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr( modelMatrix)); 
			ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix[m];
			glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
			glBindVertexArray(VAO[m]);
			/*  The following 3 lines are not needed !
			glEnableVertexAttribArray( vPosition[m] );
			glEnableVertexAttribArray( vColor[m] );
			glEnableVertexAttribArray( vNormal[m] );
			*/
			glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);
		}
	}
	else
	{
		for (int m = 0; m < nModels-1; m++) {
			//dynamic cameras
			if (cycleForward && toggleCam == 3 || cycleBackward && toggleCam == 2)
			{
				//(glm::vec3(5000.0f, 1300.0f, 6000.0f), glm::vec3(5000.0f, 1000.0f, 5000.0f), glm::vec3(0.0f, 1.0f, 0.0f)
				glm::vec3 zWarbird = glm::vec3(modelMatrix[5][0][2] * -1, modelMatrix[5][0][1], modelMatrix[5][0][0]);
				zWarbird = glm::normalize(zWarbird);
				glm::vec3 upWarbird = glm::vec3(modelMatrix[5][1]);
				upWarbird = glm::normalize(upWarbird);
				camera[2]->eye = glm::vec3(modelMatrix[5][3]) + upWarbird * 300.0f + zWarbird * 1000.0f;
				camera[2]->at = glm::vec3(modelMatrix[5][3]);
				camera[2]->up = glm::vec3(0.0f, 1.0f, 0.0f);
				viewMatrix = camera[2]->getViewMatrix();
			}
			//Unum camera
			if (cycleForward && toggleCam == 4 || cycleBackward && toggleCam == 3)
			{
				//90 degrees CW about y-axis: (x, y, z) -> (-z, y, x) -- this is how to get z-axis from x-axis --
				glm::vec3 zUnum = glm::vec3(unum->getModelMatrix()[0][2], unum->getModelMatrix()[0][1] * -1, unum->getModelMatrix()[0][0] * -1);
				zUnum = glm::normalize(zUnum);
				camera[3]->eye = glm::vec3(unum->getModelMatrix()[3]) + zUnum * 4000.0f;        // camera is 4000 units out along Unum's -z axis        
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
				camera[4]->eye = glm::vec3(duo->getModelMatrix()[3]) + zDuo * 4000.0f;				// camera is 4000 units out along Duo's -z axis
				camera[4]->at = glm::vec3(duo->getModelMatrix()[3]);								// camera is looking at Duo
				camera[4]->up = glm::vec3(0.0f, 1.0f, 0.0f);                 // camera's up is Y
				viewMatrix = camera[4]->getViewMatrix();
			}

			// glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr( modelMatrix)); 
			ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix[m];
			glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
			glBindVertexArray(VAO[m]);
			/*  The following 3 lines are not needed !
			glEnableVertexAttribArray( vPosition[m] );
			glEnableVertexAttribArray( vColor[m] );
			glEnableVertexAttribArray( vNormal[m] );
			*/
			glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);
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
	/*//rotation speed for Unum and Primus
	rotateRadianOne += 0.004f;
	if (rotateRadianOne >  2 * PI) rotateRadianOne = 0.0f;
	rotationOne = glm::rotate(identity, rotateRadianOne, glm::vec3(0, 1, 0));
	//rotation speed for Duo and Secundus
	rotateRadianTwo += 0.002f;
	if (rotateRadianTwo >  2 * PI) rotateRadianTwo = 0.0f;
	rotationTwo = glm::rotate(identity, rotateRadianTwo, glm::vec3(0, 1, 0));*/
	unum->update();
	duo->update();
	primus->update();
	secundus->update();
	warbird->update();
	axes->update();
	//if peron presses f then the fire missle function is called
	missle->update();
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
	update();  // fixed interval timer
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
			warbird->rotationMatrix = identity;
			warbird->translationMatrix = glm::translate(glm::mat4(), glm::vec3(0));
			warbird->translationMatrix = warbird->translationMatrix * glm::translate(identity, glm::vec3(unum->getModelMatrix()[3]) + zUnum * 4000.0f);
			float angle = warbird->angleBetween(glm::vec3(warbird->translationMatrix[2]), glm::vec3(unum->getModelMatrix()[2]), glm::vec3(0));
			//showMat4("umum: ", unum->getModelMatrix());
			if (unum->getModelMatrix()[3][2] > 0)
			{
				angle = 2 * PI - angle;
			}
			//printf("angle %f", angle);
			warbird->translationMatrix = warbird->translationMatrix * glm::rotate(identity, angle + PI, glm::vec3(0, 1, 0));
			atUnum = true;
		}
		else //warp to Duo
		{
			glm::vec3 zDuo = glm::vec3(duo->getModelMatrix()[0][2], duo->getModelMatrix()[0][1] * -1, duo->getModelMatrix()[0][0] * -1);
			zDuo = glm::normalize(zDuo);
			warbird->rotationMatrix = identity;
			warbird->translationMatrix = glm::translate(identity, glm::vec3(duo->getModelMatrix()[3]) + zDuo * 4000.0f);
			/*warbird->translationMatrix = warbird->getDirectionMatrix(glm::vec3(warbird->translationMatrix[3]), glm::vec3(duo->getModelMatrix()[3]));
			warbird->translationMatrix = warbird->translationMatrix * glm::rotate(identity, PI, glm::vec3(0, 1, 0));*/
			float angle = warbird->angleBetween(glm::vec3(warbird->translationMatrix[2]), glm::vec3(duo->getModelMatrix()[2]), glm::vec3(0));
			if (duo->getModelMatrix()[3][2] > 0)
			{
				angle = 2 * PI - angle;
			}
			warbird->translationMatrix = warbird->translationMatrix * glm::rotate(identity, angle + PI, glm::vec3(0, 1, 0));
			atUnum = false;
		}

		break;

		//fires the ships missles
		//work in progress...
	case 'f': case 'F':
	
		//missle->update();
		
		break;

	case 'd': case 'D':  // debug case, not fully implemented yet

		if (!debug)
		{
			debug = true;
		}
		else
		{
			debug = false;
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
		"Segundus.tri", "BattleCruiser.tri", "Missle.tri" };
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
	glGenVertexArrays(nModels, VAO);
	glGenBuffers(nModels, buffer);
	// load the buffers from the model files
	for (int i = 0; i < nModels; i++) {
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] * 1.0f / modelBR[i]);
	}

	ruber = new Planet(glm::vec3(0, 0, 0), scale[0], 264 * 3, 0.0f, 2000.0f, "Ruber.tri");
	unum = new Planet(glm::vec3(4000, 0, 0), scale[1], 264 * 3, 0.004f, 200.0f, "Unum.tri");
	duo = new Planet(glm::vec3(9000, 0, 0), scale[2], 278 * 3, 0.002f, 400.0f, "Duo.tri");
	duo->DuoRotate180();
	primus = new Planet(glm::vec3(900, 0, 0), scale[3], 264 * 3, 0.004f, 100.0f, "Primus.tri");
	secundus = new Planet(glm::vec3(1750, 0, 0), scale[4], 264 * 3, 0.002f, 150.0f, "Secundus.tri");
	warbird = new Warbird(glm::vec3(5000, 1000, 5000), scale[5], 2772 * 3, 100.0f, "Warbird.tri");
	axes = new Warbird(glm::vec3(5000, 1000, 5000), scale[7], 120 * 3, 100.0f, "axes-r100.tri");
	missle = new Missle(glm::vec3(4900, 1000, 4850), scale[6], 644 * 3, 25.0f, "Missle.tri");

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

	//// initially use a front view
	//eye = glm::vec3(0.0f, 10000.0f, 20000.0f);   // camera's position
	//at = glm::vec3(0);						   // position camera is looking at
	//up = glm::vec3(0.0f, 1.0f, 0.0f);            // camera'a up vector

	camera[0] = new Camera(glm::vec3(0.0f, 10000.0f, 20000.0f), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f), 0, " View Front");
	camera[1] = new Camera(glm::vec3(0.0f, 20000.0f, 0.0f), glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f), 0, " View Top");
	camera[2] = new Camera(glm::vec3(0), glm::vec3(0), glm::vec3(0), 0, " View Warbird");
	camera[3] = new Camera(glm::vec3(0), glm::vec3(0), glm::vec3(0), 0, " View Unum");
	camera[4] = new Camera(glm::vec3(0), glm::vec3(0), glm::vec3(0), 0, " View Duo");

	viewMatrix = camera[0]->getViewMatrix();
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


