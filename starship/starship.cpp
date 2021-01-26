#include <cmath>
#include <iostream>
#include <GL/glut.h>  

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


GLfloat starshipHeight = 8.3;
GLfloat superheavyHeight = 15;
GLfloat rocketRadius = 1;
GLUquadricObj* qobj;

GLfloat topConeHeight = 2.3;
GLfloat topConeTopRad = 0.2;
GLfloat topConeBottomRad = 1;
GLfloat topFinHeight = 3.5;
GLfloat bottomFinHeight = 4;

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
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
	GLfloat ext = 20.0f;
	GLfloat step = 1.0f;
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
	glTranslatef(0, starshipHeight + superheavyHeight + topConeHeight - topFinHeight, 0);
	glBegin(GL_POLYGON);
	
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

	drawStarship();
	drawSuperheavy();
	drawTopCone();
	drawStarshipTopFins();
	drawStarshipBottomFins();
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