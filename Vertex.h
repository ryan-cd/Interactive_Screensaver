#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
//#  include <GL/gl.h>
//#  include <GL/glu.h>
//#  include <GL/freeglut.h>
#include <stdio.h>

using namespace std;

class Vertex
{
private:
	vector<float> position; //position[0] is x coord. position[1] is y coord
	vector<float> moveToVertex; //moveToVertex[0] is x coord. moveToVertex[1] is y coord
	vector<float> directionVector; //unit vector to the desired point. index 0 is x, index 1 is y
	vector<float> color; //index 0 = red, index 1 = green, index 2 = blue
	float speed;
	float size;
	bool isMoving; //whether the vertex is moving

	static int numVertices;
	//functions
	void calculateDirectionVector(void); //this will take the moveTo point and calculate the direction vector there
public:
	Vertex(vector<float> position, vector<float> moveToVertex, vector<float> color, float speed, float size);
	
	//getters
	vector<float> getPosition(void);
	float getPositionX(void);
	float getPositionY(void);
	vector<float> getMoveToVertex(void);
	vector<float> getDirectionVector(void);
	float getDirectionVectorX(void);
	float getDirectionVectorY(void);
	float getSpeed(void);
	vector <float> getColor(void);
	float getColorR(void);
	float getColorG(void);
	float getColorB(void);
	float getColorA(void);
	float getSize(void);
	bool getIsMoving(void);

	//setters
	void setPosition(float x, float y);
	void setPosition(vector<float> position);
	void setMoveToVertex(float x, float y);
	void setMoveToVertex(vector<float> position);
	void setSpeed(float speed);
	void setSize(float newSize);
	void setColor(float r, float g, float b, float a);
	void setIsMoving(bool isMoving);
	void invertDirectionX(void);
	void invertDirectionY(void);
};