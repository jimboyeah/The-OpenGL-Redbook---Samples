#include <iostream>
#include <cstring>
#include <cstdarg>
#include <freeglut.h>

using namespace std;

#define KEY_ESC             0x1b  // [ESC,27]

void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
}

static void bitmapPrintf (const char *fmt, ...)
{
	static char buf[256];
	va_list args;

	va_start(args, fmt);
#if defined(WIN32) && !defined(__CYGWIN__)
	(void) _vsnprintf (buf, sizeof(buf), fmt, args);
#else
	(void) vsnprintf (buf, sizeof(buf), fmt, args);
#endif
	va_end(args);
	glutBitmapString ( GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)buf ) ;
}

const char* _OpenGLVersion;
void version(void)
{
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* OpenGLVersion = glGetString(GL_VERSION);
	const GLubyte* gluVersion = gluGetString(GLU_VERSION);
	const GLubyte* Extensions = glGetString(GL_EXTENSIONS);
	printf("Vender: %s\n", name);
	printf("Renderer ID: %s\n", renderer);
	printf("OpenGL Version: %s\n", OpenGLVersion );
	printf("OpenGL Utility: %s\n", gluVersion);
	// printf("OpenGL Extensions: %s\n", Extensions );
	int len = strlen((const char*)Extensions);
	unsigned char* wrap = (unsigned char*)Extensions;
	for( int i = 0; i<len; i++) wrap[i] = wrap[i]==' '? '\n':wrap[i];
	printf("OpenGL Extensions: %s\n", wrap );
	_OpenGLVersion = (const char *)OpenGLVersion;
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	/* set raster position acording to viewport for bitmap string */
	// glWindowPos2f ( 0, 0 );
	glRasterPos2f ( -1, 1 );
	bitmapPrintf("%s", _OpenGLVersion);

	/* modeling transformation */ 
	glScalef (1.0, 1.0, 1.0);
	glFlush ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-w/2, w/2, -h/2, h/2, 1, -1);

	glMatrixMode (GL_MODELVIEW);
}

void onKey(unsigned char key, int x, int y)
{
	switch(key)
	{
		case KEY_ESC:
		case 'x':
			exit(EXIT_SUCCESS);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();

	version();

	glutKeyboardFunc(onKey); 
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
