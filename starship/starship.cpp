#include<stdlib.h>
#include <iostream>
#include <GL/glut.h>  
#include <math.h>


using namespace std;

// vertices for the cube
GLfloat x = 2.0f;
GLfloat y = 2.0f;
GLfloat z = 2.0f;

// variables to move outermost Object Frame (to move all the rendered environment)
GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = 0.0f;

// variables to rotate outermost Object Frame (to move all the rendered environment)
GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;


// For animating the rotation of the objects
GLfloat animateRotation = 0.0f;

//variables to move the camera
GLfloat camY = 0.0f;
GLfloat camX = 0.0f;
GLfloat camZ = 0.0f;

// A quadratic object pointer used to draw the sides of the cylinder
GLUquadricObj* qobj;

//Starship dim
GLfloat starshipHeight = 10.0;


//Super heavy dim
GLfloat superHeavyHeight = 14.0;

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Inititalization of the quadratic object for the cylinder (one time inititalization is only needed ).
    // This can be used to draw any number of cylinders
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);

    // glPolygonMode(GL_FRONT, GL_LINE);
    // glPolygonMode(GL_BACK, GL_FILL);
    // glEnable(GL_CULL_FACE);

}

void DrawGrid() {
    GLfloat ext = 20.0f;
    GLfloat step = 1.0f;
    GLfloat yGrid = -0.4f;
    GLint line;

    glBegin(GL_LINES);
    for (line = -ext; line <= ext; line += step) {
        glVertex3f(line, yGrid, ext);
        glVertex3f(line, yGrid, -ext);

        glVertex3f(ext, yGrid, line);
        glVertex3f(-ext, yGrid, line);
    }
    glEnd();
}


void drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks) {
    glPushMatrix();
    // Triangle Fan for the Base
    /*glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, 0.0);
    for (float i = 0; i < 360; i += 1.2) {
        glVertex3f(base * cos(i), base * sin(i), 0.0);
        glVertex3f(base * cos(i + 0.2), base * sin(i + 0.2), 0.0);
    }
    glEnd();*/

    // Sides of the Cylinder
    gluCylinder(qobj, base, top, height, slices, stacks);

    // Triangle Fan for the Top
    glTranslatef(0.0, 0.0, height);

   /* glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, 0.0);
    for (float i = 0; i < 360; i += 0.2) {
        glVertex3f(top * cos(i), top * sin(i), 0.0);
        glVertex3f(top * cos(i + 0.2), top * sin(i + 0.2), 0.0);
    }
    glEnd();*/
    glPopMatrix();
}


void drawAxes() {

    glBegin(GL_LINES);
    glVertex3f(-2, 0, 0);
    glVertex3f(2, 0, 0);

    glVertex3f(0, -2, 0);
    glVertex3f(0, 2, 0);

    glVertex3f(0, 0, -2);
    glVertex3f(0, 0, 2);

    glEnd();
}


void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glPushMatrix();

    // camera orientation (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    gluLookAt(0.0 + camX, 2.0 + camY, 5.0 + camZ, 0, 0, 0, 0, 1.0, 0);

    // move the object frame using keyboard keys
    glTranslatef(moveX, moveY, moveZ);
    glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotZ, 0.0f, 0.0f, 1.0f);


    glColor3f(1.0, 1.0, 1.0);

    //DrawGrid();

    //super heavy
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    
    glRotatef(90, 1.0, 0.0, 0.0);
    
    drawCylinder(1, 1, superHeavyHeight, 100, 100);
    glPopMatrix();

    //starship
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.0,starshipHeight,0.0);

    glRotatef(90, 1.0, 0.0, 0.0);

    drawCylinder(1, 1, starshipHeight, 100, 100);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 10, 0);
    glBegin(GL_POLYGON);
    glColor3f(1,1,1);
   
    glVertex3f(-2,0,0);
    glVertex3f(2,0,0);
    glVertex3f(0.86,0.1,0);
    glVertex3f(0.86, 0.3, 0);
    glVertex3f(0,1.6,0);
    glVertex3f(-0.86, 0.3, 0);
    glVertex3f(-0.86, 0.1, 0);
    glVertex3f(-2, 0, 0);
    
   
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    
    glVertex3f(-2, 0, 0.05);
    glVertex3f(2, 0, 0.05);
    glVertex3f(0.86, 0.1, 0.05);
    glVertex3f(0.86, 0.3, 0.05);
    glVertex3f(0, 1.6, 0.05);
    glVertex3f(-0.86, 0.3, 0.05);
    glVertex3f(-0.86, 0.1, 0.05);
    glVertex3f(-2, 0, 0.05);


    glEnd();
    glPopMatrix();


    glPopMatrix();

    glutSwapBuffers();

}



void Timer(int x) {
    animateRotation += animateRotation >= 360.0 ? -animateRotation : 5;
    glutPostRedisplay();

    glutTimerFunc(60, Timer, 1);
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
    /*else if (key == 'c') {
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
    }*/
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
    glutCreateWindow("OpenGL Setup Test");
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(150, 150);
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    // keyboard function activation
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    init();
    glutTimerFunc(60.0, Timer, 1);
    glutMainLoop();


    return 0;
}
