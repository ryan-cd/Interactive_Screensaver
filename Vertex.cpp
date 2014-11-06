#include <math.h>
#include "Vertex.h"


Vertex::Vertex(vector<float> position, vector<float> moveToVertex, vector<float> color, float speed, float size)
{
	//TODO: Add integrity checks
	this->position = position;
	this->moveToVertex = moveToVertex;

	//integrity check for setting the color
	if (color[0] >= 0 && color[1] >= 0 && color[2] >= 0 && color[3] >= 0
		&& color[1] <= 1 && color[1] <= 1 && color[2] <= 1 && color[3] >= 0)
	{
		this->color = color;
	}
	else
	{
		this->color[0] = 1;
		this->color[1] = 1;
		this->color[2] = 1;
		this->color[3] = 1;
	}
	//integrity check for setting the color
	if (speed >= 0)
		this->speed = speed;
	else
		this->speed = 1;

	if (size >= 1)
		this->size = size;
	else
		this->size = 1;

	for (int i = 0; i < 2; i++)
		this->directionVector.push_back(0);
}


//getters
void Vertex::calculateDirectionVector(void)
{
	//This sets the direction vector to be the unit vector to the desired point
	this->directionVector[0] = (this->moveToVertex[0] - this->position[0]) 
		/ sqrt(pow((this->moveToVertex[0] - this->position[0]), 2) + pow((this->moveToVertex[1] - this->position[1]), 2));
	this->directionVector[1] = (this->moveToVertex[1] - this->position[1]) 
		/ sqrt(pow((this->moveToVertex[0] - this->position[0]), 2) + pow((this->moveToVertex[1] - this->position[1]), 2));
}

vector<float> Vertex::getPosition(void)
{
	return this->position;
}

float Vertex::getPositionX(void)
{
	return this->position[0]; 
}

float Vertex::getPositionY(void)
{
	return this->position[1];
}

vector<float> Vertex::getMoveToVertex(void) { return this->moveToVertex; }
vector<float> Vertex::getDirectionVector(void) { return this->directionVector; }
float Vertex::getDirectionVectorX(void) { return this->directionVector[0]; }
float Vertex::getDirectionVectorY(void) { return this->directionVector[1]; }
float Vertex::getSpeed(void) { return speed; }
vector <float> Vertex::getColor(void) { return color; }
float Vertex::getSize(void) { return size; }
bool Vertex::getIsMoving(void) { return isMoving; }
float Vertex::getColorR(void) { return this->color[0]; }
float Vertex::getColorG(void) { return this->color[1]; }
float Vertex::getColorB(void) { return this->color[2]; }
float Vertex::getColorA(void) { return this->color[3]; }

//setters
void Vertex::setPosition(float x, float y)
{
	this->position[0] = x;
	this->position[1] = y;
}

void Vertex::setPosition(vector<float> position)
{
	this->position[0] = position[0];
	this->position[1] = position[1];
}

void Vertex::setMoveToVertex(float x, float y)
{
	this->moveToVertex[0] = x;
	this->moveToVertex[1] = y;

	calculateDirectionVector();
}

void Vertex::setMoveToVertex(vector<float> position)
{
	this->moveToVertex[0] = position[0];
	this->moveToVertex[1] = position[1];

	calculateDirectionVector();
}

void Vertex::setSpeed(float speed)
{
	if (speed >= 0)
		this->speed = speed;
}

void Vertex::setSize(float size)
{
	if (size >= 1)
		this->size = size;
}

void Vertex::setColor(float r, float g, float b, float a)
{
	if (0 <= r && 0 <= g && 0 <= b 
		&& r <= 1 && g <= 1 && b <= 1
		&& 0 <= a && a <= 1)
	{
		this->color[0] = r;
		this->color[1] = g;
		this->color[2] = b;
		this->color[3] = a;
	}
}

void Vertex::setIsMoving(bool isMoving)
{
	this->isMoving = isMoving;
}

void Vertex::invertDirectionX(void)
{
	this->directionVector[0] *= -1;
}

void Vertex::invertDirectionY(void)
{
	this->directionVector[1] *= -1;
}