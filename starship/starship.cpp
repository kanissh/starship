#include<windows.h>
#include<GL/glut.h>

int refreshMillis = 30;

void  init() {
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); 
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,1.0);
}

void display() {
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBegin(GL_QUADS);              
	glColor3f(1.0f, 0.0f, 0.0f); 
	glVertex2f(-0.5f, -0.5f);    
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();

	glFlush();

	
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;

	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0, aspect, 0.1, 100.0);

	if (width <= height) {
		glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);  
	}
	else {
		glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0);  
	}
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(refreshMillis, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutCreateWindow("Starship");
	glutInitWindowSize(500,500);
	glutInitWindowPosition(50,50);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;
}