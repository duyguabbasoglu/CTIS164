/*
NAME SURNAME : Duygu Abbasoglu
STUDENT ID : 22102978
SECTION : 03
HOMEWORK : #1
*My project displays a crab nearby the seaside

PROBLEMS :
ADDITIONAL FEATURES :
* I had a crab as the complex object and I used more than required shapes with every detail
* when you left-click the sky, day changes with color & features
* it has 2 theme;
* 1- in the morning theme, light sky & sun has different-angle rises, and there are 2 clouds which have different paces with direction-changable root
* 2- in the night theme, dark sky & stars moving with different random speeds
* adding the exit button to the program (press esc)
*
* And all necessary directions has given to the user as a display message (w, space, esc and click)
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

int WINDOW_WIDTH = 800;
int  WINDOW_HEIGHT = 600;
#define TIMER_PERIOD  32 // Period for the timer.
#define TIMER_ON         0 // 0:Disable timer, 1:Enable timer
#define D2R 0.017453292
#define PI 3.141592654

#define MAX_STARS 250

// I wanted to use structures from CTIS152 course
typedef struct {

	float x, y; // star's positions
	float speed; // movement speed
}star_t;

bool up = false, down = false, right = false, left = false;

float crabx = -300; // the crab's current pos
float crabspeed = 2.0; // speed of the crab 


//cloud initial positions and speeds 
double cloud1 = 0, cloud2 = -500, cloudSpeed1 = -1, cloudSpeed2 = 1.3;

int  winWidth, winHeight; // current Window width and height
bool timerActivator = false;
bool crabmoving = true;
bool isnight = false; // the animation starts at morning
bool windowOpen = false; // window will start with close pos

star_t stars[MAX_STARS];

void initstars() {

	srand(time(NULL));

	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].x = rand() % WINDOW_WIDTH - WINDOW_WIDTH / 2;
		stars[i].y = rand() % (WINDOW_HEIGHT / 2) + WINDOW_HEIGHT / 4; // to show up the stars in the upper-half of the window
		stars[i].speed = (rand() % 50) / 50.0 + 0.5; // now, stars have different speed
	}
}

void circle(int x, int y, int r) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i += 3)
		glVertex2f(x + r * cos(i * D2R), y + r * sin(i * D2R));
	glEnd();
}

void drawcrab() {

	glColor3f(1.0, 0.0, 0.0); // red
	glPushMatrix();
	glTranslatef(crabx, -270, 0); //the location of the crab

	// 1: body
	glBegin(GL_QUADS);
	glVertex2f(-30, 0);
	glVertex2f(30, 0);
	glVertex2f(20, 20);
	glVertex2f(-20, 20);

	glEnd();

	// 2. right claw
	glBegin(GL_TRIANGLES);
	glVertex2f(20, 10);
	glVertex2f(40, 20);
	glVertex2f(40, 0);

	glEnd();

	// 3. left claw
	glBegin(GL_TRIANGLES);
	glVertex2f(-20, 10);
	glVertex2f(-40, 20);
	glVertex2f(-40, 0);

	glEnd();

	// 4. eyes
	glColor3f(1.0, 0.0, 0.0);// red
	circle(15, 25, 7); // right eye
	circle(-15, 25, 7); // left eye

	// 5. iris
	glColor3f(1.0, 1.0, 1.0);// white
	circle(15, 25, 5); // right eye
	circle(-15, 25, 5); // left eye

	// 6. eye color
	glColor3f(0.0, 0.0, 0.0); // black
	circle(15, 25, 2); // right eye
	circle(-15, 25, 2); // left eye

	// 7. mouth
	glColor3f(0.0, 0.0, 0.0); // black
	glBegin(GL_LINES);
	glVertex2f(10, 10);
	glVertex2f(-10, 10);
	glEnd();

	// 8. left tooth (smaller)
	glColor3f(1.0, 1.0, 1.0); // white
	glBegin(GL_QUADS);
	glVertex2f(-5, 10);
	glVertex2f(-1, 10);
	glVertex2f(-1, 7);
	glVertex2f(-5, 7);
	glEnd();

	// 9. right tooth (bigger)
	glBegin(GL_QUADS);
	glVertex2f(0, 10);
	glVertex2f(5, 10);
	glVertex2f(5, 6);
	glVertex2f(0, 6);
	glEnd();

	// 10. feet
	glColor3f(0.7, 0.0, 0.0); // dark red

	// draw both of them with a loop instead of more coding (this was the hardest one)
	glBegin(GL_LINES);
	for (int i = -1; i <= 1; i += 2) {
		glVertex2f(i * 10, 0);
		glVertex2f(i * 15, -20);
		glVertex2f(i * 20, 0);
		glVertex2f(i * 25, -20);
	}

	glEnd();
	glPopMatrix();
}

void drawsun() {
	glColor3f(1.0, 1.0, 0.0); // yellow
	circle(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 3, 40); // sun

	 // rises
	for (int i = 0; i < 360; i += 30) {

		glBegin(GL_LINES);
		glVertex2f(WINDOW_WIDTH / 4 + 40 * cos(i * D2R), WINDOW_HEIGHT / 3 + 40 * sin(i * D2R));
		glVertex2f(WINDOW_WIDTH / 4 + 60 * cos(i * D2R), WINDOW_HEIGHT / 3 + 60 * sin(i * D2R));
		glEnd();
	}
}

void drawstars() {

	glColor3f(1.0, 1.0, 1.0); // white

	for (int i = 0; i < MAX_STARS; i++)
		circle(stars[i].x, stars[i].y, 2);
}

void drawcloud(double xOffset, double yOffset) {
	glColor3f(1.0, 1.0, 1.0); // white

	for (int i = 0; i < 4; i++) {
		circle(WINDOW_WIDTH / 4.5 + i * 17 + xOffset, WINDOW_HEIGHT / 3 + yOffset, WINDOW_WIDTH / 80);
		circle(WINDOW_WIDTH / 4.6 + i * 20 + xOffset, WINDOW_HEIGHT / 3.37 + yOffset, WINDOW_WIDTH / 50);
	}
	for (int i = 1; i < 3; i++) {
		circle(WINDOW_WIDTH / 4.5 + i * 17 + xOffset, WINDOW_HEIGHT / 3 + yOffset, WINDOW_WIDTH / 50);
		circle(WINDOW_WIDTH / 4.5 + i * 17 + xOffset, WINDOW_HEIGHT / 3.7 + yOffset, WINDOW_WIDTH / 50);
	}
}

void drawsky() {

	if (isnight)
		glColor3f(0.1, 0.1, 0.3); // dark blue
	else
		glColor3f(0.5, 0.7, 1); // light blue

	glBegin(GL_QUADS);
	glVertex2f(-WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	glVertex2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	glVertex2f(WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 3);
	glVertex2f(-WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 3);
	glEnd();
}

void drawsand() {
	glColor3f(0.9, 0.8, 0.6); // sand color
	glBegin(GL_QUADS);
	glVertex2f(-WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2);
	glVertex2f(WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2);
	glVertex2f(WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 3);
	glVertex2f(-WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 3);
	glEnd();
}

void drawocean() {
	int waveCount = 10;
	float waveHeight = 15;
	float waveLength = WINDOW_WIDTH / (float)waveCount;

	glColor3f(0.0, 0.0, 0.8); // dark blue
	glBegin(GL_QUADS);
	glVertex2f(-WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 6); // Adjust based on your scene setup
	glVertex2f(WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 6);
	glVertex2f(WINDOW_WIDTH / 2, 0); // Lower bound of the ocean
	glVertex2f(-WINDOW_WIDTH / 2, 0);
	glEnd();

	//wawes
	glColor3f(0.1, 0.1, 0.7); // light blue
	glBegin(GL_QUAD_STRIP);
	for (float x = -WINDOW_WIDTH / 2; x < WINDOW_WIDTH / 2; x += 20.0f) {
		float y = sin(x * 2 * PI / waveLength) * waveHeight / 2 - WINDOW_HEIGHT / 12;
		glVertex2f(x, y);
		glVertex2f(x, -WINDOW_HEIGHT / 6);
	}
	glEnd();
}

void drawwindow() {

	glColor3f(0.3, 0.3, 0.3); // dark gray

	int windowTopLeftX = -WINDOW_WIDTH / 4;
	int windowTopLeftY = WINDOW_HEIGHT / 4;
	int windowWidth = WINDOW_WIDTH / 6;
	int windowHeight = WINDOW_HEIGHT / 8;
	int barThickness = WINDOW_WIDTH / 200;

	// frame
	glBegin(GL_QUADS);
	glVertex2f(windowTopLeftX, windowTopLeftY);
	glVertex2f(windowTopLeftX + windowWidth, windowTopLeftY);
	glVertex2f(windowTopLeftX + windowWidth, windowTopLeftY - windowHeight);
	glVertex2f(windowTopLeftX, windowTopLeftY - windowHeight);
	glEnd();

	glColor3f(0.1, 0.1, 0.1); // darker gray
	float barWidth = WINDOW_WIDTH / 200;

	if (windowOpen) {
		glColor3f(0.5, 0.5, 0.8); // light purple

		// glass
		glBegin(GL_QUADS);
		glVertex2f(windowTopLeftX + barThickness, windowTopLeftY - barThickness);
		glVertex2f(windowTopLeftX + windowWidth - barThickness, windowTopLeftY - barThickness);
		glVertex2f(windowTopLeftX + windowWidth - barThickness, windowTopLeftY - windowHeight + barThickness);
		glVertex2f(windowTopLeftX + barThickness, windowTopLeftY - windowHeight + barThickness);
		glEnd();
	}

	// + shaped cross
	// vertical bar
	glBegin(GL_QUADS);
	glVertex2f(windowTopLeftX + windowWidth / 2 - barThickness, windowTopLeftY);
	glVertex2f(windowTopLeftX + windowWidth / 2 + barThickness, windowTopLeftY);
	glVertex2f(windowTopLeftX + windowWidth / 2 + barThickness, windowTopLeftY - windowHeight);
	glVertex2f(windowTopLeftX + windowWidth / 2 - barThickness, windowTopLeftY - windowHeight);
	glEnd();

	// horizontal bar
	glBegin(GL_QUADS);
	glVertex2f(windowTopLeftX, windowTopLeftY - windowHeight / 2 + barThickness);
	glVertex2f(windowTopLeftX + windowWidth, windowTopLeftY - windowHeight / 2 + barThickness);
	glVertex2f(windowTopLeftX + windowWidth, windowTopLeftY - windowHeight / 2 - barThickness);
	glVertex2f(windowTopLeftX, windowTopLeftY - windowHeight / 2 - barThickness);
	glEnd();

}

// Display text with variables
void vprint(int x, int y, void* font, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, str[i]);
}

void displaythebackground() {

	glColor3f(1.0, 1.0, 1.0); // black
	vprint(-WINDOW_WIDTH / 25, WINDOW_HEIGHT / 2.2, GLUT_BITMAP_HELVETICA_18, "Duygu Abbasoglu");
	vprint(-WINDOW_WIDTH / 50, WINDOW_HEIGHT / 2.4, GLUT_BITMAP_HELVETICA_18, "22102978");

	glColor3f(0.0, 0.7, 0.0); // light green
	vprint(WINDOW_WIDTH / 2 - 300, -WINDOW_HEIGHT / 2 + 100, GLUT_BITMAP_HELVETICA_18, "Press 'CLICK' to change the sky");
	vprint(WINDOW_WIDTH / 2 - 300, -WINDOW_HEIGHT / 2 + 70, GLUT_BITMAP_HELVETICA_18, "Press 'SPACE' to stop the crab");
	vprint(WINDOW_WIDTH / 2 - 300, -WINDOW_HEIGHT / 2 + 40, GLUT_BITMAP_HELVETICA_18, "Press 'w' to open the window");
	vprint(WINDOW_WIDTH / 2 - 300, -WINDOW_HEIGHT / 2 + 10, GLUT_BITMAP_HELVETICA_18, "Press 'ESC' to quit");
}

void display() {

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawsky();
	drawocean();
	drawsand();

	if (isnight)
		drawstars();

	else {
		drawcloud(cloud1, 0);
		drawcloud(cloud2, -1 * WINDOW_HEIGHT / 9);
		drawsun();
	}

	drawwindow();
	drawcrab();

	displaythebackground();
	glutSwapBuffers();
}

//ontimer event
void update(int value) {

	if (crabmoving)
		crabx += crabspeed; // moving the crab

	if (crabx > WINDOW_WIDTH / 2)
		crabx = -WINDOW_WIDTH / 2; // to continue after finishing

	cloud1 += cloudSpeed1;
	cloud2 += cloudSpeed2;

	if (cloud1 < -1 * WINDOW_WIDTH || cloud1 > WINDOW_WIDTH / 3)
		cloudSpeed1 *= -1;

	if (cloud2 < -1 * WINDOW_WIDTH || cloud2 > WINDOW_WIDTH / 3)
		cloudSpeed2 *= -1;

	for (int i = 0; i < MAX_STARS; i++) {
		stars[i].x += stars[i].speed;

		if (stars[i].x > WINDOW_WIDTH / 2)
			stars[i].x = -WINDOW_WIDTH / 2;

		else if (stars[i].x < -WINDOW_WIDTH / 2)
			stars[i].x = WINDOW_WIDTH / 2;
	}

	glutPostRedisplay();
	glutTimerFunc(TIMER_PERIOD, update, 0); // call the function periodical
}

// Key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
void onKeyDown(unsigned char key, int x, int y) {
	// Exit when ESC is pressed
	if (key == 27)
		exit(0);

	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y) {
	// Exit when ESC is pressed
	if (key == 27)
		exit(0);

	if (key == ' ')
		crabmoving = !crabmoving; // press space to pause or resume the crab

	if (key == 'w' || key == 'W')
		windowOpen = !windowOpen; // window event

	glutPostRedisplay();
}

void onSpecialKeyDown(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		up = true;
		break;
	case GLUT_KEY_DOWN:
		down = true;
		break;
	case GLUT_KEY_LEFT:
		left = true;
		break;
	case GLUT_KEY_RIGHT:
		right = true;
		break;
	}
	// To refresh the window it calls display() function
	glutPostRedisplay();
}

// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
void onSpecialKeyUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		up = false;
		break;
	case GLUT_KEY_DOWN:
		down = false;
		break;
	case GLUT_KEY_LEFT:
		left = false;
		break;
	case GLUT_KEY_RIGHT:
		right = false;
		break;
	}
	// To refresh the window it calls display() function
	glutPostRedisplay();
}

void onClick(int button, int stat, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
		isnight = !isnight; // when you press click it will turn

	glutPostRedisplay();
}

// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
void onResize(int w, int h) {
	WINDOW_WIDTH = w;
	WINDOW_HEIGHT = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // Refresh window.
}

void onMoveDown(int x, int y) {
	// Write your code here

	// To refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your code here

	// To refresh the window it calls display() function
	glutPostRedisplay();
}

void Init() {
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char* argv[]) {

	// window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("The Crab Nearby The Ocean - Duygu ABBASOGLU");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	// Keyboard Events
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	// Mouse Events
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// Timer Event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif
	Init();
	initstars();
	glutTimerFunc(TIMER_PERIOD, update, 0); // to start the animation
	glutMainLoop();
	return 0;
}