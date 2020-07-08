/*
 * Example 4-1 : Drawing a Smooth-Shaded Triangle: smooth.c
 * smooth.c
 * This program demonstrates smooth shading.
 * A smooth shaded polygon is drawn in a 2-D projection.
 */
#include <cstdio>
#include <cstdlib>
#include <string>
#include <GL/glut.h>

int _indexColor = 0;

void init(bool mode) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (mode? GL_FLAT:GL_SMOOTH);
}

void triangle(void)
{
	glIndexi(_indexColor);
	glBegin (GL_TRIANGLES);
	glColor3f (1.0, 0.0, 0.0);
	glVertex2f (5.0, 5.0);
	glColor3f (0.0, 1.0, 0.0);
	glVertex2f (25.0, 5.0);
	glColor3f (0.0, 0.0, 1.0);
	glVertex2f (5.0, 25.0);
	glEnd();
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	triangle ();
	glFlush ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	if (w <= h)
		gluOrtho2D (0.0, 30.0, 0.0, 30.0 * (GLfloat) h/(GLfloat) w);
	else
		gluOrtho2D (0.0, 30.0 * (GLfloat) w/(GLfloat) h, 0.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'x':
		case 27:
			exit(0);
			break;
	}
}

void printIndexColor()
{
	// void glutSetColor(int cell, GLfloat red, GLfloat green, GLfloat blue);
	// GLfloat glutGetColor(int cell, int component);
	int count = glutGet(GLUT_WINDOW_COLORMAP_SIZE);
	printf("Index colors count: %d\n", count);
	for (int i = 0; i < count; ++i)
	{
		GLfloat r = glutGetColor(i, GL_RED);
		GLfloat g = glutGetColor(i, GL_GREEN);
		GLfloat b = glutGetColor(i, GL_BLUE);
		printf("--> color %d (%.2f,%.2f,%.2f)\n", i, r, g, b);
	}
}

int main(int argc, char** argv)
{
	bool fmode = argc==2 && !std::string("-flat").compare(argv[1]);
	bool imode = argc>1 && !std::string("-index").compare(argv[1]);
	if (imode)
	{
		_indexColor = atoi(argv[2]);
	}
	printf("Color Mode %d %s %s\n", _indexColor, argv[1], !fmode? "GL_SMOOTH":"GL_FLAT");

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | imode? GLUT_INDEX:GLUT_RGB);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init (fmode);
	if (imode) printIndexColor();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glutMainLoop();
	return 0;
}
