#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "traqueboule.h"
#include "mesh.h"

using namespace std;
void computeLighting();
void dealWithUserInput(int x, int y);
Mesh MyMesh;
unsigned int W_fen = 800;  // largeur fenetre
unsigned int H_fen = 800;  // hauteur fenetre

// g++ *.cpp -lGL -lGLU -lglut -I . && ./a.out

//________________________________
//________________________________
//________________________________
//________________________________
//________________________________
//COMMENCEZ A LIRE ICI!!!

//couleur du d�cors
float BackgroundColor[]={0,0,0};
// Diff�rents modes d'affichage
enum Mode{ ORIGINAL_LIGHTING=0, TOON_LIGHTING, SPECULAR_LIGHTING, COMBINED_LIGHTING, MODIFY_LIGHTING, MODIFY_SPEC, SURFACE_EDIT};
Mode mode=ORIGINAL_LIGHTING;

//selon la vitesse de l'ordinateur on recalcule l'eclairage a chaque frame.
//toggle avec la touche "U" pendant l'execution du programme.
//"u" en miniscule effectue un seul calcul pour la configuration courrante.
bool updateAlways=true;

//devient plus int�ressant plus tard dans l'exercise.
//pour le moment, utilisez toujours LightPos[0]
std::vector<Vec3Df> LightPos;
//la lumi�re courrante (au d�but ceci vaut toujours 0)
int SelectedLight=0;
//pas encore necessaire, plus tard, ca va servir pour avoir des lumi�res color�es
std::vector<Vec3Df> LightColor;

//la position de la cam�ra COURRANTE!
Vec3Df CamPos = Vec3Df(0.0f,0.0f,-4.0f);


//afficher le sommet choisi touche "s"
bool ShowSelectedVertex=false;
//les deux variables apr�s sont utilis�es plus tard dans le TP.
int SelectedVertex=-1;

//un tableau suppl�mentaire utilis� pour faire des changements locals (plus tard dans l'exercise)
std::vector<Vec3Df> customData;



Vec3Df computeLighting(Vec3Df & vertexPos, Vec3Df & normal, unsigned int light, unsigned int index)
{
//la fonction pour calculer l'�clairage du mod�le.
	switch(mode)
	{
	case ORIGINAL_LIGHTING:
		{
			// dot(N, L) dot product between surface normal and light direction
			//std::cout << light << endl;
			float li = vertexPos.dotProduct(LightPos[0], normal);
			return Vec3Df(0.3f,0.3f,0.3f).operator *=(li); // returna ratt vector
		}
	case TOON_LIGHTING:
		{
			return Vec3Df(1,0,0);
		}
	case SPECULAR_LIGHTING:
		{
			return Vec3Df(1,1,0);
		}
	case COMBINED_LIGHTING:
	{
			return Vec3Df(0,0,1);
		}

	default:
		return Vec3Df(0,1,0);
	}
}


//pour g�rer les interactions avec l'utilisateur
void userInteraction(const Vec3Df & selectedPos, const Vec3Df & selectedNormal, int selectedIndex, Vec3Df origin, Vec3Df direction)
{
}



// prise en compte du clavier
//Vous allez ajouter quelques fonctionalites pendant le TP
//ce qui est important pour vous: key contient le caract�re correspondant � la touche appuy� par l'utilisateur

void keyboard(unsigned char key, int x, int y)
{
    printf("key %d pressed at %d,%d\n",key,x,y);
    fflush(stdout);
    if (key>'0'&& key<='7')
	{
		mode=Mode(key-'1');
		computeLighting();
		return;
	}
	
	switch (key)
    {
	//A remplir
	case 'r':
	break;
	case 'R':
	break;
	case 'g':
	break;
	case 'G':
	break;
	case 'b':
	break;
	case 'B':
	break;

	//a pas y toucher!!!

//ARRETEZ DE LIRE � PARTIR D'ICI!!!
//________________________________
//________________________________
//________________________________
//________________________________
//________________________________


	case 'l':
		{
			LightPos[SelectedLight]=getCameraPosition();
			return;
		}
	case 'L':
		{
			LightPos.push_back(getCameraPosition());
			LightColor.push_back(Vec3Df(1,1,1));
			return;
		}
	case '+':
		{
			++SelectedLight;
			if (SelectedLight>=LightPos.size())
				SelectedLight=0;
			return;
		}
	case '-':
		{
			--SelectedLight;
			if (SelectedLight<0)
				SelectedLight=LightPos.size()-1;
			return;
		}
	case 'U':
		{
			updateAlways=!updateAlways;
			return;
		}

	case 'N':
		{
			for (unsigned int i=0; i<MyMesh.vertices.size();++i)
			{
				customData[i]=Vec3Df(0,0,0);
			}
			LightPos.resize(1);
			LightPos[0]=Vec3Df(0,0,3);
			LightColor.resize(1);
			LightColor[0]=Vec3Df(1,1,1);
			SelectedLight=0;
		}

	case 'u':
		{
			//mise a jour de l'eclairage
			computeLighting();
			return;
		}
	case 's':
	{
		ShowSelectedVertex=!ShowSelectedVertex;
	}
	case ' ':
		{
			//vous n'avez pas besoin de regarder cette fonction
			dealWithUserInput(x,y);
		}
	}

}



//�clairage du mod�le (normalement vous n'avez pas � y toucher...
std::vector<Vec3Df> lighting;

/************************************************************
 * Fonction pour initialiser le maillage
 ************************************************************/
void init(const char * fileName){

	//this function loads a mesh
    MyMesh.loadMesh(fileName);
	lighting.resize(MyMesh.vertices.size());
	customData.resize(MyMesh.vertices.size(), Vec3Df(0,0,0));
	LightPos.push_back(Vec3Df(0,0,3));
	LightColor.push_back(Vec3Df(1,1,1));
	computeLighting();
}



/************************************************************
 * Appel des diff�rentes fonctions de dessin
************************************************************/


void dealWithUserInput(int x, int y)
{
	Vec3Df worldPoint=getWorldPositionOfPixel(x, H_fen-y);
	SelectedVertex=MyMesh.getClosestVertexIndex(CamPos, worldPoint-CamPos);
	if (SelectedVertex>=0)
	{
		Vec3Df selectedPos=MyMesh.vertices[SelectedVertex].p;
		Vec3Df selectedNormal=MyMesh.vertices[SelectedVertex].n;
		userInteraction(selectedPos, selectedNormal, SelectedVertex, CamPos, (worldPoint-CamPos).unit());				
	}
}

void dessiner( )
{

	glPointSize(10);
	glBegin(GL_POINTS);
	//LightColor
	glColor3f(1,0,0);	
	
	for (int i=0; i<LightPos.size();++i)	
	{	
		glVertex3f(LightPos[i][0],LightPos[i][1],LightPos[i][2]);
	}
	glEnd();
	
	glPointSize(40);
	glColor3f(1,1,0);	
	glBegin(GL_POINTS);
	glVertex3f(LightPos[SelectedLight][0],LightPos[SelectedLight][1],LightPos[SelectedLight][2]);
	glEnd();

	switch( mode )
    {
    case ORIGINAL_LIGHTING:
		{
			Vec3Df p;
			if (ShowSelectedVertex&&SelectedVertex>=0)
			{
				p=MyMesh.vertices[SelectedVertex].p;
				glBegin(GL_POINTS);
				glVertex3f(p[0],p[1],p[2]);
				glEnd();
			}
			MyMesh.drawWithColors(lighting);
		}
		break;
	case MODIFY_LIGHTING:
		//defineLight();
		break;
    default:
		MyMesh.drawWithColors(lighting);
		break;
    }
}




void idle()
{
	CamPos=getCameraPosition();

	if (updateAlways)
		computeLighting();

	glutPostRedisplay();
}

void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);


void computeLighting()
{
	std::vector<Vec3Df> *result=&lighting;


	for (unsigned int i=0; i<MyMesh.vertices.size();++i)
	{
		(*result)[i]=Vec3Df();
		for (int l=0; l<LightPos.size();++l)
			(*result)[i]+=computeLighting(MyMesh.vertices[i].p, MyMesh.vertices[i].n, l, i);
	}
}



/************************************************************
 * Programme principal
 ************************************************************/
int main(int argc, char** argv)
{
    glutInit (&argc, argv);

    init(argc == 2 ? argv[1] : "DavidHeadCleanMax.obj");

    // couches du framebuffer utilisees par l'application
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // position et taille de la fenetre
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(W_fen,H_fen);
    glutCreateWindow(argv[0]);	

    // Initialisation du point de vue
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-4);
    tbInitTransform();     // initialisation du point de vue
    tbHelp();                      // affiche l'aide sur la traqueboule

    glDisable( GL_LIGHTING );
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    
    // cablage des callback
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMouseFunc(tbMouseFunc);    // traqueboule utilise la souris
    glutMotionFunc(tbMotionFunc);  // traqueboule utilise la souris
    glutIdleFunc(idle);


    // Details sur le mode de trac�
    glEnable( GL_DEPTH_TEST );            // effectuer le test de profondeur
    glShadeModel(GL_SMOOTH);

    // Effacer tout
    glClearColor (BackgroundColor[0],BackgroundColor[1], BackgroundColor[2], 0.0);
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); // la couleur et le z
   

    // lancement de la boucle principale
    glutMainLoop();

    return 0;  // instruction jamais ex�cut�e
}


/************************************************************
 * Fonctions de gestion opengl � ne pas toucher
 ************************************************************/
// Actions d'affichage
// Ne pas changer
void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT); // la couleur et le z

	glLoadIdentity();  // repere camera

    tbVisuTransform(); // origine et orientation de la scene

    dessiner( );    

    glutSwapBuffers();
}

// pour changement de taille ou desiconification
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50, (float)w/h, 1, 10);
    glMatrixMode(GL_MODELVIEW);
}
