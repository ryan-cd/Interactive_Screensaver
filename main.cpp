/*
RYAN DAVIS
INTERACTIVE SCREENSAVER

Bonus features: 
Bonus 2: Sticky vertex list: switching between modes will make the previous shapes compose the new shape
Push 4 to access a bonus primitive type
Shapes will change colors and alpha values after they are spawned

Notes:
The word "point" and "vertex" is used interchangeably.
The LINE_LOOP primitive is considered a type of polygon
*/


#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glut.h>
#include <GL/gl.h>
//#  include <GL/gl.h>
//#  include <GL/glu.h>
//#  include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include "Vertex.h"

using namespace std;



//variable declarations
int screenWidth = 1280;
int screenHeight = 720;

/*
state machine explained:

state 1: PLACE_VERTEX: This is when you click to specify a point origin
state 2: SET_DIRECTION: This is when you click to specify a point direction.
	finishing this state will either take you to MID_SHAPE or END_SHAPE depending
	on whether the shape is finished
state 3 (optional): MID SHAPE: This is when a shape has started, but it can 
	continue to have more vertexes in
	it. It is not used for points. Completing this state takes you to PLACE_VERTEX
state 4: END_SHAPE: This is when a shape is done and should be drawn
*/

enum CLICK_STATE { PLACE_VERTEX, MID_SHAPE, SET_DIRECTION, END_SHAPE }; //State machine for the mouse click state 
enum SHAPE_MODE { S_POINTS, S_LINES, S_POLYGONS, S_LINE_LOOPS }; //Which shape should be drawn
CLICK_STATE clickState = PLACE_VERTEX; 
SHAPE_MODE shapeMode = S_POINTS; 

bool pause = false;
bool randomize = false; //this bool says if random shapes should be added
float speed = 1; //this is multiplied by the point's direction vector
float size = 3; //size of the lines and points

vector<Vertex> vertexVector; //vector to contain the Vertex objects
vector<int> polygonEndIndex; //this vector keeps track of what indexes in the vertex vector are where a polygon ends
//default params for vertex
vector<float> clickPosition(2); //the coordinates where the point originates
vector<float> moveToPosition(2); //the coordinates where the point should travel to
vector<float> color(4); //the color of the vertex to be added and alpha

//this is a temporary vertex. For every point it gets overwritten and aded to the vertexVector
Vertex newVertex(clickPosition, moveToPosition, color, speed, size); 

//Used to indicate if a shape is finished. 
//currently only polygons and line_loops use this. It shows when a new one should start
bool shapeDone = false; 


//randomizes the color global
void randomizeColor(void)
{
	color[0] = (float)rand() / RAND_MAX;
	color[1] = (float)rand() / RAND_MAX;
	color[2] = (float)rand() / RAND_MAX;
	color[3] = (float)(rand() / 2) / (float)RAND_MAX / 2 + 0.5;
}

//adds a vertex to the vertexVector. The settings would have been set before this point
void addVertex(void)
{
	//set the properties to the globals
	newVertex.setPosition(clickPosition);
	newVertex.setMoveToVertex(moveToPosition);
	newVertex.setIsMoving(true);
	newVertex.setColor(color[0], color[1], color[2], color[3]);
	//add the vertex to the vector containg them
	vertexVector.push_back(newVertex);
}

//called when the screen resizes
void reshape(int w, int h)
{
	//resize the screen size globals
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	
	glMatrixMode(GL_MODELVIEW);
}

//handles drawing
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT); //set background to black
	
	//update the sizes of the points and lines
	glPointSize(size);
	glLineWidth(size);

	//set the glBegin to the proper type
	if (shapeMode == S_POINTS)
		glBegin(GL_POINTS);
	else if (shapeMode == S_LINES) 
		glBegin(GL_LINES);
	else if (shapeMode == S_POLYGONS)
		glBegin(GL_POLYGON);
	else if (shapeMode == S_LINE_LOOPS)
		glBegin(GL_LINE_LOOP);
	
		
	
	for (int i = 0; i < vertexVector.size(); i++)
	{
		//set the glColor to be the color of the vertex object
		glColor4f(vertexVector[i].getColorR(), vertexVector[i].getColorG(), vertexVector[i].getColorB(), vertexVector[i].getColorA());
		//set the glVertex to be at the position of the vertex object
		glVertex2f(vertexVector[i].getPositionX(), vertexVector[i].getPositionY());
		
		//if the mode is polygons or line loops, multiple glBegin() and glEnd() lines are needed.
		//if the current iteration is within the polygonEndIndex vector, then we need a new glBegin() and glEnd()
		
		if ((shapeMode == S_POLYGONS || shapeMode == S_LINE_LOOPS) 
			&& binary_search(polygonEndIndex.begin(), polygonEndIndex.end(), i))
		{
			glEnd();
			if (shapeMode == S_POLYGONS)
				glBegin(GL_POLYGON);
			else if (shapeMode == S_LINE_LOOPS)
				glBegin(GL_LINE_LOOP);
		}
	}
	glEnd();


	glutSwapBuffers(); //swap the buffers
}

//handles mouse events
void mouseHandler(int button, int state, int x, int y)
{
	//if you clicked
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (shapeMode == S_POINTS)
		{
			shapeDone = true; // the shape is done after the first point is placed
			//this functionality is not currently used. In future an action could be
			//performed when a point is completed (eg play a sound).
		}

		//used to indicate when a line is finished
		if (clickState == MID_SHAPE)
		{
			if (shapeMode == S_LINES)
				shapeDone = true;
			clickState = PLACE_VERTEX;
		}
		//next click will specify the point's direction
		if (clickState == SET_DIRECTION)
		{
			//set the color global to random
			randomizeColor();

			//set the point to move to
			moveToPosition[0] = x;
			moveToPosition[1] = screenHeight - y;
			//if the user clicks the same location twice, add an offset
			if (moveToPosition[0] == clickPosition[0])
				moveToPosition[0] += 1;
			if (moveToPosition[1] == clickPosition[1])
				moveToPosition[1] += 1;

			//if the shape finished, go to the END_SHAPE state. Otherwise go to MID_SHAPE
			if (shapeDone)
				clickState = END_SHAPE;
			else 
				clickState = MID_SHAPE;
		}
		//state to click to make a point origin 
		if (clickState == PLACE_VERTEX)
		{
			//set the origin point and change state
			clickPosition[0] = x;
			clickPosition[1] = screenHeight - y;
			clickState = SET_DIRECTION;
		}
	}
}

//handle key presses
void keyboardHandler(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(0); 
		break;
	case 'p':
		pause = pause ? false : true;
		break;
	case 'r':
		vertexVector.clear();
		polygonEndIndex.clear();
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'a':
		randomize = true;
		break;
	case '-':
	case '_':
		if (speed > 0.21)
			speed = speed - 0.2;
		break;
	case '=':
	case '+':
		if (speed < 10)
			speed = speed + 0.2;
		break;
	case '[':
		if (size > 1)
			size -= 1;
		break;
	case ']':
		size += 1;
		break;
	case ' ':
		if (shapeMode == S_POLYGONS || shapeMode == S_LINE_LOOPS)
			shapeDone = true;
		break;
	case '1':
		shapeMode = S_POINTS;
		break;
	case '2':
		if (size > 5)
			size = 5;
		shapeMode = S_LINES;
		break;
	case '3':
		shapeMode = S_POLYGONS;
		break;
	case '4':
		if (size > 5)
			size = 5;
		shapeMode = S_LINE_LOOPS;
		break;
	}
}

void idle(void)
{
	//if this bool is true, add a random number of random shapes of random colors and alphas
	if (randomize)
	{		
		for (int i = 0; i < rand() % 50 + 25; i++)
		{
			randomizeColor();

			moveToPosition[0] = rand() % screenWidth;
			moveToPosition[1] = rand() % screenHeight;
			clickPosition[0] = rand() % screenWidth;
			clickPosition[1] = rand() % screenHeight;

			//random chance to split into multiple polygon objects
			if (i % 4 == 0 && i > 0)
			{
				if (rand() % 10 > 5)
					polygonEndIndex.push_back(i);
			}
			addVertex();
		}
		randomize = false;
	}
	//user has indicated that they want the polygon to end.
	if ((shapeMode == S_POLYGONS || shapeMode == S_LINE_LOOPS) && shapeDone)
	{
		//add the last index of the vertexVector to the polygonEnd index. This way
		//it will be tracked and a glBegin and glEnd can be wrapped around the vertices
		if (polygonEndIndex.size() == 0 && vertexVector.size() >= 3
			|| (polygonEndIndex.size() > 0 && polygonEndIndex[polygonEndIndex.size() - 1] <= vertexVector.size() - 4))
		{
			polygonEndIndex.push_back((int)vertexVector.size() - 1);
			shapeDone = false;
		}
	}

	//mouse counsequences
	if (clickState == END_SHAPE || clickState == MID_SHAPE)
	{
		//the globals for the vertex options will be set before this function is called
		addVertex();
		
		shapeDone = false;
		clickState = PLACE_VERTEX;
	}

	//updates for the points every time function is called
	for (int i = 0; i < vertexVector.size(); i++)
	{
		//if the vertex is moving and the program isn't paused
		if (vertexVector[i].getIsMoving() && !pause)
		{
			//bounce off side walls
			if (vertexVector[i].getPositionX() + vertexVector[i].getDirectionVectorX() > screenWidth
				|| vertexVector[i].getPositionX() + vertexVector[i].getDirectionVectorX() < 0)
			{
				vertexVector[i].invertDirectionX();
			}
			//bounce off upper and lower walls
			if (vertexVector[i].getPositionY() + vertexVector[i].getDirectionVectorY() > screenHeight
				|| vertexVector[i].getPositionY() + vertexVector[i].getDirectionVectorY() < 0)
			{
				vertexVector[i].invertDirectionY();
			}
			
			//set the speed to the speed global
			vertexVector[i].setSpeed(speed);

			//update the position
			vertexVector[i].setPosition(vertexVector[i].getPositionX() + vertexVector[i].getDirectionVectorX() * speed
				, vertexVector[i].getPositionY() + vertexVector[i].getDirectionVectorY() * speed);
		
			//random chance to update the color and alpha
			if (rand() % 10000 > 9995)
			{
				randomizeColor();
				vertexVector[i].setColor(color[0], color[1], color[2], color[3]);
			}	
		}
	}
	
	glutPostRedisplay(); //goto display function
}


//program entry function
int main(int argc, char** argv)
{
	fprintf(stderr, "Welcome to Interactive Screensaver\n\n");
	fprintf(stderr, "Controls: \n"
		"q: quit\n"
		"p: pause\n"
		"r: reset\n"
		"f: fullscreen\n"
		"a: rAndomize\n"
		"-: slow down\n"
		"+: speed up\n"
		"[: size down\n"
		"]: size up\n"
		"<space>: end polygon\n"
		"1: points mode\n"
		"2: line mode\n"
		"3: filled polygon mode\n"
		"4: unfilled polygon mode\n\n");
	fprintf(stderr, "Notes: \n"
		"1) You need at least 3 points \nbefore you can end a polygon\n"
		"2) Click once to set a vertex \nposition. Click again to set \nthe direction you want it to travel");

	srand(time(NULL)); //set the randome number seed

	glutInit(&argc, argv);	//begin glut

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //using RGB and double buffering
	glutInitWindowSize(screenWidth, screenHeight); //set the window sizing
	glutInitWindowPosition(600, 200); //window spawn coordinates
	glutCreateWindow("\n\nInteractive Screensaver\n");	
	glEnable(GL_BLEND); //needed for alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //needed for alpha
	
	//callback functions
	glutDisplayFunc(display);	
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseHandler);
	glutKeyboardFunc(keyboardHandler);
	glutIdleFunc(idle);
	
	glutMainLoop();		
	
	return(0);	
}



