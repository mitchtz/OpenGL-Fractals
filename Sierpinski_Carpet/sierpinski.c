//Mitch Zinser
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <math.h>
#include <stdio.h>


// global variable
GLfloat angle = 0.0f;  // rotational angle of the shapes
int refreshMills = 50; // refresh interval in milliseconds
//Timer value
double timer_num = 0;
//Toggle movement
int move=0;
//Ratio for converting degrees to radians
double deg_to_rad = M_PI/180;
//Frequency fo color change
double frequency = 0.1;
//Increment frequency to cause color change rapidy
double freq_inc = 0.01;
//Scaling factor
double third = 1.0/3.0;
//Where to move the board when moving
double look_x = 0.0;
double look_y = 0.0;
//How far to zoom in (by scaling)
double zoom = 2.0;
//Number of repetitions, cycles
int repetitions = 0;
//Global pointer to dynamic 2D array of points to draw
int *frac_points = NULL;
double red,green,blue;
int mod;
/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/* Initialize OpenGL Graphics */
void initGL() {
   // Set "clearing" or background color
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

/*Function that draws a section of the Sierpinski Carpet fractal. Draws a square for base case
*Recursive. Takes in cycles left to draw*/
void Sierpinski(int cycles, int color_num) {
   glPushMatrix();
   //Scale to 1/3 size
   glScaled(third,third,third);
   //Base case
   //If there is no more cycles, draw trunk (line)
   if (cycles == 0) {
      glColor3f(1.0,1.0,1.0);
      glBegin(GL_QUADS);
      glVertex2f(-0.5,-0.5);
      glVertex2f(0.5,-0.5);
      glVertex2f(0.5,0.5);
      glVertex2f(-0.5,0.5);
      glEnd();
   }
   //Otherwise square of 8 squares (middle not filled in)
   else {
      
   
      //Move to bottom left, call Sierpinski
      glTranslated(-third,-third,0);
      Sierpinski(cycles-1, color_num+1);
      //Move to bottom middle, call Sierpinski
      glTranslated(third,0,0);
      Sierpinski(cycles-1, color_num+1);
      //Move to bottom right, call Sierpinski
      glTranslated(third,0,0);
      Sierpinski(cycles-1, color_num+1);
      //Move to middle right, call Sierpinski
      glTranslated(0,third,0);
      Sierpinski(cycles-1, color_num+1);
      //Move to upper right, call Sierpinski
      glTranslated(0,third,0);
      Sierpinski(cycles-1, color_num+1);
      //Move to upper middle, call Sierpinski
      glTranslated(-third,0,0);
      Sierpinski(cycles-1, color_num+1);
      //Move to upper left, call Sierpinski
      glTranslated(-third,0,0);
      Sierpinski(cycles-1, color_num+1);
      //Move to middle left, call Sierpinski
      glTranslated(0,-third,0);
      Sierpinski(cycles-1, color_num+1);
   }
   glPopMatrix();
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer
   glMatrixMode(GL_MODELVIEW);     // To operate on Model-View matrix
   glLoadIdentity();               // Reset the model-view matrix
   //glPushMatrix();
   //Scaling
   glScaled(zoom,zoom,zoom);
   //Move for zooming
   glTranslated(look_x,look_y,0.0);
   //Center shape
   glTranslated(0.0,0.0,0.0);
   //Call Koch
   Sierpinski(repetitions,0);
   //glPopMatrix(); // Restore the model-view matrix
   glColor3f(1.0,1.0,1.0);
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Repetitions:%d",repetitions);
   
   glFlush();
   glutSwapBuffers();   // Double buffered - swap the front and back buffers
 
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
   //aspect = (GLfloat)width/(GLfloat)height;
   //printf("width:%.2f\n", (GLfloat)width);
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();
   if (width >= height) {
     // aspect >= 1, set the height from -1 to 1, with larger width
      gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);

   } else {
      // aspect < 1, set the width to -1 to 1, with larger height
     gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
   }
}
/*
 *  GLUT calls this routine every 50ms
 */
void timer(int toggle)
{
   
   //  Toggle timer_num
   if (toggle>0)
      move = !move;
   //  Increment timer_num
   else
   {
      timer_num += 0.1;
      //frequency += freq_inc;
   }

   //  Set timer to go again
   if (move && toggle>=0) glutTimerFunc(refreshMills,timer,0);
   //Don't redisplay unless key is pressed since everything is static
   //glutPostRedisplay();
}
/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - move right based on zoom
   if (key == GLUT_KEY_RIGHT)
      look_x -= (1/zoom)/10;
   //  Left arrow key - move left
   else if (key == GLUT_KEY_LEFT)
      look_x += (1/zoom)/10;
   //  Up arrow key - move up
   else if (key == GLUT_KEY_UP)
      look_y -= (1/zoom)/10;
   //  Down arrow key - move down
   else if (key == GLUT_KEY_DOWN)
      look_y += (1/zoom)/10;
   //  PageUp key - zoom in
   else if (key == GLUT_KEY_PAGE_UP)
      zoom = zoom*1.5;
   //  PageDown key - zoom out
   else if (key == GLUT_KEY_PAGE_DOWN)
      zoom = zoom/1.5;
   //  Update state
   timer(-1);
   //Redisplay
   glutPostRedisplay();
}
/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //Exit on ESC
   if (ch == 27)
      exit(0);
   else if (ch == '0') {
      look_x = look_y = 0.0;
      zoom = 3.0;
   }
   //Increase 
   else if (ch == 'p') {
      repetitions++;
      //printf("Repetitions:%d\n", repetitions);
   }
   //Decrease points
   else if (ch == 'P' && repetitions > 0) {
      repetitions--;
      //printf("Repetitions:%d\n", repetitions);
   }

   //  Update state
   timer(-1);
   //Redisplay
   glutPostRedisplay();
}

 

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);          // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height - non-square
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow("Sierpinski Carpet Fractal");  // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   //glutTimerFunc(0, Timer, 0);     // First timer call immediately
   initGL();                       // Our own OpenGL initialization
   //Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   timer(1);
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}