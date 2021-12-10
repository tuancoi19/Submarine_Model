#include <iostream>
#include <string.h>
#include <math.h>
#include <time.h>
#include <fstream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <windows.h>
using namespace std;
static GLuint m_texture[5];
static float speed = 0.0;
static float worldX = 0;
static float worldY = 0.0;
static float scaleFactor = 0.5;
static bool animate = false;
static float updown = 0.0;
static float updowns = 0.0;
static float leftright = 0.0;
static float leftrightl = 0.0;
static float updownl = 0.0;

void Idle()
{
	if (animate){
		speed += 1.5;
		updowns += 0.005;
	}
	if (!animate) {
		leftrightl = 0.0;
		updownl = 0.0;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y){
	switch (key)
	{
	case 'q': exit(0);
		break;
	case 'w': worldX -= 1.0f;
		break;
	case 's': worldX += 1.0f;
		break;
	case 'd': worldY -= 1.0f;
		break;
	case 'a': worldY += 1.0f;
		break;
	case '-': scaleFactor -= 0.1;
		break;
	case '+': scaleFactor += 0.1;
		break;
	case '8': 
		updown += 0.5f;
		updownl += 0.5;
		if(updownl > 90)
			updownl = 90;
		break;         
	case '2': 
		updown -= 0.5f;
		updownl -= 0.5f;
		if(updownl < -90)
			updownl = -90;  
		break;          
	case '4': 
		leftright += 1.0f;
		leftrightl += 1.0f;
		if(leftrightl > 90)
			leftrightl = 90;
		break;         
	case '6': 
		leftright -= 1.0f;
		leftrightl -= 1.0f;
		if(leftrightl < -90)
			leftrightl = -90;
		break;          
	case ' ': animate = !animate;;
		break;
	default: break;
	}
	if (leftrightl == 90 || leftrightl == -90) {
		Sleep(1);
  		leftrightl = 0;
  	}
  	if (updownl == 90 || updownl == -90) {
		Sleep(1);
	  	updownl = 0;
	}
}

void fill(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess, GLfloat lowAmbient[]) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_LIGHT0, GL_AMBIENT, lowAmbient);		
}

GLfloat qaBlack[] = { 0,0,0,1.0 };
GLfloat qaGreen[] = { 0,1.0,0,1.0 };
GLfloat qaRed[] = { 1.0,0,0,1.0 };
GLfloat qaBlue[] = { 0,0,1.0,1.0 };
GLfloat qaWhite[] = { 1.0,1.0,1.0,1.0 };
GLfloat qaYellow[] = { 1.0,1.0,0,1.0 };
GLfloat qaGrey[] = { 0.5,0.5,0.5,0.5 };
GLfloat qaLowAmbient[] = { 0.2,0.2,0.2,1.0 };
GLfloat qaFullAmbient[] = { 1.0,1.0,1.0,1.0 };
bool night = 0;

void LoadTexture(const char* filename, int texName) {
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, m_texture[texName]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void LoadGLTextures() {
	glGenTextures(4, m_texture);
	LoadTexture("3.jpg", 0);
	LoadTexture("2.jpg", 1);
	LoadTexture("1.jpg", 2);
	LoadTexture("4.png", 3);
}

void axis()
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-15.0, 0.0, 0.0);
	glVertex3f(15.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -15.0, 0.0);
	glVertex3f(0.0, 15.0, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, 0.0, -15.0);
	glVertex3f(0.0, 0.0, 15.0);
	glEnd();

}

void MakeBox(const float length, const float width, const float height)
{
	float x = length;
	float y = height;
	float z = width;

	//Back
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);
	glVertex3f(0, y, 0);
	glEnd();

	// left
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, z);
	glVertex3f(0, y, z);
	glVertex3f(0, y, 0);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 0, z);
	glVertex3f(0, y, z);
	glVertex3f(x, y, z);
	glVertex3f(x, 0, z);
	glEnd();

	// right
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x, 0, z);
	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, z);
	glEnd();

	//Top
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, y, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, z);
	glVertex3f(0, y, z);
	glEnd();

	//Bottom
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, 0, z);
	glVertex3f(0, 0, z);
	glEnd();
}

void MakeSphere(const float x){

	GLUquadricObj* quadric;
    quadric = gluNewQuadric();
    gluSphere(quadric, x, 36, 18);
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluDeleteQuadric(quadric);
}

void MakeCylinder(const float& radius, const float& length)
{
	GLUquadricObj* quadratic_obj;
	quadratic_obj = gluNewQuadric();
	gluCylinder(quadratic_obj, radius, radius, length, 32, 32);
}

void MakeTruncatedCone(const float& base_rad, const float & top_rad, const float& length)
{
	GLUquadricObj *quadratic_obj;
	quadratic_obj = gluNewQuadric();
	gluCylinder(quadratic_obj, base_rad, top_rad, length, 32, 32);
}

void XOY() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.00, 0.0);
	glVertex3f(-150, -150, 0);
	glTexCoord2f(1.00, 0.0);
	glVertex3f(150, -150, 0);
	glTexCoord2f(1.00, 1.0);
	glVertex3f(150, 150, 0);
	glTexCoord2f(0.00, 1.0);
	glVertex3f(-150, 150, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void YOZ() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.00, 0.0);
	glVertex3f(0, -150, -150);
	glTexCoord2f(1.00, 0.0);
	glVertex3f(0, 150, -150);
	glTexCoord2f(1.00, 1.0);
	glVertex3f(0, 150, 150);
	glTexCoord2f(0.00, 1.0);
	glVertex3f(0, -150, 150);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void XOZ() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.00, 0.0);
	glVertex3f(-150, 0, -150);
	glTexCoord2f(1.00, 0.0);
	glVertex3f(150, 0, -150);
	glTexCoord2f(1.00, 1.0);
	glVertex3f(150, 0, 150);
	glTexCoord2f(0.00, 1.0);
	glVertex3f(-150, 0, 150);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void BackGround() {
	
	glPushMatrix();
	glTranslatef(0, 25, 150);
	glRotatef(180, 0, 0, 1);
	XOY();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-150, 25, 0);
	glRotatef(90, 1, 0, 0);
	YOZ();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(150, 25, 0);
	glRotatef(90, 1, 0, 0);
	YOZ();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -125, 0);
	XOZ();
	glPopMatrix();
}

void sea() {
	glPushMatrix();
	glColor4f(0, 1, 1, 0.2);
    glTranslatef(-150, -125, -150);
    MakeBox(300, 300, 300);
	glPopMatrix();
}

void flag(const float length, const float width, const float height) {
	float x = length;
	float y = height;
	float z = width;

	//Back
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[3]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x, 0, 0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x, y, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0, y, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// left
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, z);
	glVertex3f(0, y, z);
	glVertex3f(0, y, 0);
	glEnd();

	//front
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture[3]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0, 0, z);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0, y, z);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x, y, z);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x, 0, z);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// right
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(x, 0, z);
	glVertex3f(x, 0, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, z);
	glEnd();

	//Top
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, y, 0);
	glVertex3f(x, y, 0);
	glVertex3f(x, y, z);
	glVertex3f(0, y, z);
	glEnd();

	//Bottom
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(x, 0, 0);
	glVertex3f(x, 0, z);
	glVertex3f(0, 0, z);
	glEnd();
}

void SubMarine() {
	glRotatef(90, 0, 0, 1);
		
	glPushMatrix();
	glTranslatef(2.5, 53.5, 2.5);
	glColor3f(1, 1, 1);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(2, 0, 32, 32);
	glPopMatrix(); 
	
	//Right
	glPushMatrix();
	glTranslatef(3.25, 51, 2.25);
	glColor3f(1, 1, 1);
	glRotatef(-90, 0, 1, 0);
	glRotatef(speed, 0, 1, 0);
	MakeBox(4, 0.75, 3);
	glPopMatrix();
	
	//Up
	glPushMatrix();
	glTranslatef(2.75, 51, 2);
	glColor3f(1, 1, 1);
	glRotatef(0, 0, 1, 0);
	glRotatef(speed, 0, 1, 0);
	MakeBox(4, 0.75, 3);
	glPopMatrix();
	
	//Down
	glPushMatrix();
	glTranslatef(3, 51, 2.75);
	glColor3f(1, 1, 1);
	glRotatef(180, 0, 1, 0);
	glRotatef(speed, 0, 1, 0);
	MakeBox(4, 0.75, 3);
	glPopMatrix();
	
	//Left
	glPushMatrix();
	glTranslatef(2.5, 51, 2.5);
	glColor3f(1, 1, 1);
	glRotatef(90, 0, 1, 0);
	glRotatef(speed, 0, 1, 0);
	MakeBox(4, 0.75, 3);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(2.5, 51, 2.5);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	MakeCylinder(5, 3);
	glPopMatrix();
		
	glPushMatrix();
	glTranslatef(2.5, 46, 2.5);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	MakeTruncatedCone(2, 5, 5);
	glPopMatrix();
		
	glPushMatrix();
	glTranslatef(2.5, 46, 2.5);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	MakeCylinder(2, 7);
	glPopMatrix(); 
	
	glPushMatrix();
	glTranslatef(2.5, 40, 2.5);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	MakeSphere(7);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(2.85, 35, -10);
	glColor3f(1, 0.1, 0.1);
	glRotatef(-90, 0, 1, 0);
	glRotatef(updownl, 1, 0, 0);
	MakeBox(25, 0.75, 3);
	glPopMatrix();
	  	
	glPushMatrix();
	glTranslatef(14.5, 35, 2.85);
	glColor3f(1, 0, 0);
	glRotatef(180, 0, 1, 0);
	glRotatef(leftrightl, 1, 0, 0);
	MakeBox(25, 0.75, 3);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(15, 22, 2.5);
	glRotatef(0, 0, 1, 0);
	glRotatef(leftrightl, 1, 0, 0);	
	flag(8, 0.1, 10);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(5, 22, 2.5);
	glColor3f(1, 1, 1);
	glRotatef(90, 0, 1, 0);
	MakeCylinder(0.5, 18);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(5, 28, 3.5);
	glColor3f(1, 0, 0);
	glRotatef(180, 1, 0, 0);
	MakeBox(8, 2, 8);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(2.5, 10, 2.5);
	glColor3f(1, 1, 1);
	glRotatef(-90, 1, 0, 0);
	MakeCylinder(7, 30);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(2.5, 10, 2.5);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	MakeSphere(7);
	glPopMatrix();
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	
	gluLookAt(0, 0, -80, 0, 0, 0, 0, 1, 0);
	
	glRotatef(worldX, 1, 0, 0);
	glRotatef(worldY, 0, 1, 0);
	
	glScalef(scaleFactor, scaleFactor, scaleFactor);
	
	axis();
	BackGround();
	
	glPushMatrix();
	glRotatef(leftright, 0, 1, 0);
	glRotatef(updown, 0, 0, 1);
	glTranslatef(updowns, 0, 0);
	SubMarine();
	glPopMatrix();
	
	sea();
	glutSwapBuffers();
	glFlush();

}

void ReShape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float)width / (float)height;
	gluPerspective(45.0, ratio, 1, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void init(){
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	GLfloat light_position[] = { 25.0, 50.0, 5.0, 1 };
	GLfloat light_diff[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_amb[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_specular[] = { 0.5, 0.5 ,0.5 ,1.0 };

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	LoadGLTextures();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1360, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D SubMarine");
	init();
	glutReshapeFunc(ReShape);
	glutDisplayFunc(MyDisplay);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
	return 0;
}
