/*
 * Example 6-3 : Antialiased lines: aargb.c
 *  This program draws shows how to draw anti-aliased lines. It draws
 *  two diagonal lines to form an X; when 'r' is typed in the window, 
 *  the lines are rotated in opposite directions.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

static float rotAngle = 0.;
static int _antialias = 0;
static char _level = ' ';
static char _hint = ' ';

/*  Initialize antialiasing for RGBA mode, including alpha
 *  blending, hint, and line width.  Print out implementation
 *  specific info on line width granularity and width.
 */
void init(void)
{
   GLfloat values[2];
   glGetFloatv (GL_LINE_WIDTH_GRANULARITY, values);
   printf ("GL_LINE_WIDTH_GRANULARITY value is %3.1f\n", values[0]);

   glGetFloatv (GL_LINE_WIDTH_RANGE, values);
   printf ("GL_LINE_WIDTH_RANGE values are %3.1f %3.1f\n",
      values[0], values[1]);

   glEnable (GL_LINE_SMOOTH);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
   glLineWidth (1.5);

   glClearColor(0.0, 0.0, 0.0, 0.0);
}

/* Draw 2 diagonal lines to form an X
 */
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   if(_antialias)
   {
      glEnable (GL_LINE_SMOOTH);
   }else{
      glDisable (GL_LINE_SMOOTH);
   }

   int level = _level=='h'? GL_NICEST: _level=='l'? GL_FASTEST:GL_DONT_CARE;
   switch(_hint)
   {
      case '1': glHint(GL_FOG_HINT, level);
      case '2': glHint(GL_LINE_SMOOTH_HINT, level);
      case '3': glHint(GL_PERSPECTIVE_CORRECTION_HINT, level);
      case '4': glHint(GL_POINT_SMOOTH_HINT, level);
      case '5': glHint(GL_POLYGON_SMOOTH_HINT, level);
      default: glHint(GL_FALSE, GL_DONT_CARE);
   }

   glColor3f (0.0, 1.0, 0.0);
   glPushMatrix();
   glRotatef(-rotAngle, 0.0, 0.0, 0.1);
   glBegin (GL_LINES);
      glVertex2f (-0.5, 0.5);
      glVertex2f (0.5, -0.5);
   glEnd ();
   glPopMatrix();

   glColor3f (0.0, 0.0, 1.0);
   glPushMatrix();
   glRotatef(rotAngle, 0.0, 0.0, 0.1);
   glBegin (GL_LINES);
      glVertex2f (0.5, 0.5);
      glVertex2f (-0.5, -0.5);
   glEnd ();
   glPopMatrix();

   glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h) 
      gluOrtho2D (-1.0, 1.0, 
         -1.0*(GLfloat)h/(GLfloat)w, 1.0*(GLfloat)h/(GLfloat)w);
   else 
      gluOrtho2D (-1.0*(GLfloat)w/(GLfloat)h, 
         1.0*(GLfloat)w/(GLfloat)h, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{

   switch (key) {
      case 'a':
         _antialias = !_antialias;
         break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
         _hint = key; 
         break;
      case 'h':
      case 'l':
      case 'd':
         _level = key; 
         break;
      case 'r':
         rotAngle += 20.;
         if (rotAngle >= 360.) rotAngle = 0.;
         break;
      case 27:  /*  Escape Key  */
         exit(0);
         break;
      default:
         break;
    }
   glutPostRedisplay(); 
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (400, 400);
   glutCreateWindow (argv[0]);
   init();
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard);
   glutDisplayFunc (display);
   glutMainLoop();
   return 0;
}
