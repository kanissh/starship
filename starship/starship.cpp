#include <cmath>
#include <iostream>
#include <GL/glut.h> 
#include <fstream>
#include <SOIL2.h>

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

GLuint steelTex;
GLuint steelTexFins;
GLuint concreteTex;
GLuint redPillarTex;
GLuint whiteMetalTex;
GLuint blackTex;
GLuint offGreyTex;
GLuint offWhiteTex;

void initTexture() {
steelTex = SOIL_load_OGL_texture
	(
		"../textures/steel-tex.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

steelTexFins = SOIL_load_OGL_texture
(
	"../textures/steel-tex-fins.jpg",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
);

concreteTex = SOIL_load_OGL_texture
(
	"../textures/concrete-tex.jpg",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
);

redPillarTex = SOIL_load_OGL_texture
(
	"../textures/red-pillar-tex.jpg",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
);

whiteMetalTex = SOIL_load_OGL_texture
(
	"../textures/white-metal-tex.jpg",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
);

blackTex = SOIL_load_OGL_texture
(
	"../textures/black-tex.jpg",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
);

offGreyTex = SOIL_load_OGL_texture
(
	"../textures/off-grey-tex.jpg",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
);

offWhiteTex = SOIL_load_OGL_texture
(
	"../textures/off-white-tex.jpg",
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
);


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
	glEnable(GL_NORMALIZE);
	initTexture();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, steelTex);
	gluQuadricTexture(qobj, GL_TRUE);	
	gluQuadricNormals(qobj, GLU_SMOOTH);
	
	glColor3f(1,0.4,1);
	glTranslatef(0,superheavyHeight,0);
	glRotatef(90, -1, 0, 0);
	
	gluCylinder(qobj, rocketRadius, rocketRadius, starshipHeight,100,100);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}



void drawSuperheavy() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, steelTex);
	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	glRotatef(90, -1, 0, 0);
	gluCylinder(qobj, rocketRadius, rocketRadius, superheavyHeight, 100, 100);
	glDisable(GL_TEXTURE_2D);
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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, steelTexFins);
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
	glDisable(GL_TEXTURE_2D);
	
}

void drawStarshipBottomFins() {
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, steelTexFins);
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
	glDisable(GL_TEXTURE_2D);
	
	
}

void drawStarshipSmallFins() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, steelTexFins);
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
	glDisable(GL_TEXTURE_2D);
}

void drawSuperheavyBottomFins() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, steelTexFins);
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
	glDisable(GL_TEXTURE_2D);
	
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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,offGreyTex);
	glPushMatrix();
	glRotatef(90,0,1,0);
	glTranslatef(-6,0,0.5);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(4, superheavyHeight + starshipHeight - 1, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(2, superheavyHeight + starshipHeight - 4, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(2, 0, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();

	//back
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-6, 0, -0.5);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0, 0.0); glVertex3f(0, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0, 0, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2, 0, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(2, superheavyHeight + starshipHeight - 4, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(4, superheavyHeight + starshipHeight - 1, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, superheavyHeight + starshipHeight, 0);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, blackTex);

	//right
	glPushMatrix();
	glTranslatef(0, 0,6);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.5, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0, 0);
	glEnd();
	glPopMatrix();

	//top
	glPushMatrix();
	glTranslatef(0, 0, 2);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.5, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, superheavyHeight + starshipHeight, 4);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.5, superheavyHeight + starshipHeight, 4);
	
	glEnd();
	glPopMatrix();

	//left
	glPushMatrix();
	glTranslatef(0,0,2);
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.5, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, superheavyHeight + starshipHeight, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.5, superheavyHeight + starshipHeight - 1, 0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, superheavyHeight + starshipHeight - 1, 0);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.5, superheavyHeight + starshipHeight - 4, 2);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, superheavyHeight + starshipHeight - 4, 2);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0, 2);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0, 2);
	glEnd();
	glPopMatrix();

	//bottom
	glPushMatrix();
	glTranslatef(0, 0, 4);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0, 0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0, 0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0, 2);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0, 2);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void drawLaunchSupports() {
	GLint cornerPollLength = 5;
	GLint cornerPollHeight = 1;
	GLfloat cornerPollRadius = 0.1;

	/*glPushMatrix();//glucylinder implementation
	glTranslatef(0, 0, 0.3);
	glRotatef(90,0,1,0);
	gluCylinder(qobj, cornerPollRadius, cornerPollRadius, cornerPollLength, 100, 100);
	glPopMatrix();*/

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, offWhiteTex);
	//four corner polls
	glPushMatrix();
	glTranslatef(0, 0, 0.3);
	//gluCylinder(qobj, cornerPollRadius, cornerPollRadius, cornerPollLength, 100,100);
	drawCylinder(0.1, cornerPollLength, 1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -0.3);
	//gluCylinder(qobj, cornerPollRadius, cornerPollRadius, cornerPollLength, 100, 100);
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
	glDisable(GL_TEXTURE_2D);

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

void renderBase() {

	glPushMatrix();
	
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(25,0,25);
	glTexCoord2f(1.0, 0.0); glVertex3f(-25,0, 25);
	glTexCoord2f(1.0, 1.0); glVertex3f(-25,0, -25);
	glTexCoord2f(0.0, 1.0); glVertex3f(25,0, -25);
	glEnd();

	
	glPopMatrix();
}

void renderLaunchStage() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, concreteTex);
	glPushMatrix();

	//front slant
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);glVertex3f(-5, 0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(-5, 0, -6);
	glTexCoord2f(1.0, 1.0); glVertex3f(-15, -4, -6);
	glTexCoord2f(0.0, 1.0);glVertex3f(-15, -4, 10);

	


	glEnd();

	//side1
	glBegin(GL_QUADS);
	glColor3f(1,1,1);
	glTexCoord2f(0.0, 0.0);glVertex3f(4, 0, 10);
	glTexCoord2f(1.0, 0.0);glVertex3f(-5, 0, 10);
	glTexCoord2f(1.0, 1.0);glVertex3f(-5, -4, 25);
	glTexCoord2f(0.0, 1.0);glVertex3f(4, -4, 25);
	glEnd();

	
	
	
	

	//side2
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0.0, 0.0);glVertex3f(-5, 0, -6);
	glTexCoord2f(1.0, 0.0);glVertex3f(4, 0, -6);
	glTexCoord2f(1.0, 1.0);glVertex3f(4, -4, -25);
	glTexCoord2f(0.0, 1.0);glVertex3f(-5, -4, -25);
	glEnd();
	

	//slant side1
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.5, 0.5);
	glTexCoord2f(0.0, 0.0);glVertex3f(-5, 0, 10);
	glTexCoord2f(1.0, 0.0);glVertex3f(-15, -4, 10);
	glTexCoord2f(1.0, 1.0);glVertex3f(-5, -4, 25);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, -4, 25);
	glEnd();



	//slant side2
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.5, 0.5);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5, 0, -6);
	glTexCoord2f(1.0, 0.0); glVertex3f(-15, -4, -6);
	glTexCoord2f(1.0, 1.0); glVertex3f(-5, -4, -25);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, -4, -25);
	glEnd();

	//vertical side1
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.5, 0.5);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5, 0, -6);
	glTexCoord2f(1.0, 0.0); glVertex3f(-15, -4, -6);
	glTexCoord2f(1.0, 1.0); glVertex3f(-5, -4, -25);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, -4, -25);
	glEnd();

	//vertical side2
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.5, 0.5);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5, 0, -6);
	glTexCoord2f(1.0, 0.0); glVertex3f(-15, -4, -6);
	glTexCoord2f(1.0, 1.0); glVertex3f(-5, -4, -25);
	glTexCoord2f(0.0, 1.0); glVertex3f(-15, -4, -25);
	glEnd();

	//back slant side1
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.1, 0.5);
	glTexCoord2f(0.0, 0.0); glVertex3f(4, 0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, -4, 25);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, -4, 10);
	glTexCoord2f(0.0, 1.0); glVertex3f(10, -4, 25);
	glEnd();

	//back slant side2
	glBegin(GL_QUAD_STRIP);
	glColor3f(1, 0.1, 0.5);
	glTexCoord2f(0.0, 0.0); glVertex3f(4, 0, -6);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, -4, -25);
	glTexCoord2f(1.0, 1.0); glVertex3f(10, -4, -6);
	glTexCoord2f(0.0, 1.0); glVertex3f(10, -4, -25);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5, 0.1, 0.5);
	glTexCoord2f(0.0, 0.0); glVertex3f(10, -4, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, 0, 10);
	glTexCoord2f(1.0, 1.0); glVertex3f(4, -4, 10);
	glTexCoord2f(0.0, 1.0); glVertex3f(-5, 0, 10);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5, -4, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(-5, 0, -6);
	glTexCoord2f(1.0, 1.0); glVertex3f(-5, -4, -6);
	glTexCoord2f(0.0, 1.0); glVertex3f(4, 0, -6);
	glTexCoord2f(0.0, 0.0); glVertex3f(4, -4, -6);
	glTexCoord2f(1.0, 0.0); glVertex3f(10, -4, -6);
	glEnd();
	

	GLfloat elevation = 0.5f;
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);

	//side 1
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5, 0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(-5, -elevation, 10);
	glTexCoord2f(1.0, 1.0); glVertex3f(4, -elevation, 10);
	glTexCoord2f(0.0, 1.0); glVertex3f(4, 0, 10);


	//back
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(4, 0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, -elevation, 10);
	glTexCoord2f(1.0, 1.0); glVertex3f(4, -elevation, -6);
	glTexCoord2f(0.0, 1.0); glVertex3f(4, 0, -6);

	//side 2
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(4, 0, -6);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, -elevation, -6);
	glTexCoord2f(1.0, 1.0); glVertex3f(-5, -elevation, -6);
	glTexCoord2f(0.0, 1.0); glVertex3f(-5, 0, -6);

	//front
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5, 0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(-5, 0, -6);
	glTexCoord2f(1.0, 1.0); glVertex3f(-5, -elevation, -6);
	glTexCoord2f(0.0, 1.0); glVertex3f(-5, -elevation, 10);

	//top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5, 0, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, 0, 10);
	glTexCoord2f(1.0, 1.0); glVertex3f(4, 0, -6);
	glTexCoord2f(0.0, 1.0); glVertex3f(-5, 0, -6);

	//bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-5, -elevation, 10);
	glTexCoord2f(1.0, 0.0); glVertex3f(4, -elevation, 10);
	glTexCoord2f(1.0, 1.0); glVertex3f(4, -elevation, -6);
	glTexCoord2f(0.0, 1.0); glVertex3f(-5, -elevation, -6);

	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}

void drawO2Tank() {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, offWhiteTex);
	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);

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

	glDisable(GL_TEXTURE_2D);
}

void drawO2TankPillars() {
	
	GLfloat towerPillarHeight = 10;
	GLfloat towerPillarDistance = 3;
	GLfloat towerPillarLeanAngle = 9;
	GLfloat mainPipeRadius = 0.5;
	GLfloat supportPillarRadius = 0.20;

	

	glRotatef(90, -1, 0, 0); 
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, offWhiteTex);
	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	//center pipe
	gluCylinder(qobj, 0.5, 0.5, towerPillarHeight, 100, 100);
	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, redPillarTex);
	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
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
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, offGreyTex);
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
	glDisable(GL_TEXTURE_2D);
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

	GLfloat topAntennaeStageHeight = 3;
	GLfloat topAntennaeStagePillarRadius = 0.1;
	GLfloat stageRadius = 2;
	GLfloat antennaeRadius = 0.5;
	GLfloat antennaeHeight = 7;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, redPillarTex);
	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
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

	//vertical pillars top antannae
	glPushMatrix();
	glTranslatef(0, 0, comTowerHeight);
	for (float j = 0; j < 360; j += 45) {
		glRotatef(45, 0, 0, 1);
		glPushMatrix();
		glTranslatef(stageRadius - topAntennaeStagePillarRadius, 0, 0);
		gluCylinder(qobj, topAntennaeStagePillarRadius, topAntennaeStagePillarRadius, topAntennaeStageHeight, 100, 100);
		glPopMatrix();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, whiteMetalTex);
	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	//cylindrical circles
	glPushMatrix();

	for (int i = 3; i < comTowerHeight;i+=3) {
		glTranslatef(0,0,3);
		glutSolidTorus(0.15,comTowerDistanceFromOrigin + comTowerPillarRadius,100,100);
	}
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	

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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, whiteMetalTex);
	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	//circular pillars
	glPushMatrix();
	for (int i = 0; i < topAntennaeStageHeight;i+=1) {
		glTranslatef(0,0,1);
		glutSolidTorus(topAntennaeStagePillarRadius,stageRadius + topAntennaeStagePillarRadius,100,100);
	}
	glPopMatrix();

	

	//centre antennae
	glPushMatrix();
	gluCylinder(qobj, antennaeRadius, antennaeRadius, antennaeHeight,100,100);

	glPopMatrix();
	
	glEnable(GL_TEXTURE_2D);
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

	renderBase();

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