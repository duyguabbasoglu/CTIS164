/***
NAME SURNAME : Duygu Abbasoglu
STUDENT ID : 22102978
SECTION : 03
HOMEWORK : #2
*My project displays a game simulation, rocket hits the harts and fight with monsters

PROBLEMS :
ADDITIONAL FEATURES:
    * 100 random star is created for the background of the game, their locations and sizes are different
    *
    * 5 hearts that have different speeds and directions added. If any of the hearts removed from the screen or land on the ground, a new heart is created

    *  If user cannot shoot a heart until it lands on the ground, an pacman is created at the position of the landed heart. They have different speeds and directions

    * Points are awarded for each successful shot, with a live score and remaining time displayed on the screen. The time is formatted in seconds and milliseconds to enhance the sense of urgency.
            An additional function is added to show the remaining milliseconds

    * If the number of pacman monsters reach 3, the game ends

    * In addition to up, down,left and right special keys, cannon weapon can move using 'a' and'd' keys

    * In addition to F1, 'r' key also restart the game even after the game starts

    * Before starting the game and after pausing the game, new bullets cannot be launched

    * Ammo is limited by 10 bullets. After the bullets finish, ammo is refilled

    * Collected points are counted until the game is over. Each succesful shot gains depend on the heart position, if it is on the center, it takes 1 points and increases with widht
    *
    * A proper beggining and ending messages are displayed at the rightside of the outer screen

    * Program terminates when ESC key is pressed

*****/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD   20 //period for the timer
#define TIMER_ON        1 //0:disable timer, 1:enable timer

#define D2R 0.0174532
#define PI 3.14159265358979323846


/* Global Variables for Template File */

bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; //current Window width and height

bool timeract = false;

bool active = false;
int counter = 0;


//game variables
bool  gameover = false, gamepaused = false;

//collecting points
char s[10] = "";
int point = 0;

float heart[5][4];
void inithearts() {

    for (int i = 0; i < 5; i++) {
        heart[i][0] = 1;
        heart[i][1] = rand() % 601 - 300;
        heart[i][2] = 450;
        heart[i][3] = (rand() % 5 + 4) / 50.0;
    }
}


float xt = -100, yt = -220, vt = 9; // cannon movement in each time

float pacmanmonster[3][4] = { {0, 25, -175, (rand() % 5 + 2) / 5.},
                      {0, 25, -175, (rand() % 5 + 2) / 5.},
                      {0, 25, -175, (rand() % 5 + 2) / 5.} }; //visibiliy, x, y, speed
int currentpacmanmonster = 0;

int bullet[10][3] = { {0},{0},{0},{0},{0},{0},{0},{0},{0},{0} }; //moving action, x, y, speed
int currentbullet = 0;
float vf = 7;
int deadbullet = 0;

// geometric shapes
void circle(int x, int y, int r) {
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void drawhalfcircle(int cx, int cy, int r) {
    glBegin(GL_POLYGON);
    for (int i = 180; i < 360; i++) {
        float theta = i * PI / 180;
        glVertex2f(cx + r * cos(theta), cy + r * sin(theta));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font) {
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, string[i]);
    }
}

/***
   to display text with variables
vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
***/
void vprint(int x, int y, void* font, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, str[i]);
    }
}

void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}




//showing remining millisec in each fps

int msec(int x) {
    if (x == 100) { return 00; }
    else { return x; }
}



// background

void drawgradienttheme(int x1, int y1, int w, int h, float r, float g, float b) {

    glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex2f(x1, y1);
    glVertex2f(x1 + w, y1);
    glColor3f(r + 0.7, g + 0.7, b + 0.7);
    glVertex2f(x1 + w, y1 - h);
    glVertex2f(x1, y1 - h);
    glEnd();

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x1 + w, y1);
    glVertex2f(x1 + w, y1 - h);
    glVertex2f(x1, y1 - h);
    glEnd();
}

void drawstars(int num) {
    srand(4);
    glColor3f(0.7, 0.7, 0.7);
    int x = 0, y = 0, r = 0;
    for (int i = 0; i < num; i++) {
        x = rand() % 801 - 400;
        y = rand() % 601 - 220;
        r = rand() % 2 + 0.4;
        circle(x, y, r);
    }
}

void resetcannon() {
    xt = 0;
    yt = -220;
    counter = 0; //reseting time period
    point = 0; //reseting point
}

void resetheart() {

    for (int i = 0; i < 5; i++) {

        heart[i][0] = rand() % 601 - 300;
        heart[i][2] = 450;
        heart[i][3] = (rand() % 5 + 4) / 60.0;
    }
}

void resetbullet() {
    currentbullet = 0;
    deadbullet = 0;
    for (int i = 0; i < 10; i++) {
        bullet[i][0] == 0;
        bullet[i][1] = xt;
        bullet[i][2] = yt;
    }
}

void resetpacmanmonster() {
    currentpacmanmonster = 0;
    for (int i = 0; i < 3; i++) {
        pacmanmonster[i][0] = 0;
    }
}


// DRAWING THE OBJECTS

void drawheart(int x, int y) {

    glColor3f(1, 0, 0); // red

    // upper circles
    circle(x - 15, y + 15, 15); // left
    circle(x + 15, y + 15, 15); // right

    // bottom side
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 30, y + 10);
    glVertex2f(x + 30, y + 10);
    glVertex2f(x, y - 30);
    glEnd();

    // highlighter
    glColor3f(1, 0.15, 0.15); // lighter red
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 30, y + 20);
    glVertex2f(x + 30, y + 20);
    glVertex2f(x, y - 10);
    glEnd();
}

void drawhearts() {
    for (int i = 0; i < 5; i++) {
        if (heart[i][0] == 1)
            drawheart(heart[i][1], heart[i][2]);

        else {

            heart[i][0] = 1;
            heart[i][2] = 350;
        }
    }
}


void drawbullet(int xf, int yf) {

    //outer circle
    glColor3f(1.0f, 0.5f, 0.0f); // orange
    circle(xf, yf, 13);

    // Draw inner circle
    glColor3f(1.0f, 1.0f, 0.0f); // yellow
    circle(xf, yf, 7);

}

void drawbullets() {
    for (int i = 0; i < 10; i++) {
        if (bullet[i][0] == 0) {
            bullet[i][1] = xt;
            bullet[i][2] = yt;
        }
        drawbullet(bullet[i][1], bullet[i][2]);
    }
}


void drawcannon() {

    // body
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(xt - 20, yt);
    glVertex2f(xt + 20, yt);
    glVertex2f(xt + 20, yt + 80);
    glVertex2f(xt - 20, yt + 80);
    glEnd();

    // upperside
    glColor3f(1.0, 1.0, 0.0); // yellow
    glBegin(GL_TRIANGLES);
    glVertex2f(xt - 20, yt + 80);
    glVertex2f(xt, yt + 100);
    glVertex2f(xt + 20, yt + 80);
    glEnd();

    // nearsides
    glColor3f(0.5, 0.5, 0.5); // gray

    // left
    glBegin(GL_QUADS);
    glVertex2f(xt - 20, yt + 20);
    glVertex2f(xt - 30, yt + 20);
    glVertex2f(xt - 30, yt + 60);
    glVertex2f(xt - 20, yt + 60);
    glEnd();

    // right
    glBegin(GL_QUADS);
    glVertex2f(xt + 20, yt + 20);
    glVertex2f(xt + 30, yt + 20);
    glVertex2f(xt + 30, yt + 60);
    glVertex2f(xt + 20, yt + 60);
    glEnd();
}

void drawpacmanmonster(int x, int y) {

    glColor3f(1, 1, 0); // yellow
    circle(x, y, 26);

    // eyes
    glColor3f(1, 1, 1); // white
    circle(x - 10, y + 10, 6);
    circle(x + 10, y + 10, 6);

    // iris
    glColor3f(0, 0, 0); // black
    circle(x - 10, y + 15, 3);
    circle(x + 10, y + 15, 3);

    // feet
    glColor3f(1, 1, 0);
    drawhalfcircle(x - 13, y - 26, 13);
    drawhalfcircle(x + 13, y - 26, 13);
    glBegin(GL_QUADS);
    glVertex2f(x - 26, y); // top left
    glVertex2f(x + 26, y); // top right
    glVertex2f(x + 26, y - 26); // bottom right
    glVertex2f(x - 26, y - 26); // bottom left
    glEnd();
}

void drawpacmanmonsters() {
    for (int i = 0; i < 3; i++)
        if (pacmanmonster[i][0] == 1)
            drawpacmanmonster(pacmanmonster[i][1], pacmanmonster[i][2]);
}


void drawBackground() {
    drawgradienttheme(-400, 300, 800, 600, 0, 37. / 255, 51. / 255);
    drawgradienttheme(-400, -240, 800, 50, 0, 0, 0); //base
    drawstars(100);


    //title
    glColor3f(1, 1, 1);
    print(-90, 275, "Rocket vs Pac-man", GLUT_BITMAP_TIMES_ROMAN_24);


    //bottom info panel
    glColor3f(0, 0, 0);
    glRectf(-400, -280, 400, -300);

    glColor3f(1, 0, 1); // purple
    print(300, -295, "Points: ", GLUT_BITMAP_HELVETICA_12);
    if (gameover)
        print(-100, -295, "<F1> to Start a New Game", GLUT_BITMAP_HELVETICA_12);
    else
        print(-100, -295, "<Spacebar> bullet,  <F1> Pause/Restart", GLUT_BITMAP_HELVETICA_12);

}

//remining time and point calculation

void displayStat() {

    glColor3f(1, 0, 0); // red
    //showing the points gained
    sprintf_s(s, "%d", point);
    print(340, -295, s, GLUT_BITMAP_HELVETICA_12);

    //showing remaining time
    print(200, 270, "Remaining Time: ", GLUT_BITMAP_HELVETICA_18);
    char t[10] = "";
    sprintf_s(t, "%d.%2d", (int)(20 - counter / 50.), msec(100 - (counter % 50) * 2));
    print(340, 270, t, GLUT_BITMAP_HELVETICA_18);

    // remaining time screen in outer screen
    print(800, 270, "Remaining Time: ", GLUT_BITMAP_TIMES_ROMAN_10);
    char timer[10] = "";
    sprintf_s(t, "%d.%2d", (int)(20 - counter / 50.), msec(100 - (counter % 50) * 2));
    print(840, 270, t, GLUT_BITMAP_TIMES_ROMAN_10);
}

void displaybegginingmessage() {

    if (!gamepaused) {
        drawgradienttheme(+450, 65, 300, 120, 0, 0, 204 / 255.);
        glColor3f(0, 1, 204 / 255.);
        print(+470, 40, "F1: Start / Pause", GLUT_BITMAP_HELVETICA_18);
        print(+470, 20, "A/Left Button: Move left", GLUT_BITMAP_HELVETICA_18);
        print(+470, 0, "D/Right Button: Move right", GLUT_BITMAP_HELVETICA_18);
        print(+470, -20, "Space: Launch missle", GLUT_BITMAP_HELVETICA_18);
        print(+470, -40, "R: Reset game", GLUT_BITMAP_HELVETICA_18);
    }

}
void displayendingmessage() {
    drawgradienttheme(-130, 60, 300, 100, 0, 0, 204 / 255.);

    glColor3f(1, 1, 0); // yellow
    print(-80, 0, "         SCORE ", GLUT_BITMAP_TIMES_ROMAN_24);
    print(0, -20, s, GLUT_BITMAP_TIMES_ROMAN_24);
    if (currentpacmanmonster == 3) {
        print(-100, 20, "   PAC MAN WON   ", GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else {
        print(-60, 20, "  TIME IS UP  ", GLUT_BITMAP_TIMES_ROMAN_24);
    }

}

void display() {
    //clear window to black
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    // adding display functions
    drawBackground();
    drawhearts();
    drawbullets();
    drawpacmanmonsters();
    drawcannon();
    displayStat();

    // regulate the game start and finish
    if (gameover) {
        displayendingmessage();
    }
    if (!timeract && !gameover)
        displaybegginingmessage();

    glutSwapBuffers();
}

/***
   key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
***/
void onKeyDown(unsigned char key, int x, int y) {
    //exit when ESC is pressed.

    switch (key) {

    case 27:
        exit(0);
        break;

    case 'r':
        gameover = false;
        resetcannon();
        resetbullet();
        resetheart();
        resetpacmanmonster();
        if (timeract) {
            timeract = !timeract;
        }
        break;

    case ' ':
        if (timeract) {
            if (currentbullet < 10) {
                bullet[currentbullet][0] = 1; //bullet moving 
                currentbullet++;
            }
        }
        break;

    case 'a':
        left = true;
        if (xt > -320 && !gameover) {
            xt -= vt;
        }
        break;

    case 'd':
        right = true;
        if (xt < 320 && !gameover) {
            xt += vt;
        }
        break;

    }
    //to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y) {
    //exit when ESC is pressed.
    if (key == 27)
        exit(0);

    //to refresh the window it calls display() function
    glutPostRedisplay();
}

/***
   special Key like GLUT_KEY_F1, F2, F3,...
   arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
***/
void onSpecialKeyDown(int key, int x, int y) {
    //write your codes here.

    switch (key) {

    case GLUT_KEY_UP:
        up = true;
        break;

    case GLUT_KEY_DOWN:
        down = true;
        break;

    case GLUT_KEY_LEFT:
        left = true;
        if (xt > -320 && !gameover) {
            xt -= vt;
        }
        break;

    case GLUT_KEY_RIGHT:
        right = true;
        if (xt < 320 && !gameover) {
            xt += vt;
        }
        break;

    case GLUT_KEY_F1:
        if (!gameover) {
            timeract = !timeract;
            gamepaused = !gamepaused;
        }
        else {
            gameover = false;
            resetcannon();
            resetbullet();
            resetheart();
            resetpacmanmonster();
            timeract = false;
            gamepaused = false;

        }
        break;
    }

    //to refresh the window it calls display() function
    glutPostRedisplay();
}

/***
   special Key like GLUT_KEY_F1, F2, F3,...
   arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
***/
void onSpecialKeyUp(int key, int x, int y) {
    //write your codes here

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

    //to refresh the window it calls display() function
    glutPostRedisplay();
}

/***
   When a click occurs in the window,
   It provides which button
   buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
   states  : GLUT_UP , GLUT_DOWN
   x, y is the coordinate of the point that mouse clicked
***/
void onClick(int button, int stat, int x, int y) {
    //write your codes here.
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {


    }

    //to refresh the window it calls display() function
    glutPostRedisplay();
}

/***
   This function is called when the window size changes.
   w : is the new width of the window in pixels
   h : is the new height of the window in pixels
***/
void onResize(int w, int h) {
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); //refresh window
}


void onTimer(int v) {
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    if (!gameover && timeract) {
        if (timeract) {
            counter++; // TIME EVENT


            if (counter == 20 * 50) { gameover = true; }
            //moving bullet
            for (int i = 0; i < 10; i++) {
                if (bullet[i][0] == 1) bullet[i][2] += vf;

                if (bullet[i][2] > 300) {

                    bullet[i][0] = 0;
                    deadbullet++;
                }

                for (int j = 0; j < 5; j++) {
                    if (heart[j][0] == 1) {
                        if (heart[j][2] > -180 && heart[j][2] > 0) {

                            heart[j][2] -= heart[j][3]; // regulate the position of the hearts

                            if ((bullet[i][0] == 1) && (bullet[i][1] > heart[j][1] - 50) && (bullet[i][1] < heart[j][1] + 43) &&
                                (bullet[i][2] > heart[j][2] - 34) && (bullet[i][2] < heart[j][2] + 30)) {
                                bullet[i][0] = 0;
                                heart[j][0] = 0;


                                // to calculate distance
                                int centerx = 400;
                                int distance = heart[j][1] - centerx;

                                if (distance < 0)
                                    distance = -distance;

                                int score = 0;

                                if (distance < 40)
                                    score = 5;
                                else if (distance < 120)
                                    score = 4;
                                else if (distance < 200)
                                    score = 3;
                                else if (distance < 280)
                                    score = 2;
                                else
                                    score = 1;

                                point += score;

                                if (++deadbullet == 10)
                                    resetbullet(); // refill amo

                            }
                        }

                        else {
                            heart[j][2] = 450;
                            heart[j][0] = 0;
                            if (currentpacmanmonster < 3) {
                                pacmanmonster[currentpacmanmonster][0] = 1;

                                pacmanmonster[currentpacmanmonster][1] = heart[j][1];
                                pacmanmonster[currentpacmanmonster][2] = 10;
                                pacmanmonster[currentpacmanmonster][3] = (rand() % 5 + 2) / 5.0;
                                currentpacmanmonster++;
                            }
                            if (currentpacmanmonster == 3) { gameover = true; }
                        }
                    }
                }
            }

            for (int i = 0; i < 3; i++) {
                if (pacmanmonster[i][0] == 1) {
                    pacmanmonster[i][1] += pacmanmonster[i][3];


                    if (pacmanmonster[i][1] < -350 || pacmanmonster[i][1] > 350)
                        pacmanmonster[i][3] *= -1;
                }
            }


            if (deadbullet == 10)
                resetbullet();
        }
    }

    //to refresh the window it calls display() function
    glutPostRedisplay();

}


void init() {
    //smoothing shapes

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    inithearts();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Heart to Monster - Duygu Abbasoglu");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //keyboard registration
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //mouse registration
    glutMouseFunc(onClick);


#if TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    init();

    glutMainLoop();

    return 0;

}