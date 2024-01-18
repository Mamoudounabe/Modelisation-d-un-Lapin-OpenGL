/********************************************************/
/*                     Lapin.cpp                                                 */
/********************************************************/
/*                                      */
/********************************************************/

/* inclusion des fichiers d'en-tete freeglut */

#ifdef __APPLE__
#include <GLUT/glut.h> /* Pour Mac OS X */
#else
#include <GL/glut.h>   /* Pour les autres systemes */


#include <GL/freeglut.h>
#include <stdlib.h>
#endif
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <jpeglib.h>
#include <jerror.h>
//#include <Eigen/Dense>
#include <glm/glm.hpp>                // Pour les types de base (vec2, vec3, mat4, etc.)
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#define NM 30
#define NP 10



//#define larging  800
//#define hauting   531

#define larging  1300
#define hauting   866

#define large 804
#define haute 468


#include <cmath>
#include <math.h>

#define PI 3.14159265358979323846

using namespace std;

/*class Point*/

class Point{
    public :
	//coordonnées x, y et z du point
	double x;
	double y;
	double z;
	// couleur r, v et b du point
	float r;
	float g;
	float b;
};
//Tableau pour stocker les sommets du cube et leur couleur

Point pCube[8]={
    {-0.5,-0.5, 0.5,1.0,0.0,0.0},
    { 0.5, -0.5, 0.5,0.0,1.0,0.0},
    { 0.5, -0.5, -0.5,0.0,0.0,1.0},
    { -0.5, -0.5, -0.5,1.0,1.0,1.0},
    { -0.5,0.5, 0.5,1.0,0.0,0.0},
    { 0.5, 0.5, 0.5,0.0,1.0,0.0},
    { 0.5, 0.5, -0.5,0.0,0.0,1.0},
    { -0.5, 0.5, -0.5,1.0,1.0,1.0}};

//Tableau pour stocker les indices des sommets par face pour le cube
int fCube[6][4]={
  {0,1,2,3},//face envers endroit={0,3,2,1}
  {0,1,5,4},
  {1,2,6,5},
  {2,3,7,6},
  {0,4,7,3},
  {4,5,6,7}};

unsigned char image[larging*hauting*3];
unsigned char texture[hauting][larging][3];

unsigned char textureS[haute][large][3];
unsigned char imageS[large*haute*3];

char presse;
int anglex,angley,x,y,xold,yold;
double theta;
double fi;
double X[NP*NM];
double Y[NP*NM];
double Z[NP*NM];
int fSphere[(NP-1)*NM][4];
static double ycam=0;
static double xcam=0;
static double zoom=0;
static double Scalexc=1;
static double Scaleyc=1;
static double Scalezc=1;
static double lr=1.0;//couleur rouge lumiere
static double lb=1.0;//couleur bleu lumière
static double lj=1.0;//couleur jaune lumière
GLfloat Scalex=0.05, Scaley=0.05, Scalez=0.05;//zoom
GLfloat Translatex, Translatey, Translatez;//deplacement
GLfloat angle=0;
GLfloat posx=0;

/* Prototype des fonctions */
void affichage();//methode pour faire l'affichage
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();//fonction pour animer
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void remplirFace();//fonction pour remplir les faces
void remplirPoint(double R);//fonction pour remplir les points
void textureVentreLapin(char *fichier);
void textureCube(char *fichier);
void redim(int l,int h);
void ConstructionLapin();//fonction pour construire un dragon
void dessinCube();//fonction pour dessiner un cube
void GestionSpecial(int key, int x, int y);//fonction qui permet la rotation de la caméra avec les flèches directionelle
void affichageScaled(GLfloat scalex,GLfloat scaley, GLfloat scalez);//fonction pour gerer le zoom et le dezoom
void affichageTranslated(GLfloat translatex,GLfloat translatey, GLfloat translatez); //fonction pour gere le deplacement du dragon
void textureSol(char *fichier);//texture pour le sol
void Sol();//fonction plaquage texture au sol
void illumination();//methode pour illuminer


//double zoom = 10;
const int nbCercle = 20;
const int nbPointParCercle = 20;
// Tableau contenant les points
Point ptMembre[nbCercle * nbPointParCercle];
// Tableau contenant les faces
int fMembre[(nbCercle-1)*nbPointParCercle][4];
void initCorps();
void Corps();

//int anglex =  90,angley = 0;

void loadImage1(char* fichier);
const int hautimg1 = 255, largimg1 = 256;
unsigned char imageEcaille1[hautimg1*largimg1*3];
unsigned char textureEcaille1[hautimg1][largimg1][3];

void loadImage2(char* fichier);
const int hautimg2 = 512, largimg2 = 512;
unsigned char imageEcaille2[hautimg2*largimg2*3];
unsigned char textureEcaille2[hautimg2][largimg2][3];





int main(int argc,char **argv)
{
  /* initialisation de glut et creation de la fenetre */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(400,400);
  glutInitWindowSize(1000,800);
  glutCreateWindow("LAPIN");


 //textureVentreLapin("./peauRouge.jpg");//appel de l'image pour la texture
 textureVentreLapin("./PeauLapin.jpg");
 textureSol("./sol.jpg");//texture placage au sol


  /* Initialisation de l'etat d'OpenGL */
  glClearColor(0.0,0.0,0.0,0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);


  /* Mise en place de la projection perspective */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  /* Parametrage du placage de textures */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);


  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);
  glutSpecialFunc(GestionSpecial);//permet d'utiliser les touches directionelles du clavier
  glutIdleFunc(idle); //appel de la fonction pour animer càd activation du call back
  glutMainLoop();
    return 0;
}





/*************** Texture de peau de lapin ********************************************/

void textureVentreLapin(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;
  //unsigned char image[256*256*3];
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);


#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  if ((cinfo.image_width!=larging)||(cinfo.image_height!=hauting)) {
    fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }

  jpeg_start_decompress(&cinfo);
  ligne=image;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=image+3*larging*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

//stocke l'image dans la texture
  for(int i=0;i<hauting;i++)
    {
        for(int j=0;j<larging;j++)
        {
            texture[i][j][0]=image[i*larging*3+j*3];
            texture[i][j][1]=image[i*larging*3+j*3+1];
            texture[i][j][2]=image[i*larging*3+j*3+2];
        }
    }
}





void affichage()
{
/* effacement de l'image avec la couleur de fond */

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  glLoadIdentity();
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);
  glScalef(Scalexc,Scaleyc,Scalezc);
  glTranslatef(0,0,0);
  glOrtho(-1.5,1.5,-1.5,1.5,-1.5,1.5);
//gluLookAt(sin(xcam),0,cos(xcam)+ zoom, 0, 0, 0., 0, 1., 0.);//placement de la caméra

  //LUMIERE AMBIANTE
    float  CouleurLumiere[] = { lb, lr,lj};
    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CouleurLumiere);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


/****appel de la fonction constructionDragon pour dessiner le dragon**********/
ConstructionLapin();

/****appel de la fonction poour plaquer une texture au sol **********/
Sol();

    //Repère
    //axe x en rouge
 glMatrixMode(GL_MODELVIEW);

   glBegin(GL_LINES);
        glColor3f(1,0,0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(1, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
    	 glColor3f(0,1,0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 1,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 0,1.0);
    glEnd();

   //  glClearColor(0.8, 0.8, 0.8, 1); // couleur de l'arriere plan
  //glClearColor(1, 1, 1, 1);
  glFlush();

  //On echange les buffers
  glutSwapBuffers();
}



/*********************methode de construction d'un lapin*****************************/

void ConstructionLapin(){

glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,larging,hauting,0,GL_RGB,GL_UNSIGNED_BYTE,texture);//activer la texture sur le dragon
glEnable(GL_TEXTURE_2D);

 glScalef(Scalex,Scaley,Scalez);//une position initiale pour voir le dragon apres un lancement

  glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(Translatex,Translatey,Translatez);//fonction pour deplacer haut bas gauche droite
     glTranslatef(posx,0,0);;

/**---------------------------Le corps --------------------*/
        glPushMatrix();
        //glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(0, 0, 0);
        glScalef(1.0, 1.0, 1.5);
        remplirPoint(5.5);
        glPopMatrix();





       /**----------sphere pour le coup----------**/
               glPushMatrix();
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0, 1.0, 1.0);
        //glColor3f(0.25, 0.25, 0.25);
        glTranslatef(0,3.0,-6);
        //glRotatef(90, 0, 0, 1.0);
        glScalef(0.8, 2, 3);
        glRotatef(-55, 1, 0, 0);
        glRotatef(-70, 1.0,0.0,0.0);

        remplirPoint(1.5);
        glPopMatrix();

/**----------------------------- La tete --------------------**/

        glPushMatrix();
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(0.0, 6, -8.0);
         glScalef(0.8, 2, 3);
        glRotatef(-30, 1.0,0.0,0.0);
        remplirPoint(2);
        glPopMatrix();





/**----------Oreille gauchee----------**/

        glPushMatrix();
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glDisable(GL_TEXTURE_2D);
        glRotatef(angle/60,0,1,0);
        glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(-1, 11,-8.0); // Translation sur l'axe vertical
        glRotatef(angle, 0.0, 0.0, 0.0); // Rotation autour de l'axe X pour l'orientation verticale
        glRotatef(-90,0.0,1.0,0.0);
        float angleLeft = 10.0;  // Angle d'inclinaison vers la gauche (à ajuster)
        glRotatef(angleLeft, 8000.0, 10.0, 0.0);
        glScalef(1.5, 3.5, 0.9);
        remplirPoint(1.5);
        glPopMatrix();

    /**----------Oreille droite----------**/

        glPushMatrix();
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glRotatef(-angle/60,0,1,0);
        glColor3f(1,1,1);
        glTranslatef(1, 11, -8.0);
        glRotatef(-90,0.0,1.0,0.0);
        float angleright= 10.0;  // Angle d'inclinaison vers la gauche (à ajuster)
        glRotatef(angleright, -8000.0, 10.0, 0.0);
        glScalef(1.5, 3.5, 0.9);
        remplirPoint(1.5);
        glPopMatrix();


 /**-------Eyes---------------**/
          glPushMatrix();
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
            //glColor3f(1,1,1);
            glColor3f(0,0,0);
            //glTranslatef(-3,9,2.2);
            glTranslatef(-1,8,-11.5);
            remplirPoint(0.5);
            glPopMatrix();

           glPushMatrix();
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
            //glColor3f(1,1,1);
            glColor3f(0,0,0);
             glTranslatef(1,8,-11.5);
            remplirPoint(0.5);
            glPopMatrix();



        /**------sphere nez------**/

        glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(0, 7.5, -13.5);
        glBegin(GL_TRIANGLES);
            glVertex3f(-0.5f, -0.5f, 0.0f);
            glVertex3f(0.5f, -0.5f, 0.0f);
            glVertex3f(0.0f, 0.5f, 0.0f);
        glEnd();
        glRotatef(30, 1.0, 0.0, 0.0);
        glPopMatrix();

         /**---------- Bouche ----------**/
        glPushMatrix();
        glActiveTextureARB(GL_TEXTURE0_ARB);
            glColor3f(0,0,0);
            glTranslatef(0,6,-13.5);
            remplirPoint(0.5);
        glPopMatrix();



/**----------------- Pieds -----------------------**/

            glPushMatrix();
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
            glColor3f(1,1,1);
             glTranslatef(-3,-3.5,5);
            remplirPoint(1.5);
            glPopMatrix();



            glPushMatrix();
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
            glPushMatrix();
            glColor3f(1,1,1);
             glTranslatef(-3,-5,2.8);
            glRotatef(70, 1, 0, 0);
            glScalef(1.5, 3.5, 0.9);
            remplirPoint(1);
            glPopMatrix();


             glPushMatrix();
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
            glColor3f(1,1,1);
             glTranslatef(3,-3.5,4);
            remplirPoint(1.5);
            glPopMatrix();


             glPushMatrix();
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
            glPushMatrix();
            glColor3f(1,1,1);
             glTranslatef(3,-5,2.8);
            glRotatef(70, 1, 0, 0);
            glScalef(1.5, 3.5, 0.9);
            remplirPoint(1);
            glPopMatrix();



/**--------------------- Mains ---------------------**/


        glPushMatrix();
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(-2, -4, -3.0);
        glRotatef(-90,0.0,1.0,0.0);
        glScalef(1.5, 3.5, 0.9);
        remplirPoint(1);
        glPopMatrix();


        glPushMatrix();
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(2, -4, -3.0);
        glRotatef(-90,0.0,1.0,0.0);
        glScalef(1.5, 3.5, 0.9);
        remplirPoint(1);
        glPopMatrix();

        /** queue**/
        glPushMatrix();
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glColor3f(1,1,1);
       glTranslatef(0,0,8); // Translation sur l'axe vertical
       glRotatef(-30, 1.0, 0.0, 0.0);
         glScalef(1.5, 3.5, 2);
        remplirPoint(0.5);
        glPopMatrix();
   glPopMatrix();
glEnd();
}



/*********************plaquer la texture de la foret au sol*************************/
void Sol()
{
 glMatrixMode(GL_MODELVIEW);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,large,haute,0,GL_RGB,GL_UNSIGNED_BYTE,textureS);
    glEnable(GL_TEXTURE_2D);//on active la deuxieme texture qu'on a plaqué sur un cube qu'on a mise en echelle pour faire sur le sol
    glPushMatrix();
      glColor3f(1,1,1);
      glTranslatef(4,-8,0);
      glScalef(60,1,60);
      dessinCube();//appel de la methode pour dessiner le cube
    glPopMatrix();
 glEnd();
}


/*******************methode pour remplir les faces**************************/
  void remplirFace()
{
    for(int j=0;j<NP-1;j++)
    {
      for(int i=0;i<NM;i++)
       {
        fSphere[j*NM+i][0]=j*NM+i;
        fSphere[j*NM+i][1]=j*NM+(i+1)%NM;
        fSphere[j*NM+i][2]=(j+1)*NM+(i+1)%NM;
        fSphere[j*NM+i][3]=j*NM+(i+NM);

       }

    }

}


/*******************methode pour remplir les point avec un rayon en parametre*****/
 void remplirPoint(double R )
 {
    remplirFace();

    for(int j=0;j<NP;j++)
    {
      for(int i=0;i<NM;i++)

        {
            fi=-(PI/2)+(j*PI)/(NP-1);
            theta=i*(2*PI/NM);

             X[i+j*NM]=R*cos(theta)*cos(fi);
             Y[i+j*NM]=R*sin(theta)*cos(fi);
             Z[i+j*NM]=R*sin(fi);

        }
    }


  for (int i=0;i<(NP-1)*NM;i++)
    {

      glBegin(GL_POLYGON);
       //coller la texture sur une face du sphere

            glTexCoord2f((i%NM)/(float)NM, ((NP-1)-(i/NM))/(float)(NP-1));
            glVertex3f(X[fSphere[i][0]],Y[fSphere[i][0]],Z[fSphere[i][0]]);


            glTexCoord2f(((i%NM)+1)/(float)NM, ((NP-1)-(i/NM))/(float)(NP-1));
            glVertex3f(X[fSphere[i][1]],Y[fSphere[i][1]],Z[fSphere[i][1]]);

            glTexCoord2f(((i%NM)+1)/(float)NM, ((NP-2)-(i/NM))/(float)(NP-1));
            glVertex3f(X[fSphere[i][2]],Y[fSphere[i][2]],Z[fSphere[i][2]]);


            glTexCoord2f((i%NM)/(float)NM, ((NP-2)-(i/NM))/(float)(NP-1));
            glVertex3f(X[fSphere[i][3]],Y[fSphere[i][3]],Z[fSphere[i][3]]);

        glEnd();
}
}


/********************Dessiner le cube pour le coup avec des formules parametriques******/
void dessinCube()
{
       // Dessin du cube
  for (int i=0;i<6;i++)
    {
      glBegin(GL_POLYGON);
      //for (int j=0;j<4;j++){
          glTexCoord2f(0,0);
          glVertex3f(pCube[fCube[i][0]].x,pCube[fCube[i][0]].y,pCube[fCube[i][0]].z);

          glTexCoord2f(0,1);
          glVertex3f(pCube[fCube[i][1]].x,pCube[fCube[i][1]].y,pCube[fCube[i][1]].z);

          glTexCoord2f(1,1);
          glVertex3f(pCube[fCube[i][2]].x,pCube[fCube[i][2]].y,pCube[fCube[i][2]].z);

          glTexCoord2f(1,0);
          glVertex3f(pCube[fCube[i][3]].x,pCube[fCube[i][3]].y,pCube[fCube[i][3]].z);


      glEnd();
    }
}






/*************************Texture pour le sol ******************************/

void textureSol(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;
  //unsigned char image[256*256*3];
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);


#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  if ((cinfo.image_width!=large)||(cinfo.image_height!=haute)) {
    fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }

  jpeg_start_decompress(&cinfo);
  ligne=imageS;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=imageS+3*large*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

//stocke l'image dans la texture
  for(int i=0;i<haute;i++)
    {
        for(int j=0;j<large;j++)
        {
            textureS[i][j][0]=imageS[i*large*3+j*3];
            textureS[i][j][1]=imageS[i*large*3+j*3+1];
            textureS[i][j][2]=imageS[i*large*3+j*3+2];
        }
    }
}


/*************activer les touches du clavier pour faire une tache**********/

void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {

   case 'p': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f': /* affqichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* Affichage en mode sommets seuls */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'a'://metre des lignes sur les figures
      glPolygonMode(GL_FRONT,GL_FILL);
      glPolygonMode(GL_FRONT,GL_LINE);
      glutPostRedisplay();
    break;
    case 'z' : //zoomer
        affichageScaled(0.05, 0.05, 0.05);
        glutPostRedisplay();
    break;
    case 'Z' : //dezoomé
      affichageScaled(-0.05, -0.05, -0.05);
      glutPostRedisplay();
    break;

    case 'h' : // bouge en haut
        affichageTranslated(0.0,0.1,0.0);
        glutPostRedisplay();
    break;
    case 't' : // bouge a droite
        affichageTranslated(0.1,0.0,0.0);
        glutPostRedisplay();
    break;
    case 'g' : // bouge a gauche
        affichageTranslated(-0.1,0.0,0.0);
        glutPostRedisplay();
    break;
    case 'b' : // bouge en bas
        affichageTranslated(0.0,-0.1,0.0);
        glutPostRedisplay();
    break;
    case 'w': /* changer la couleur de la lumière aléatoirement */
     srand(time(NULL)); // initialise le generateur de maniere aleatoire
      lb=rand()/(RAND_MAX+1.0);
      lr=rand()/(RAND_MAX+1.0);
      lj=rand()/(RAND_MAX+1.0);
      glutPostRedisplay();
      break;
    case 'i': /* affichage illumination */
       illumination();
       glutPostRedisplay();
      break;
    case 13 : /*la touche 'q' permet de quitter le programme */
      {
        static int anim=1;
        anim=!anim;
        glutIdleFunc((anim)? idle : NULL);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        lr=lb=lj=1.0;
        glutPostRedisplay();
      }
     break;
    case 'q' : /*la touche 'q' permet de quitter le programme */
      exit(0);

    }
}

/********************remodeler la vue********************/

void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else
    glViewport((x-y)/2,0,y,y);
}

/*********************souris*****************************/

void mouse(int button, int state,int x,int y)
{
  /* si on appuie sur le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    presse = 1; /* le booleen presse passe a 1 (vrai) */
    xold = x; /* on sauvegarde la position de la souris */
    yold=y;
  }
  /* si on relache le bouton gauche */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    presse=0; /* le booleen presse passe a 0 (faux) */
}

/********************mouvement de souris*****************/

void mousemotion(int x,int y)
  {
    if (presse) /* si le bouton gauche est presse */
    {
      /* on modifie les angles de rotation de l'objet
	 en fonction de la position actuelle de la souris et de la derniere
	 position sauvegardee */
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
    }

    xold=x; /* sauvegarde des valeurs courante de le position de la souris */
    yold=y;

  }

/************************Redimensionner******************/
 void redim(int l,int h)
{
  if (l<h)
    glViewport(0,(h-l)/2,l,l);
  else
    glViewport((l-h)/2,0,h,h);
}

/**********************Methode pour faire l'animation *********/

void idle(void)
 {
    angle+=2.0;
    if(angle>360)
        angle-=360;


glutPostRedisplay();

  }


/****fonction qui permet la rotation de la caméra avec les flèches directionelle**/

void GestionSpecial(int key, int x, int y)
  {
    switch (key)
    {
     case GLUT_KEY_LEFT ://rotation camera vers la droite
        anglex -= 2;
        glutPostRedisplay();
     break;
     case GLUT_KEY_RIGHT : //rotation de la camera vers la gauche
          anglex += 2;
          glutPostRedisplay();
     break;
     case GLUT_KEY_DOWN : //rotation de la caméra en bas vers le haut
          angley += 2;
          glutPostRedisplay();
     break;
     case GLUT_KEY_UP : //rotation de la camera en haut bas
          angley -= 2;
          glutPostRedisplay();
     break;
    }
}

/********************fonction pour gerer le zoom et le dezoom**************/

void affichageScaled(GLfloat scalex,GLfloat scaley, GLfloat scalez)//zoom dezoom
{
      Scalexc +=scalex;
      Scaleyc +=scaley;
      Scalezc +=scalez;
}

/*****************************fonction pour gere le deplacement du lapin***/

void affichageTranslated(GLfloat translatex,GLfloat translatey, GLfloat translatez)//bouger
{
    Translatex +=translatex;
    Translatey +=translatey;
    Translatez +=translatez;
}


/*****************Methode pour faire illumination***************************/

void illumination()
{

GLfloat lum_ambient[]={0.0,1.0,0.2,1.0};
GLfloat lum_diffu[]={1.0,1.0,1.0,1.0};
GLfloat lum_specul[]={1.0,1.0,1.0,1.0};
GLfloat lum_positi[]={-2.0,2.0,1.0,1.0};

    glLightfv(GL_LIGHT0,GL_AMBIENT,lum_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lum_diffu);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lum_specul);
    glLightfv(GL_LIGHT0,GL_POSITION,lum_positi);
    glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.5);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.5);
    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.2);

   glEnable(GL_LIGHT0);
}

