
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <iostream>

using namespace std;

const double TWO_PI = 6.2831853;
double i = 0.;
// unde se va opri masina pe axa x, cand face schimbarea de benzi
double xFinal = 20;
double j = 1.0;
double k = 0;
double vitezaTrafic = 0.15;
GLsizei winWidth = 500, winHeight = 500;
GLuint traficID;
static GLfloat rotTheta = 0.0;

int distantaMasini = 700;


class scrPt
{
public:
	GLfloat x, y, z;
	void print(const char* text = " ") {
		cout << text << ' ' << x << ' ' << y << ' ' << z << endl;
	}
	scrPt operator + (scrPt const& a) {
		scrPt res;
		res.x = x + a.x;
		res.y = y + a.y;
		res.z = z + a.z;
		return res;
	}
	scrPt operator - (scrPt const& a) {
		scrPt res;
		res.x = x - a.x;
		res.y = y - a.y;
		res.z = z - a.z;
		return res;
	}
};


enum {
	NONE, AFIN, PERSP
};

enum {
	PATRULARE, MISIUNE, MISIUNE_SPECIALA
};

int rendermode = PATRULARE;

void menu(int selection)
{
	rendermode = selection;
	glutPostRedisplay();
}

void miscareGirofar(void) {
	// pentru girofar
	j += 0.05;
	if (j > 360) {
		j = 0;
	}
	if (j < 0) {
		j = 360;
	}

	// pentru schimbarea banzii
	if (i > xFinal && i > -120) {
		i -= 0.15;
	}
	if (i < xFinal && i < 20) {
		i += 0.15;
	}

	// resetarea pozitiei traficului
	if (k < -1 * 12. * distantaMasini) {
		k = 0;
	}
	else if (k > 5. * distantaMasini) {
		k = -1 * 6. * distantaMasini;
	}
	else {
		k -= vitezaTrafic;
	}

	glutPostRedisplay();
}

static void init(void)
{
	scrPt cerc;
	GLdouble theta;
	GLint aux;
	GLint n = 2000;

	//glClearColor(1.0, 1.0, 0.0, 0.0);
	// 159 / 255 , 61 / 255 , 0 , 1
	glClearColor(0.6235294, 0.239215, 0, 1);
	gluOrtho2D(-100, 100, -100, 100);


	traficID = glGenLists(1);
	//glNewList(traficID, GL_COMPILE);


	for (aux = 0; aux < 20; aux++)
	{
		//glBegin(GL_POLYGON);
		glColor3f(0, 1, 0);
		//glRectf(20, 100 + aux * 20, 60, 160 + aux*20);
		glRectf(20, 100 + aux * 20, 60, 160 + aux * 20);

		//glColor3f(0.5, 0, 0.5);
		glColor3f(0, 1, 0);
		glRectf(-120, 250 + aux * 20, -80, 310 + aux * 20);
	}
	glEndList();

	// folosit pentru opacitatea culorilor
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		if (vitezaTrafic < 0.7)
			vitezaTrafic += 0.05;
	}
	else
	{
		if (vitezaTrafic > -0.7)
			vitezaTrafic -= 0.05;
	}

	return;
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			xFinal = -120;
		//alpha = -1.0; glutIdleFunc(miscas);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			//alpha = 1.0; glutIdleFunc(miscad);
			xFinal = 20;
		break;
	default:
		break;
	}
}

void desenDrum(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	//glCallList(traficID);
	glLoadIdentity();

	//glBegin(GL_POLYGON);
////glRectf(-20., -1000, 20, 1000);
//glVertex2f(-20., -1000.);
//glVertex2f(20., -1000.);
//glVertex2f(20., 1000.);
//glVertex2f(-20., 1000.);
//glEnd();
	// DRUMUL
	glColor3f(0.2, 0.2, 0.2);
	glRectf(-200., -1000., 200., 1000.);
	// Linia ce separa benzile
	glColor3f(1, 1, 1);
	glRectf(-10., -1000., 10., 1000.);

	// initializare pentru tranzitii

	glPushMatrix();
	glTranslated(i, 0, 0);

	// MASINA
	//glColor3f(0, 0, 1);
	//glRectf(20, -180, 80, -110);
	// sasiu
	glColor3f(0, 0, 0);
	glRectf(20, -180, 80, -155);
	glColor3f(1, 1, 1);
	glRectf(20, -155, 80, -115);
	glColor3f(0, 0, 0);
	glRectf(20, -115, 80, -90);


	// stopuri
	glColor3f(1, 0, 0);
	glRectf(25, -185, 35, -180);
	glColor3f(1, 0, 0);
	glRectf(65, -185, 75, -180);

	// faruri
	glColor3f(0.8, 0.5, 0);
	glRectf(25, -90, 35, -85);
	glColor3f(0.8, 0.5, 0);
	glRectf(65, -90, 75, -85);

	//lumini faruri

	if (rendermode == MISIUNE_SPECIALA)
	{

		float x = 0.2;
		if (int(abs(k / 100)) % 2 == 0) {
			x = 0.7;
		}
		else {
			x = 0.2;
		}


		// cand x este 0.7 inseamna ca este faza lunga, deci opacitate mai mare.
		// pe langa opacitate as vrea sa adaug si o scalare pe verticala, sa para luminile mai lungi
		// este nevoie de round deoarece comparatia x == 0.7 nu poate fi folosita
		if (std::round(x) == 1) {
			glPushMatrix();
			// este necesare o translatie deoarece luminile sunt sub axa 0x si se scaleaza prost
			glTranslated(-90, -90, 0);
			glScalef(1., 2., 1.);
			glTranslated(90, 90, 0);


		}

		cout << x << endl;
		glColor4f(1, 1, 0, x);
		glBegin(GL_TRIANGLES);
		glVertex2f(30, -85);
		glVertex2f(0, 0);
		glVertex2f(60, 0);

		glVertex2f(40, 0);
		glVertex2f(100, 0);
		glVertex2f(70, -85);

		glEnd();

		if (std::round(x) == 1) {
			glPopMatrix();
		}

	}


	// GIROFAR
	if (rendermode != PATRULARE)
	{

		glTranslated(50, -135, 0);
		glRotated(j, 0, 0, 1);
		glTranslated(-50, 135, 0);

		//girofar
		glColor4f(1, 0, 0, 0.5);
		glBegin(GL_TRIANGLES);
		glVertex2f(10, -115);
		glVertex2f(50, -135);
		glVertex2f(10, -155);
		glEnd();


		glColor4f(0, 0, 1, 0.5);
		glBegin(GL_TRIANGLES);
		glVertex2f(50, -135);
		glVertex2f(90, -115);
		glVertex2f(90, -155);
		glEnd();

		glPopMatrix();

	}





	// pop pentru translatia masinii
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, k, 0);


	for (int aux = 0; aux < 10; aux++)
	{
		float offset = aux * distantaMasini;
		// masina banda dreapta cu geamuri
		glColor3f(0, 1, 0);
		glRectf(80, 490 + offset, 140, 600 + offset);
		glColor3f(0, 0, 0.4);
		glRectf(90, 510 + offset, 130, 520 + offset);
		glRectf(90, 570 + offset, 130, 580 + offset);


		// masina banda stanga cu geamuri
		glColor3f(0.5, 0, 0.5);
		glRectf(-140, 250 + offset, -80, 360 + offset);
		glColor3f(0, 0, 0.4);
		glRectf(-130, 270 + offset, -90, 280 + offset);
		glRectf(-130, 330 + offset, -90, 340 + offset);


		glBegin(GL_LINES);
		// portbagaje stanga
		glVertex2f(-140, 265 + offset);
		glVertex2f(-136, 265 + offset);

		glVertex2f(-136, 265 + offset);
		//glVertex2f(-140, 285 + offset);
		glVertex2f(-132, 285 + offset);

		glVertex2f(-132, 285 + offset);
		glVertex2f(-88, 285 + offset);

		glVertex2f(-88, 285 + offset);
		glVertex2f(-84, 265 + offset);

		glVertex2f(-84, 265 + offset);
		glVertex2f(-80, 265 + offset);

		// portbagaje dreapta x: 80 140 y: 490 600
		glVertex2f(80, 505 + offset);
		glVertex2f(84, 505 + offset);

		glVertex2f(84, 505 + offset);
		//glVertex2f(-140, 285 + offset);
		glVertex2f(88, 525 + offset);

		glVertex2f(88, 525 + offset);
		glVertex2f(132, 525 + offset);

		glVertex2f(132, 525 + offset);
		glVertex2f(136, 505 + offset);

		glVertex2f(136, 505 + offset);
		glVertex2f(140, 505 + offset);

		glEnd();

		// stopuri masini banda stanga
		glColor3f(1, 0, 0);
		glRectf(-135, 245 + offset, -125, 250 + offset);
		glColor3f(1, 0, 0);
		glRectf(-95, 245 + offset, -85, 250 + offset);

		// faruri masini banda stanga
		glColor3f(0.8, 0.5, 0);
		glRectf(-135, 360 + offset, -125, 365 + offset);
		glColor3f(0.8, 0.5, 0);
		glRectf(-95, 360 + offset, -85, 365 + offset);

		// stopuri masini banda dreapta
		glColor3f(1, 0, 0);
		glRectf(85, 485 + offset, 95, 490 + offset);
		glColor3f(1, 0, 0);
		glRectf(125, 485 + offset, 135, 490 + offset);

		// faruri masini banda dreapta
		glColor3f(0.8, 0.5, 0);
		glRectf(85, 600 + offset, 95, 605 + offset);
		glColor3f(0.8, 0.5, 0);
		glRectf(125, 600 + offset, 135, 605 + offset);


		//glEnd();

	}


	// incrementare pentru girofar si alte translatii
	glutIdleFunc(miscareGirofar);




	glutSwapBuffers();
	glFlush();
}


void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Trafic - Masina de politie");

	init();

	glutDisplayFunc(desenDrum);
	glutReshapeFunc(winReshapeFcn);

	glutCreateMenu(menu);

	glutAddMenuEntry("Patrulare", PATRULARE);
	glutAddMenuEntry("Misiune", MISIUNE);
	glutAddMenuEntry("Misiune speciala", MISIUNE_SPECIALA);

	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	glutMainLoop();

}

