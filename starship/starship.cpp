#include <cmath>
#include <iostream>
#include <GL/glut.h> 
#include <fstream>

using namespace std;

#define PI 3.1415927

GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = 0.0f;

GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;

GLfloat camY = 0.0f;
GLfloat camX = 0.0f;
GLfloat camZ = 0.0f;

bool showWireframe = true;
bool showAxes = true;
bool showGrid = true;


GLfloat starshipHeight = 8.3; //11
GLfloat superheavyHeight = 15;
GLfloat rocketRadius = 1;
GLUquadricObj* qobj;

GLfloat topConeHeight = 2.3;
GLfloat topConeTopRad = 0.2;
GLfloat topConeBottomRad = 1;
GLfloat topFinHeight = 3.5;
GLfloat bottomFinHeight = 4;


struct BitMapFile
{
	int sizeX;
	int sizeY;
	unsigned char* data;
};

BitMapFile* getbmp(string filename)
{
	int offset, headerSize;

	// Initialize bitmap files for RGB (input) and RGBA (output).
	BitMapFile* bmpRGB = new BitMapFile;
	BitMapFile* bmpRGBA = new BitMapFile;

	// Read input bmp file name.
	ifstream infile(filename.c_str(), ios::binary);

	// Get starting point of image data in bmp file.
	infile.seekg(10);
	infile.read((char*)&offset, 4);

	// Get header size of bmp file.
	infile.read((char*)&headerSize, 4);

	// Get image width and height values from bmp file header.
	infile.seekg(18);
	infile.read((char*)&bmpRGB->sizeX, 4);
	infile.read((char*)&bmpRGB->sizeY, 4);

	// Determine the length of zero-byte padding of the scanlines 
	// (each scanline of a bmp file is 4-byte aligned by padding with zeros).
	int padding = (3 * bmpRGB->sizeX) % 4 ? 4 - (3 * bmpRGB->sizeX) % 4 : 0;

	// Add the padding to determine size of each scanline.
	int sizeScanline = 3 * bmpRGB->sizeX + padding;

	// Allocate storage for image in input bitmap file.
	int sizeStorage = sizeScanline * bmpRGB->sizeY;
	bmpRGB->data = new unsigned char[sizeStorage];

	// Read bmp file image data into input bitmap file.
	infile.seekg(offset);
	infile.read((char*)bmpRGB->data, sizeStorage);

	// Reverse color values from BGR (bmp storage format) to RGB.
	int startScanline, endScanlineImageData, temp;
	for (int y = 0; y < bmpRGB->sizeY; y++)
	{
		startScanline = y * sizeScanline; // Start position of y'th scanline.
		endScanlineImageData = startScanline + 3 * bmpRGB->sizeX; // Image data excludes padding.
		for (int x = startScanline; x < endScanlineImageData; x += 3)
		{
			temp = bmpRGB->data[x];
			bmpRGB->data[x] = bmpRGB->data[x + 2];
			bmpRGB->data[x + 2] = temp;
		}
	}

	// Set image width and height values and allocate storage for image in output bitmap file.
	bmpRGBA->sizeX = bmpRGB->sizeX;
	bmpRGBA->sizeY = bmpRGB->sizeY;
	bmpRGBA->data = new unsigned char[4 * bmpRGB->sizeX * bmpRGB->sizeY];

	// Copy RGB data from input to output bitmap files, set output A to 1.
	for (int j = 0; j < 4 * bmpRGB->sizeY * bmpRGB->sizeX; j += 4)
	{
		bmpRGBA->data[j] = bmpRGB->data[(j / 4) * 3];
		bmpRGBA->data[j + 1] = bmpRGB->data[(j / 4) * 3 + 1];
		bmpRGBA->data[j + 2] = bmpRGB->data[(j / 4) * 3 + 2];
		bmpRGBA->data[j + 3] = 0xFF;
	}

	return bmpRGBA;
}

static unsigned int texture[4];

// Load external textures.
void loadExternalTextures()
{
	// Local storage for bmp image data.
	BitMapFile* image[4];

	image[0] = getbmp("C:/Users/kanis/Desktop/gray-shiny-background.bmp");

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	glGenTextures(2, texture);
	loadExternalTextures();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_NORMALIZE);


}

void drawAxes() {
	glBegin(GL_LINES);

	glLineWidth(1.5);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-25.0, 0.0, 0.0);
	glVertex3f(25.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -25.0, 0.0);
	glVertex3f(0.0, 25.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -25);
	glVertex3f(0.0, 0.0, 25);

	glEnd();
}

void drawGrid() {
	GLfloat ext = 25.0f;
	GLfloat step = 1.0f ;
	GLfloat yGrid = 0.0f;
	GLint line;

	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_LINES);
	for (line = -ext; line <= ext; line += step) {
		glVertex3f(line, yGrid, ext);
		glVertex3f(line, yGrid, -ext);

		glVertex3f(ext, yGrid, line);
		glVertex3f(-ext, yGrid, line);
	}
	glEnd();
}

void drawCylinder(GLfloat radius, GLfloat height, GLfloat r, GLfloat g, GLfloat b) {
	GLfloat y = 0.0;
	GLfloat z = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.1;

	glColor3f(r,g,b);

	// TUBE
	glBegin(GL_QUAD_STRIP);
	angle = 2 * PI;
	while (angle >= 0.0) {
		y = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(height, y, z);
		glVertex3f(0.0, y, z);
		angle = angle - angle_stepsize;
	}
	glVertex3f(height, 0.0, radius);
	glVertex3f(0.0, 0.0, radius);
	glEnd();

	// BACK
	glColor3f(1.0, 1.0, 0.5);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		y = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(0.0, y, z);
		angle = angle + angle_stepsize;
	}
	glVertex3f(0.0, 0.0, radius);
	glEnd();

	// FRONT
	glColor3f(1.0, 1.0, 0.5);
	glBegin(GL_POLYGON);
	angle = 2 * PI - angle_stepsize;
	while (angle >= 0.0) {
		y = radius * sin(angle);
		z = radius * cos(angle);
		glVertex3f(height, y, z);
		angle = angle - angle_stepsize;
	}
	glVertex3f(height, 0.0, radius);
	glEnd();
}

void drawStarship() {

	

	glPushMatrix();
	
	glRotatef(90,0,0,1);
	glTranslatef(superheavyHeight,0,0);
	
	drawCylinder(rocketRadius,starshipHeight, 1,0,1);
	glPopMatrix();
}



void drawSuperheavy() {
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	drawCylinder(rocketRadius, superheavyHeight,1,1,1);
	glPopMatrix();
}
 



void drawTopCone() {
glPushMatrix();
	glColor3f(1,1,1);
	glTranslatef(0,starshipHeight+superheavyHeight,0);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(qobj, topConeBottomRad, topConeTopRad, topConeHeight, 50, 50);
	glPopMatrix();
	

	glPushMatrix();
	glTranslatef(0, starshipHeight + superheavyHeight + topConeHeight -0.07, 0);
	glutSolidSphere(topConeTopRad, 100, 100);
	
	glPopMatrix();
	
}

void drawStarshipTopFins() {
	glPushMatrix();
	glTranslatef(0, starshipHeight + superheavyHeight + topConeHeight - topFinHeight, 0.15);
	glBegin(GL_POLYGON);
	
	//large face
	glColor3f(1,1,1);
	glVertex3f(0,0,0);
	glVertex3f(1,0,0);
	glVertex3f(2, 0.3, 0);
	glVertex3f(2, 0.6, 0);
	glVertex3f(0, topFinHeight, 0);
	glVertex3f(-2, 0.6, 0);
	glVertex3f(-2, 0.3, 0);
	glVertex3f(-1, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	
	glPopMatrix();
	//large face
	glPushMatrix();
	glTranslatef(0, starshipHeight + superheavyHeight + topConeHeight - topFinHeight, -0.15);
	glBegin(GL_POLYGON);

	glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(2, 0.3, 0);
	glVertex3f(2, 0.6, 0);
	glVertex3f(0, topFinHeight, 0);
	glVertex3f(-2, 0.6, 0);
	glVertex3f(-2, 0.3, 0);
	glVertex3f(-1, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();

glPopMatrix();

	//bottom rectangle
	glPushMatrix();
	glTranslatef(0, starshipHeight + superheavyHeight + topConeHeight - topFinHeight, 0);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(1, 0, 0.15);
	glVertex3f(1, 0, -0.15);
	glVertex3f(-1, 0, -0.15);
	glVertex3f(-1, 0, 0.15);
	glVertex3f(1, 0, 0.15);
	glEnd();

	//bottom slant rectangle 1
	glBegin(GL_POLYGON);
	glVertex3f(1, 0, 0.15);
	glVertex3f(1, 0, -0.15);
	glVertex3f(2, 0.3, -0.15);
	glVertex3f(2, 0.3, 0.15);
	glEnd();
	

	glBegin(GL_POLYGON);
	glVertex3f(-1, 0, 0.15);
	glVertex3f(-1, 0, -0.15);
	glVertex3f(-2, 0.3, -0.15);
	glVertex3f(-2, 0.3, 0.15);
	glVertex3f(-1, 0, 0.15);
	glEnd();

	//side straight rectangle
	glBegin(GL_POLYGON);
	glVertex3f(2, 0.3, -0.15);
	glVertex3f(2, 0.3, 0.15);
	glVertex3f(2, 0.6, 0.15);
	glVertex3f(2, 0.6, -0.15);
	glVertex3f(2, 0.3, -0.15);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-2, 0.3, -0.15);
	glVertex3f(-2, 0.3, 0.15);
	glVertex3f(-2, 0.6, 0.15);
	glVertex3f(-2, 0.6, -0.15);
	glVertex3f(-2, 0.3, -0.15);
	glEnd();

	//side top slant rectangle
	glBegin(GL_POLYGON);
	glVertex3f(2, 0.6, 0.15);
	glVertex3f(2, 0.6, -0.15);
	glVertex3f(0, topFinHeight, -0.15);
	glVertex3f(0, topFinHeight, 0.15);
	glVertex3f(2, 0.6, 0.15);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-2, 0.6, 0.15);
	glVertex3f(-2, 0.6, -0.15);
	glVertex3f(0, topFinHeight, -0.15);
	glVertex3f(0, topFinHeight, 0.15);
	glVertex3f(-2, 0.6, 0.15);
	glEnd();

	glPopMatrix();
	
}

void drawStarshipBottomFins() {
	
	//large face
	glPushMatrix();
	glTranslatef(0, superheavyHeight, -0.15);
	glBegin(GL_POLYGON);

	glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(2, 0, 0);
	glVertex3f(2, 1.5, 0);
	glVertex3f(1, bottomFinHeight, 0);
	glVertex3f(-1, bottomFinHeight, 0);
	glVertex3f(-2, 1.5, 0);
	glVertex3f(-2, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();

	//large face
	glPushMatrix();
	glTranslatef(0, superheavyHeight, 0.15);
	glBegin(GL_POLYGON);

	glColor3f(1, 1, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(2, 0, 0);
	glVertex3f(2, 1.5, 0);
	glVertex3f(1, bottomFinHeight, 0);
	glVertex3f(-1, bottomFinHeight, 0);
	glVertex3f(-2, 1.5, 0);
	glVertex3f(-2, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	glPopMatrix();

	//rectangular sides
	glPushMatrix();
	glTranslatef(0, superheavyHeight, 0);
	
	//bottom
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(2, 0, 0.15);
	glVertex3f(-2, 0, 0.15);
	glVertex3f(-2, 0, -0.15);
	glVertex3f(2, 0, -0.15);
	glVertex3f(2, 0, 0.15);
	glEnd();

	//side
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(2, 0, 0.15);
	glVertex3f(2, 0, -0.15);
	glVertex3f(2, 1.5, -0.15);
	glVertex3f(2, 1.5, 0.15);
	glVertex3f(2, 0, 0.15);
	glEnd();

	//side
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(-2, 0, 0.15);
	glVertex3f(-2, 0, -0.15);
	glVertex3f(-2, 1.5, -0.15);
	glVertex3f(-2, 1.5, 0.15);
	glVertex3f(-2, 0, 0.15);
	glEnd();

	//side slant
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(-2, 1.5, -0.15);
	glVertex3f(-2, 1.5, 0.15);
	glVertex3f(-1, bottomFinHeight, 0.15);
	glVertex3f(-1, bottomFinHeight, -0.15);
	glVertex3f(-2, 1.5, -0.15);
	glEnd();

	//side slant
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex3f(2, 1.5, -0.15);
	glVertex3f(2, 1.5, 0.15);
	glVertex3f(1, bottomFinHeight, 0.15);
	glVertex3f(1, bottomFinHeight, -0.15);
	glVertex3f(2, 1.5, -0.15);
	glEnd();

	glPopMatrix();


}

void drawStarshipSmallFins() {
	glPushMatrix();
	glTranslatef(0, 0, 0.05);
	glBegin(GL_POLYGON);
	
	glColor3f(1,1,1);
	glVertex3f(1.25,0,0);
	glVertex3f(1.25, 1.2, 0);
	glVertex3f(1, 1.5, 0);
	glVertex3f(-1, 1.5, 0);
	glVertex3f(-1.25, 1.2, 0);
	glVertex3f(-1.25, 0, 0);
	glVertex3f(1.25, 0, 0);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -0.05);
	glBegin(GL_POLYGON);
	
	glColor3f(1, 1, 1);
	glVertex3f(1.25, 0, 0);
	glVertex3f(1.25, 1.2, 0);
	glVertex3f(1, 1.5, 0);
	glVertex3f(-1, 1.5, 0);
	glVertex3f(-1.25, 1.2, 0);
	glVertex3f(-1.25, 0, 0);
	glVertex3f(1.25, 0, 0);

	glEnd();
	glPopMatrix();

	//bottom rectangle
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(1.25, 0, 0.05);
	glVertex3f(1.25, 0, -0.05);
	glVertex3f(-1.25, 0, -0.05);
	glVertex3f(1.25, 0, 0.05);
	glEnd();

	//side staraight
	glBegin(GL_QUADS);
	glVertex3f(1.25, 0, 0.05);
	glVertex3f(1.25, 0, -0.05);
	glVertex3f(1.25, 1.2, -0.05);
	glVertex3f(1.25, 1.2, 0.05);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-1.25, 0, 0.05);
	glVertex3f(-1.25, 0, -0.05);
	glVertex3f(-1.25, 1.2, -0.05);
	glVertex3f(-1.25, 1.2, 0.05);
	glEnd();

	//side slant
	glBegin(GL_QUADS);
	glVertex3f(1.25, 1.2, -0.05);
	glVertex3f(1.25, 1.2, 0.05);
	glVertex3f(1, 1.5, 0.05);
	glVertex3f(1, 1.5, -0.05);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-1.25, 1.2, -0.05);
	glVertex3f(-1.25, 1.2, 0.05);
	glVertex3f(-1, 1.5, 0.05);
	glVertex3f(-1, 1.5, -0.05);
	glEnd();

	//top rectangle
	glBegin(GL_QUADS);
	glVertex3f(1, 1.5, 0.05);
	glVertex3f(1, 1.5, -0.05);
	glVertex3f(-1, 1.5, -0.05);
	glVertex3f(-1, 1.5, 0.05);
	glEnd();

	glPopMatrix();
}

void drawSuperheavyBottomFins() {
	glPushMatrix();
	glTranslatef(0, 0, 0.15);
	glBegin(GL_POLYGON);

	glVertex3f(2, 0, 0);
	glVertex3f(2, 1, 0);
	glVertex3f(1, 3.75, 0);
	glVertex3f(-1, 3.75, 0);
	glVertex3f(-2, 1, 0);
	glVertex3f(-2, 0, 0);
	glVertex3f(2, 0, 0);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -0.15);
	glBegin(GL_POLYGON);

	glVertex3f(2, 0, 0);
	glVertex3f(2, 1, 0);
	glVertex3f(1, 3.75, 0);
	glVertex3f(-1, 3.75, 0);
	glVertex3f(-2, 1, 0);
	glVertex3f(-2, 0, 0);
	glVertex3f(2, 0, 0);

	glEnd();
	glPopMatrix();

	//bottom rectangle
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(2, 0, 0.15);
	glVertex3f(2, 0, -0.15);
	glVertex3f(-2, 0, -0.15);
	glVertex3f(2, 0, 0.15);
	glEnd();

	//side staraight
	glBegin(GL_QUADS);
	glVertex3f(2, 0, 0.15);
	glVertex3f(2, 0, -0.15);
	glVertex3f(2, 1, -0.15);
	glVertex3f(2, 1, 0.15);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-2, 0, 0.15);
	glVertex3f(-2, 0, -0.15);
	glVertex3f(-2, 1, -0.15);
	glVertex3f(-2, 1, 0.15);
	glEnd();

	//side slant
	glBegin(GL_QUADS);
	glVertex3f(2, 1, -0.15);
	glVertex3f(2, 1, 0.15);
	glVertex3f(1, 3.75, 0.15);
	glVertex3f(1, 3.75, -0.15);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(-2, 1, -0.15);
	glVertex3f(-2, 1, 0.15);
	glVertex3f(-1, 3.75, 0.15);
	glVertex3f(-1, 3.75, -0.15);
	glEnd();

	//top rectangle
	glBegin(GL_QUADS);
	glVertex3f(1, 3.75, 0.15);
	glVertex3f(1, 3.75, -0.15);
	glVertex3f(-1, 3.75, -0.15);
	glVertex3f(-1, 3.75, 0.15);
	glEnd();

	glPopMatrix();
}

void renderStarship() {
	glPushMatrix();
	drawStarship();
	drawTopCone();
	drawStarshipTopFins();
	drawStarshipBottomFins();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, superheavyHeight, 0);
	glRotatef(45, 0, 1,0);
	drawStarshipSmallFins();

	glRotatef(90, 0, 1, 0);
	drawStarshipSmallFins();
	glPopMatrix();
}

void renderSuperheavy() {
	glPushMatrix();
	drawSuperheavy();
	glPopMatrix();

	glPushMatrix();
	drawSuperheavyBottomFins();
	
	glRotatef(45, 0, 1, 0);
	drawSuperheavyBottomFins();

	glRotatef(90, 0, 1, 0);
	drawSuperheavyBottomFins();
	glPopMatrix();
}

void drawLaunchTower() {
	//front
	glPushMatrix();
	glRotatef(90,0,1,0);
	glTranslatef(-6,0,0.5);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, superheavyHeight + starshipHeight, 0);
	glVertex3f(4, superheavyHeight + starshipHeight, 0);
	glVertex3f(4, superheavyHeight + starshipHeight - 1, 0);
	glVertex3f(2, superheavyHeight + starshipHeight - 4, 0);
	glVertex3f(2, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();

	//back
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-6, 0, -0.5);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, superheavyHeight + starshipHeight, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(2, 0, 0);
	glVertex3f(2, superheavyHeight + starshipHeight - 4, 0);
	glVertex3f(4, superheavyHeight + starshipHeight - 1, 0);
	glVertex3f(4, superheavyHeight + starshipHeight, 0);
	glEnd();
	glPopMatrix();

	//right
	glPushMatrix();
	glTranslatef(0, 0,6);
	glBegin(GL_QUADS);
	glVertex3f(0.5, superheavyHeight + starshipHeight, 0);
	glVertex3f(-0.5, superheavyHeight + starshipHeight, 0);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(0.5, 0, 0);
	glEnd();
	glPopMatrix();

	//top
	glPushMatrix();
	glTranslatef(0, 0, 2);
	glBegin(GL_QUADS);
	glVertex3f(0.5, superheavyHeight + starshipHeight, 0);
	glVertex3f(-0.5, superheavyHeight + starshipHeight, 0);
	glVertex3f(-0.5, superheavyHeight + starshipHeight, 4);
	glVertex3f(0.5, superheavyHeight + starshipHeight, 4);
	
	glEnd();
	glPopMatrix();

	//left
	glPushMatrix();
	glTranslatef(0,0,2);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(0.5, superheavyHeight + starshipHeight, 0);
	glVertex3f(-0.5, superheavyHeight + starshipHeight, 0);
	glVertex3f(0.5, superheavyHeight + starshipHeight-1, 0);
	glVertex3f(-0.5, superheavyHeight + starshipHeight - 1, 0);
	glVertex3f(0.5, superheavyHeight + starshipHeight - 4, 2);
	glVertex3f(-0.5, superheavyHeight + starshipHeight - 4, 2);
	glVertex3f(0.5, 0, 2);
	glVertex3f(-0.5, 0, 2);
	glEnd();
	glPopMatrix();

	//bottom
	glPushMatrix();
	glTranslatef(0, 0, 4);
	glBegin(GL_QUADS);
	glVertex3f(0.5, 0, 0);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(-0.5, 0, 2);
	glVertex3f(0.5, 0, 2);
	glEnd();
	glPopMatrix();
}

void drawLaunchSupports() {
	GLint cornerPollLength = 5;
	GLint cornerPollHeight = 1;

	

	//four corner polls
	glPushMatrix();
	glTranslatef(0, 0, 0.3);
	drawCylinder(0.1, cornerPollLength, 1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -0.3);
	drawCylinder(0.1, cornerPollLength, 1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, cornerPollHeight, 0.3);
	drawCylinder(0.1, cornerPollLength, 1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, cornerPollHeight, -0.3);
	drawCylinder(0.1, cornerPollLength, 1, 1, 1);
	glPopMatrix();


	//criss cross hatch
	for (int i = 0; i < cornerPollLength; i++) {
		glPushMatrix();
		glTranslatef(i, 0, -0.3);
		glRotatef(45, 0, 0, 1);

		drawCylinder(0.05, 1.5, 1, 1, 1);
		glPopMatrix();
	}


	for (int i = 0; i < cornerPollLength; i++) {
		glPushMatrix();
		glTranslatef(i, 0, 0.3);
		glRotatef(45, 0, 0, 1);

		drawCylinder(0.05, 1.5, 1, 1, 1);
		glPopMatrix();
	}

	for (int i = 1; i < cornerPollLength + 1; i++) {
		glPushMatrix();
		glTranslatef(i, 0, -0.3);
		glRotatef(135, 0, 0, 1);

		drawCylinder(0.05, 1.5, 1, 1, 1);

		glPopMatrix();
	}

	for (int i = 1; i < cornerPollLength + 1; i++) {
		glPushMatrix();
		glTranslatef(i, 0, 0.3);
		glRotatef(135, 0, 0, 1);

		drawCylinder(0.05, 1.5, 1, 1, 1);

		glPopMatrix();
	}

	//crossbars bottom
	for (int i = 1; i < cornerPollLength; i++) {
		glPushMatrix();
		glTranslatef(i, 0, 0.3);
		glRotatef(90, 0, 1, 0);

		drawCylinder(0.05, 0.6, 1, 1, 1);

		glPopMatrix();
	}

	//crossbars top
	for (int i = 1; i < cornerPollLength; i++) {
		glPushMatrix();
		glTranslatef(i, 1, 0.3);
		glRotatef(90, 0, 1, 0);

		drawCylinder(0.05, 0.6, 1, 1, 1);

		glPopMatrix();
	}
	
}

void renderLaunchTower() {
	glPushMatrix();
	drawLaunchTower();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glTranslatef(0, superheavyHeight - 3, 0);
	drawLaunchSupports();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glTranslatef(0, superheavyHeight + starshipHeight - 5, 0);
	drawLaunchSupports();
	glPopMatrix();
}

void renderLaunchStage() {
	glPushMatrix();

	//front slant
	
	glBegin(GL_QUADS);
	glVertex3f(-5, 0, 10);
	glVertex3f(-5, 0, -6);
	glVertex3f(-15, -4, -6);
	glVertex3f(-15, -4, 10);

	glEnd();

	//side1
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	glVertex3f(4, 0, 10);
	glVertex3f(-5, 0, 10);
	glVertex3f(-5, -2, 20);
	glVertex3f(4, -2, 20);
	glEnd();

	//side2
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex3f(-5, 0, -6);
	glVertex3f(4, 0, -6);
	glVertex3f(4, -2, -20);
	glVertex3f(-5, -2, -20);
	glEnd();

	//slant side1
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.5, 0.5);
	glVertex3f(-5, 0, 10);
	glVertex3f(-15, -4, 10);
	glVertex3f(-5, -2, 20);
	glVertex3f(-15, -4, 20);
	glEnd();

	//slant side2
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.5, 0.5);
	glVertex3f(-5, 0, -6);
	glVertex3f(-15, -4, -6);
	glVertex3f(-5, -2, -20);
	glVertex3f(-15, -4, -20);
	glEnd();

	//vertical side1
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.5, 0.5);
	glVertex3f(-5, 0, -6);
	glVertex3f(-15, -4, -6);
	glVertex3f(-5, -2, -20);
	glVertex3f(-15, -4, -20);
	glEnd();

	//vertical side2
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.5, 0.5);
	glVertex3f(-5, 0, -6);
	glVertex3f(-15, -4, -6);
	glVertex3f(-5, -2, -20);
	glVertex3f(-15, -4, -20);
	glEnd();

	//back slant side1
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.1, 0.5);
	glVertex3f(4, 0, 10);
	glVertex3f(4, -2, 20);
	glVertex3f(10, -4, 10);
	glVertex3f(10, -4, 20);
	glEnd();

	//back slant side2
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.1, 0.5);
	glVertex3f(4, 0, -6);
	glVertex3f(4, -2, -20);
	glVertex3f(10, -4, -6);
	glVertex3f(10, -4, -20);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5, 0.1, 0.5);
	glVertex3f(10, -4, 10);
	glVertex3f(4, 0, 10);
	glVertex3f(4, -4, 10);
	glVertex3f(-5, 0, 10);
	glVertex3f(-5, -4, 10);
	glVertex3f(-5, 0, -6);
	glVertex3f(-5, -4, -6);
	glVertex3f(4, 0, -6);
	glVertex3f(4, -4, -6);
	glVertex3f(10, -4, -6);
	glEnd();


	GLfloat elevation = 0.5f;
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);

	//side 1
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-5, 0, 10);
	glVertex3f(-5, -elevation, 10);
	glVertex3f(4, -elevation, 10);
	glVertex3f(4, 0, 10);


	//back
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(4, 0, 10);
	glVertex3f(4, -elevation, 10);
	glVertex3f(4, -elevation, -6);
	glVertex3f(4, 0, -6);

	//side 2
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(4, 0, -6);
	glVertex3f(4, -elevation, -6);
	glVertex3f(-5, -elevation, -6);
	glVertex3f(-5, 0, -6);

	//front
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-5, 0, 10);
	glVertex3f(-5, 0, -6);
	glVertex3f(-5, -elevation, -6);
	glVertex3f(-5, -elevation, 10);

	//top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-5, 0, 10);
	glVertex3f(4, 0, 10);
	glVertex3f(4, 0, -6);
	glVertex3f(-5, 0, -6);

	//bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-5, -elevation, 10);
	glVertex3f(4, -elevation, 10);
	glVertex3f(4, -elevation, -6);
	glVertex3f(-5, -elevation, -6);

	glEnd();

	glPopMatrix();
}

void drawO2Tank() {
	double eqnTop[] = { 0.0, 0.0, 1.0, 0.0 };
	//Cylinder tank
	glPushMatrix();
	gluCylinder(qobj, 2, 2, 2.5, 100, 100);
	glPopMatrix();

	//top dome 
	glTranslatef(0, 0, 2.5);
	glPushMatrix();

	glClipPlane(GL_CLIP_PLANE0, eqnTop);
	glEnable(GL_CLIP_PLANE0);

	glTranslatef(0, 0, -2.21);
	glColor3f(1, 0, 1);
	glutSolidSphere(3, 100, 100);

	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();


	//bottom dome 
	glTranslatef(0, 0, -2.5);
	glRotatef(180, 1, 0, 0);
	glPushMatrix();
	glClipPlane(GL_CLIP_PLANE0, eqnTop);
	glEnable(GL_CLIP_PLANE0);
	glColor3f(1, 0, 1);
	glTranslatef(0, 0, -2.21);
	glutSolidSphere(3, 100, 100);

	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
}

void drawO2TankPillars() {
	
	GLfloat towerPillarHeight = 10;
	GLfloat towerPillarDistance = 3;
	GLfloat towerPillarLeanAngle = 9;
	GLfloat mainPipeRadius = 0.5;
	GLfloat supportPillarRadius = 0.20;

	

	glRotatef(90, -1, 0, 0);

	//center pipe
	gluCylinder(qobj, 0.5, 0.5, towerPillarHeight, 100, 100);

	//x
	glPushMatrix();
	glTranslatef(towerPillarDistance,0,0);
	glRotatef(towerPillarLeanAngle, 0, -1, 0);
	gluCylinder(qobj, supportPillarRadius, supportPillarRadius, towerPillarHeight, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-towerPillarDistance, 0, 0);
	glRotatef(towerPillarLeanAngle, 0, 1, 0);
	gluCylinder(qobj, supportPillarRadius, supportPillarRadius, towerPillarHeight, 100, 100);
	glPopMatrix();

	//y
	glPushMatrix();
	glTranslatef(0, -towerPillarDistance, 0);
	glRotatef(towerPillarLeanAngle, -1, 0, 0);
	gluCylinder(qobj, supportPillarRadius, supportPillarRadius, towerPillarHeight, 100, 100);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0, towerPillarDistance, 0);
	glRotatef(towerPillarLeanAngle, 1, 0, 0);
	gluCylinder(qobj, supportPillarRadius, supportPillarRadius, towerPillarHeight, 100, 100);
	glPopMatrix();


	//diagonal pillars
	glPushMatrix();//diagonal pillars
	glRotatef(45, 0, 0, 1);
	//x
	glPushMatrix();
	glTranslatef(towerPillarDistance, 0, 0);
	glRotatef(towerPillarLeanAngle, 0, -1, 0);
	gluCylinder(qobj, supportPillarRadius, supportPillarRadius, towerPillarHeight, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-towerPillarDistance, 0, 0);
	glRotatef(towerPillarLeanAngle, 0, 1, 0);
	gluCylinder(qobj, supportPillarRadius, supportPillarRadius, towerPillarHeight, 100, 100);
	glPopMatrix();

	//y
	glPushMatrix();
	glTranslatef(0, -towerPillarDistance, 0);
	glRotatef(towerPillarLeanAngle, -1, 0, 0);
	gluCylinder(qobj, supportPillarRadius, supportPillarRadius, towerPillarHeight, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, towerPillarDistance, 0);
	glRotatef(towerPillarLeanAngle, 1, 0, 0);
	gluCylinder(qobj, supportPillarRadius, supportPillarRadius, towerPillarHeight, 100, 100);
	glPopMatrix();

	glPopMatrix();//diagonal pillars

	glPushMatrix();
	glColor3f(1,0.25,0.25);
	glTranslatef(0,0,2);
	glutSolidTorus(0.125, 2.69, 100, 100);

	glTranslatef(0, 0, 2);
	glutSolidTorus(0.125, 2.366, 100, 100);

	glTranslatef(0, 0, 2);
	glutSolidTorus(0.125, 2.05, 100, 100);

	glTranslatef(0, 0, 2);
	glutSolidTorus(0.125, 1.73, 100, 100);
	glPopMatrix();
}


void renderO2Tank() {
	
	glPushMatrix();
	drawO2TankPillars();
	glTranslatef(0,0,10);
	drawO2Tank();
	glFlush();
	glPopMatrix();
}

void drawComTower() {
	GLfloat comTowerHeight = 12;
	GLfloat comTowerPillarRadius = 0.15;
	GLfloat comTowerDistanceFromOrigin = 0.5;
	
	
	glRotatef(90, -1, 0, 0);
	glPushMatrix();
	//pillars
	glPushMatrix();
	glTranslatef(comTowerDistanceFromOrigin, comTowerDistanceFromOrigin,0);
	gluCylinder(qobj, comTowerPillarRadius, comTowerPillarRadius, comTowerHeight, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-comTowerDistanceFromOrigin, -comTowerDistanceFromOrigin, 0);
	gluCylinder(qobj, comTowerPillarRadius, comTowerPillarRadius, comTowerHeight, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-comTowerDistanceFromOrigin, comTowerDistanceFromOrigin, 0);
	gluCylinder(qobj, comTowerPillarRadius, comTowerPillarRadius, comTowerHeight, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(comTowerDistanceFromOrigin, -comTowerDistanceFromOrigin, 0);
	gluCylinder(qobj, comTowerPillarRadius, comTowerPillarRadius, comTowerHeight, 100, 100);
	glPopMatrix();
	

	//cylindrical circles
	glPushMatrix();
	for (int i = 3; i < comTowerHeight;i+=3) {
		glTranslatef(0,0,3);
		glutSolidTorus(0.15,comTowerDistanceFromOrigin + comTowerPillarRadius,100,100);
	}
	glPopMatrix();
	glPopMatrix();

	GLfloat topAntennaeStageHeight = 3;
	GLfloat topAntennaeStagePillarRadius = 0.1;
	GLfloat stageRadius = 2;
	GLfloat antennaeRadius = 0.5;
	GLfloat antennaeHeight = 7;

	glPushMatrix();
	glTranslatef(0,0,comTowerHeight);
	// antennae top
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,0,0);
	for (float i = 0; i < 360; i += 1.2) {
		glVertex3f(stageRadius * cos(i), stageRadius * sin(i), 0.0);
		glVertex3f(stageRadius * cos(i + 0.2), stageRadius * sin(i + 0.2), 0.0);
	}

	glEnd();
	glPopMatrix();

	// antennae top
	glPushMatrix();
	glTranslatef(0,0,topAntennaeStageHeight);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (float i = 0; i < 360; i += 1.2) {
		glVertex3f(stageRadius * cos(i), stageRadius * sin(i), 0.0);
		glVertex3f(stageRadius * cos(i + 0.2), stageRadius * sin(i + 0.2), 0.0);
	}

	glEnd();
	glPopMatrix();


	//circular pillars
	glPushMatrix();
	for (int i = 0; i < topAntennaeStageHeight;i+=1) {
		glTranslatef(0,0,1);
		glutSolidTorus(topAntennaeStagePillarRadius,stageRadius + topAntennaeStagePillarRadius,100,100);
	}
	glPopMatrix();

	//vertical pillars
	glPushMatrix();
	for (float j = 0; j < 360; j += 45) {
		glRotatef(45, 0, 0, 1);
		glPushMatrix();
		glTranslatef(stageRadius - topAntennaeStagePillarRadius,0,0);
		gluCylinder(qobj, topAntennaeStagePillarRadius, topAntennaeStagePillarRadius, topAntennaeStageHeight, 100, 100);
		glPopMatrix();
	}
	glPopMatrix();

	//centre antennae
	glPushMatrix();
	gluCylinder(qobj, antennaeRadius, antennaeRadius, antennaeHeight,100,100);

	glPopMatrix();
	

	glPopMatrix();
	
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	gluLookAt(10.0 + camX, 10.0 + camY, 10.0 + camZ, 0, 0, 0, 0, 1.0, 0);

	glTranslatef(moveX, moveY, moveZ);
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

	if (showAxes) {
		drawAxes();
	}

	if (showGrid) {
		drawGrid();
	}
	glPushMatrix();
	glTranslatef(0,4,0);
	renderStarship();
	renderSuperheavy();
	renderLaunchTower();
	renderLaunchStage();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-17,0,-15);
	renderO2Tank();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(20, 0, -20);
	drawComTower();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20, 0, 20);
	drawComTower();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, 0, 20);
	drawComTower();
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}



void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		moveZ += 1;
	}
	else if (key == GLUT_KEY_DOWN) {
		moveZ -= 1;
	}
	else if (key == GLUT_KEY_LEFT) {
		moveX += 1;
	}
	else if (key == GLUT_KEY_RIGHT) {
		moveX -= 1;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'w') {
		camY += 0.5;
	}
	else if (key == 's') {
		camY -= 0.5;
	}
	else if (key == 'a') {
		rotY += 5.0;
	}
	else if (key == 'd') {
		rotY -= 5.0;
	}
	else if (key == '2') {
		moveY += 1;
	}
	else if (key == '8') {
		moveY -= 1;
	}
	else if (key == 'c') {
		if (showWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		showWireframe = !showWireframe;
	}
	else if (key == 'x') {
		showAxes = !showAxes;
	}
	else if (key == 'g') {
		showGrid = !showGrid;
	}
	else if (key == 'r') {
		camY = -10;
		rotY = 145;
		moveY = 0;
		moveX = 7;
		moveZ = 10;
	}
	else if (key == 'p') {
		std::cout << camY << "|" << rotY << "|" << moveY << "|" << moveX << "|" << moveZ;
	}
	glutPostRedisplay();
}

void changeSize(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, aspect_ratio, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

	glutInitWindowSize(500, 500);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("Starship");

	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);

	init();
	glutMainLoop();

	return 0;
}