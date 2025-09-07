/* DM Builder 0.8x code
 *
 * This is the base code of version DM Builder 0.85.mu (also known as 0.86.c)
 * It was initially released in 2003-04.
 * I rebuilt this original version in 2010-04 (7 years later ! ...)
 *
 * This branch is now referenced as DM Builder 0.86.kappa (kappa to refer to "keep" of "Skullkeep"!)
 * However, it is not the code from the original 0.86 branch made in 2003.
 * The 0.86 was never released because incomplete. So this new one is built from the "stable" 0.85 released version.
 *
 * The new DMB reference build in 2010 is the 0.9x Core9 branch while the 0.85/0.86 is still in Core6.
 * This 0.86.kappa version is a sort of quick dev to support DM2 map with the 0.85 interface
 * and then allowing a patched version for the 0.85.
 *
 * Between 2003 and 2010 I made two attempts to rewrite DMB but failed to complete any functional version:
 * Core7 introduced a more friendly interface with moveable windows and item selection from panels.
 * Core8 introduced a batch mode and interactive communication mode from a console.
 * The basis of these two cores eventually ended in the structure of the Core9.
 * Core9 is now developed
 *
 * DMB 0.8x is written in C/OpenGL whereas 0.9x is written in C/C++/SDL
 *
 */

#include <skullexe.h>
#include <editor.h>
#include <keyboard.h>
#include <display.h>
#include <displaytext.h>
#include <version.h>
#include <mouse.h>
#include <string.h>

#include <GL/glut.h>	// include glut last

extern tSkullExe xSkullExe;

//--- Release version : NDEBUG
//--- Debug version : _DEBUG

#ifdef _DEBUG
static int versionDebug = 1;
#else 
static int versionDebug = 0;
#endif

static const char *shorttitle = "DMBuilder";
static const char *apptitle = "Dungeon Master Builder";
static const char *writtenby = "written by Guillaume Bastard";
static const char *author = "Sphenx";
static const char *codeversion = "code version 6";

// WSXGA+
int winW = 1680;
int winH = 1050;

void glutDisplay (void)
{
	redrawScreen ();
	glutSwapBuffers ();
}

extern int angle;

void glutIdle (void)
{
	angle = angle + 1;
	glutPostRedisplay ();
}

void glutResize (int w, int h)
{
    winW = w;
    winH = h;

    glViewport (0, 0, w, h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

    glOrtho (-w, w, -h, h, -1, 1);

    glutPostRedisplay ();
}

void glutAppInit (void)
{
	Mouse_Init ();
	startEditor ();
	fastInit ();
	glDisable (GL_DEPTH_TEST);     
    glEnable (GL_TEXTURE_2D);
	glBlendFunc(GL_ONE, GL_ONE);
}

int main (void)
{
	char appName[128];
	char revision[64];
//--- Preparation 
	sprintf(appName, "%s v%1d.%02d", apptitle, DMB_MAJOR, DMB_MINOR);
	if (versionDebug == 1)
		strcat(appName, " (DEBUG BUILD)");
#ifdef DEVBUILD
	strcat(appName, " (DEVEVELOPMENT BUILD)");
#endif
	printf("Starting %s (c) %04d - %04d by %s\n", shorttitle
		, DMB_YEAR_FIRST, DMB_YEAR_CURRENT, author);
	printf("Application Core %1d (%s) Kernel %1d.%02d",
		DMB_CORE, DMB_MONSTER, DMB_MAJOR, DMB_MINOR);
#if defined(DMB_REVISION)
	sprintf(revision, "%c", DMB_REVISION);
#endif
	if (revision[0] != '\0')
		printf(".%s", revision);
	printf("\n");

	Context_Init();
	SkullExe_Init(&xSkullExe);
	reinitTextStrings();

//--- Init GL and run main
    glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize (winW, winH);
    glutCreateWindow (appName);
    glutDisplayFunc (glutDisplay);
    glutReshapeFunc (glutResize);
    glutIdleFunc (glutIdle);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc (arrow_keys);
	glutMouseFunc (Mouse_Handler);
	glutMotionFunc(Mouse_Motion);
	glutPassiveMotionFunc (Mouse_Motion_Passive);

    glutAppInit ();
    
    glutMainLoop();
	return 0;
}



