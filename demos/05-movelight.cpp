
/*
 * Example 5-6 : Moving a Light with Modeling Transformations: movelight.c
 *  movelight.c
 *  This program demonstrates when to issue lighting and
 *  transformation commands to render a model with a light
 *  which is moved by a modeling transformation (rotate or
 *  translate).  The light position is reset after the modeling
 *  transformation is called.  The eye position does not change.
 *
 *  A sphere is drawn using a grey material characteristic.
 *  A single light source illuminates the object.
 *
 *  Interaction:  pressing the left mouse button alters
 *  the modeling transformation (x rotation) by 30 degrees.
 *  The scene is then redrawn with the light in a new position.
 */
#include <GL/glut.h>
#include <stdlib.h>

static int spin = 0;
static int _ambient = 0;
static int _diffuse = 0;
static int _specular = 0;
static int _light = 0;
static int _material = 0;

static void initLights( void );


/*  Initialize material property, light source, lighting model,
 *  and depth buffer.
 */
void init(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	initLights();
}

static void initLights()
{
	glClearColor(0.0f, 0.0f, 0.7f, 1.0f);

	GLfloat ambientLight[]  = {0.2f,  0.2f,  0.2f,  1.0f};//环境光源
	GLfloat diffuseLight[]  = {0.9f,  0.9f,  0.9f,  1.0f};//漫反射光源
	GLfloat specularLight[] = {1.0f,  1.0f,  1.0f,  1.0f};//镜面光源
	GLfloat lightPos[]      = {50.0f, 80.0f, 60.0f, 1.0f};//光源位置
 
	if(_light){
		glEnable(GL_LIGHTING);                                //启用光照
		glEnable(GL_LIGHT0);                                  //打开第一个灯光
	}
	if(_ambient) glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);       //设置环境光源
	if(_diffuse) glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);       //设置漫反射光源
	if(_specular) glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);     //设置镜面光源
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);          //设置灯光位置
 
	if(_material){
		glEnable(GL_COLOR_MATERIAL);                          //启用材质的颜色跟踪
	}else{
		glDisable(GL_COLOR_MATERIAL);
	} 
	if(_diffuse) glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);    //指定材料着色的面
	if(_specular) glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);   //指定材料对镜面光的反应
	glMateriali(GL_FRONT, GL_SHININESS, 100);             //指定反射系数
}

/*  Here is where the light position is reset after the modeling
 *  transformation (glRotated) is called.  This places the
 *  light at a new position in world coordinates.  The cube
 *  represents the position of the light.
 */
void display(void)
{
	GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };
	initLights();

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix ();
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix ();
	glRotated ((GLdouble) spin, 1.0, 0.0, 0.0);
	glLightfv (GL_LIGHT0, GL_POSITION, position);

	glTranslated (0.0, 0.0, 1.5);
	glDisable (GL_LIGHTING);
	glColor3f (0.0, 1.0, 1.0);
	glutSolidCube (0.1);
	if (_light) glEnable (GL_LIGHTING);

	glColor3f( 0.4, 0.28, 0.28 );
	glTranslated(0.0, -1.0, -1.5);
	glutSolidTeapot (0.75);
	glPopMatrix ();

	glutSolidTorus (0.275, 0.85, 8, 15);
	glPopMatrix ();
	glFlush ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				spin = (spin + 30) % 360;
				glutPostRedisplay();
			}
			break;
		default:
			break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'w':
			spin = (spin + 30) % 360;
			break;
		case 'e':
			spin = (spin + 360 - 30) % 360;
			break;

		case 'a': _ambient = !_ambient; break;
		case 'd': _diffuse = !_diffuse; break;
		case 's': _specular = !_specular; break;
		case 'l': _light = !_light; break;
		case 'm': _material = !_material; break;

		case 'x':
		case 27:
			exit(0);
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
