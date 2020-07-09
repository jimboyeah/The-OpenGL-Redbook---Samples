
/*
 *  Example 3-6 : Planetary System: planet.c
 *  This program shows how to composite modeling transformations
 *  to draw translated and rotated models.
 *  Interaction:  pressing the d and y keys (day and year)
 *  alters the rotation of the planet around the sun.
 */
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>

static int year = 0, day = 0;
static float _near = 1.0;
static float _far = 20.0;
static float _fov = 60.0;
static int _win;
static int _winsize = 2;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   glPushMatrix();
   glutWireSphere(1.0, 20, 16);   /* draw sun */
   glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);
   glTranslatef (2.0, 0.0, 0.0);
   glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
   glutWireSphere(0.2, 10, 8);    /* draw smaller planet */
   glPopMatrix();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(_fov, (GLfloat) w/(GLfloat) h, _near, _far);
   printf("_fov %f, _near %f, _far %f\n", _fov, _near, _far);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case '[':
         _far -= 0.2;
         _winsize = -_winsize;
         break;
      case ']':
         _far += 0.2;
         _winsize = -_winsize;
         break;
      case 'w':
         _near += .2;
         _far += .2;
         _winsize = -_winsize;
         break;
      case 's':
         _near -= .2;
         _far -= .2;
         _winsize = -_winsize;
         break;
      case 'q':
         _fov += 1;
         _winsize = -_winsize;
         break;
      case 'e':
         _fov -= 1;
         _winsize = -_winsize;
         break;
      case 'd':
         day = (day + 10) % 360;
         break;
      case 'D':
         day = (day - 10) % 360;
         break;
      case 'y':
         year = (year + 5) % 360;
         break;
      case 'Y':
         year = (year - 5) % 360;
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
   glutPostRedisplay();
   glutReshapeWindow(500 + _winsize, 500 + _winsize);
   glutPositionWindow(100-(_winsize>0? 1:0), 100-(_winsize>0? 1:0));
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   _win = glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}
