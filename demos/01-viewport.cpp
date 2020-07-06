// OpenGL Programming Guide Example 3-1 : Transformed Cube: cube.c

#include <iostream>
#include <cstdarg>
#include <freeglut.h>

using namespace std;

/*
 Key set
 1 - 9 models 
	 1 Cone
	 2 Cube
	 3 Dodecahedron
	 4 Icosahedron
	 5 Octahedron
	 6 Sphere
	 7 Teapot
	 8 Tetrahedron
	 9 Torus
 h/g solid/wired
 a - left
 s - down
 d - right
 w - up
 j - forward, mouse wheel
 k - backward, mouse wheel
 l - left view
 r - right view
 t - top view
 b - back view
 f - front view
*/

#define KEY_ESC             0x1b  // [ESC,27]

enum Surface {SurfaceSolid, SurfaceWired };

Surface WiredOrSolid = SurfaceWired;

float _distance = 5;
float _center_x = 0;
float _center_y = 0;
char _model = '1';

static void drawModel()
{
	float size = 1;
	float radius = 1;
	double base = 1;
	double height = 2;
	double width = 2;
	double innerRadius = 1;
	double outerRadius = 2;
	int slices = 85;
	int stacks = 86;
	int sides = 87;
	int rings = 88;

	switch (_model)
	{
		case '3':
		case '4':
		case '5':
		glColor3f (0.5, 0.8, 0.5); break;
		case '2':
		case '6':
		case '7':
		glColor3f (1.0, 0.3, 0.5); break;
	}

	if(WiredOrSolid==SurfaceWired)
	{
		 switch (_model)
		 {
			case '1': glutWireCone( base, height, slices, stacks ); break;
			case '2': glutWireCube( size ); break;
			case '3': glutWireDodecahedron( ); break;
			case '4': glutWireIcosahedron( ); break;
			case '5': glutWireOctahedron( ); break;
			case '6': glutWireSphere( radius, slices, stacks ); break;
			case '7': glutWireTeapot( size ); break;
			case '8': glutWireTetrahedron( ); break;
			case '9': glutWireTorus( innerRadius, outerRadius, sides, rings ); break;
		 }
		 return;
	}
	switch(_model)
	{
		 case '1': glutSolidCone( base, height, slices, stacks ); break;
		 case '2': glutSolidCube( size ); break;
		 case '3': glutSolidDodecahedron( ); break;
		 case '4': glutSolidIcosahedron( ); break;
		 case '5': glutSolidOctahedron( ); break;
		 case '6': glutSolidSphere( radius, slices, stacks ); break;
		 case '7': glutSolidTeapot( size ); break;
		 case '8': glutSolidTetrahedron( ); break;
		 case '9': glutSolidTorus( innerRadius, outerRadius, sides, rings ); break;
	}
}


static void initLights()
{
    glClearColor(0.0f, 0.0f, 0.7f, 1.0f);

    GLfloat ambientLight[]  = {0.2f,  0.2f,  0.2f,  1.0f};//环境光源
    GLfloat diffuseLight[]  = {0.9f,  0.9f,  0.9f,  1.0f};//漫反射光源
    GLfloat specularLight[] = {1.0f,  1.0f,  1.0f,  1.0f};//镜面光源
    GLfloat lightPos[]      = {50.0f, 80.0f, 60.0f, 1.0f};//光源位置
 
    glEnable(GL_LIGHTING);                                //启用光照
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);       //设置环境光源
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);       //设置漫反射光源
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);     //设置镜面光源
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);          //设置灯光位置
    glEnable(GL_LIGHT0);                                  //打开第一个灯光
 
    glEnable(GL_COLOR_MATERIAL);                          //启用材质的颜色跟踪
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);    //指定材料着色的面
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);   //指定材料对镜面光的反应
    glMateriali(GL_FRONT, GL_SHININESS, 100);             //指定反射系数
}


void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_FLAT);
	initLights();
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

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);

	/* set raster position acording to viewport for bitmap string */
	// glWindowPos2f ( 0, 0 );
	glRasterPos2f ( -1, 1 );
	bitmapPrintf("O(%.0f,%.0f,%.0f) M[%c] S[%c]\n", 
		_center_x, _center_y, _distance,
		_model, (WiredOrSolid == SurfaceWired? "W":"S"));

	/* clear the matrix */
	glLoadIdentity ();

	/* viewing transformation */
	gluLookAt (
		_center_x, _center_y, _distance, 
		0.0, 0.0, 0.0, 
		0.0, 1.0, 0.0);
	/* modeling transformation */ 
	glScalef (1.0, 1.0, 1.0);

	drawModel();
	glFlush ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
	glMatrixMode (GL_MODELVIEW);
}

static void onMouseWheel ( int wheel_number, int direction, int x, int y )
{
	_distance += direction;
	glutPostRedisplay();
}

void onKey(unsigned char key, int x, int y) 
{
	switch(key)
	{
		case 'a': _center_x -= 1; break;
		case 's': _center_y -= 1; break;
		case 'd': _center_x += 1; break;
		case 'w': _center_y += 1; break;
		case 'j': _distance -= 1; break;
		case 'k': _distance += 1; break;
		case 't': _center_y = 5; _center_x = 0; _distance = 0; break;
		case 'r': _center_y = 0; _center_x = 5; _distance = 0; break;
		case 'l': _center_y = 0; _center_x = -5; _distance = 0; break;
		case 'f': _center_y = 0; _center_x = 0; _distance = 5; break;
		case 'b': _center_y = 0; _center_x = 0; _distance = -5; break;

		case '1': 
		case '2': 
		case '3': 
		case '4': 
		case '5': 
		case '6': 
		case '7': 
		case '8': 
		case '9': _model = key; break;
		case 'h': WiredOrSolid = SurfaceSolid; break;
		case 'g': WiredOrSolid = SurfaceWired; break;

		case KEY_ESC:
		case 'x': exit(EXIT_SUCCESS); break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	glutKeyboardFunc(onKey);
	glutMouseWheelFunc(onMouseWheel);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
