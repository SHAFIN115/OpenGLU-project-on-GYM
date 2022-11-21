#include <windows.h>
#include <mmsystem.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include<math.h>
#include<bits/stdc++.h>


using namespace std;

unsigned int ID,ID1;

double Txval=0,Tyval=0,Tzval=0;
double windowHeight=1500, windowWidth=1500;
GLfloat alpha = 0.0, theta = 0.0, gamma = 0.0, axis_x=0.0, axis_y=0.0, sandAngle=90, kappa=0.0;
bool fan4=false;
bool shoulderLeft = false;
bool squeeze=true, release=false;
bool swingBehind=true, swingAhead=true;
double lpan=0,rpan=0,bpan=0,tpan=0;

GLdouble eyex=20, eyey=50,eyez=150,centerx=14,centery=50,centerz=0, upx=0,upy=1,upz=0, zNear=4, zFar=250;
GLdouble dx,dy,dz;
// original -> 20,6,-20, 2,0,0, 0,1,0
double scale_x = 2;
double scale_y = 2;
double scale_z = 2;

GLboolean bRotate = false, uRotate = false, light0 = false,light1=false,light2=false, sandbagMov=false;

#define PI 3.1415927

static GLfloat v_cube[8][3] =
{
    {0, 0, 0}, //0
    {0, 0, 1}, //1
    {1, 0, 1}, //2
    {1, 0, 0}, //3
    {0, 1, 0}, //4
    {0, 1, 1}, //5
    {1, 1, 1}, //6
    {1, 1, 0}  //7
};

static GLubyte cubeIndices[6][4] =
{
    {0, 1, 2, 3}, //bottom
    {4, 5, 6, 7}, //top
    {5, 1, 2, 6}, //front
    {0, 4, 7, 3}, // back is clockwise
    {2, 3, 7, 6}, //right
    {1, 5, 4, 0}  //left is clockwise
};


static GLfloat v_cube0[8][3] =
{
    {-.5, -.5, -.5}, //0
    {-.5, -.5, .5}, //1
    {.5, -.5, .5}, //2
    {.5, -.5, -.5}, //3
    {-.5, .5, -.5}, //4
    {-.5, .5, .5}, //5
    {.5, .5, .5}, //6
    {.5, .5, -.5}  //7
};

static GLubyte cubeIndices0[6][4] =
{
    {0, 1, 2, 3}, //bottom
    {4, 5, 6, 7}, //top
    {5, 1, 2, 6}, //front
    {0, 4, 7, 3}, // back is clockwise
    {2, 3, 7, 6}, //right
    {1, 5, 4, 0}  //left is clockwise
};



static GLfloat v_cubePULL[8][3] =
{
    {0,-1,0}, //0
    {0,-1,1}, //1
    {1,-1,1}, //2
    {1,-1,0}, //3
    {0,0,0}, //4
    {0,0,1}, //5
    {1,0,1}, //6
    {1,0,0}  //7
};

static GLubyte cubeIndicesPULL[6][4] =
{
    {0, 1, 2, 3}, //bottom
    {4, 5, 6, 7}, //top
    {5, 1, 2, 6}, //front
    {0, 4, 7, 3}, // back is clockwise
    {2, 3, 7, 6}, //right
    {1, 5, 4, 0}  //left is clockwise
};




static GLfloat v_floor[8][3] =
{
    {-14,15,25}, //0
    {-14,15,0}, //1
    {-14,0,0}, //2
    {-14,0,25}, //3

    {25,15,0}, //4
    {25,0,0}, //5
    {25,15,25}, //6
    {25,0,25}, //7


};



static GLubyte florindices[2][4] =
{
    //{0,1,2,3},
    {1,4,5,2},
    //{4,6,7,5},
    {5,7,3,2}
};



void drawWall(GLfloat r=1,GLfloat g=1,GLfloat b=1,GLboolean emission=false)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {90};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    for (GLint i = 0; i <2; i++)
    {
        glBegin(GL_QUADS);
        glVertex3fv(&v_floor[florindices[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_floor[florindices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_floor[florindices[i][2]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_floor[florindices[i][3]][0]);
        glTexCoord2f(0,1);

        glEnd();
    }
}



static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void drawCube(GLfloat r=1,GLfloat g=1,GLfloat b=1,GLboolean emission=false)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    for (GLint i = 0; i <6; i++)
    {
        glBegin(GL_QUADS);
        glVertex3fv(&v_cube[cubeIndices[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_cube[cubeIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cube[cubeIndices[i][2]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[cubeIndices[i][3]][0]);
        glTexCoord2f(0,1);
        glEnd();
    }
}

class BmpLoader
{
public:
    unsigned char* textureData;
    int iWidth, iHeight;

    BmpLoader(const char*);
    ~BmpLoader();

private:
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
};

BmpLoader::BmpLoader(const char* filename)
{
    FILE *file=0;
    file=fopen(filename, "rb");
    if(!file)
        cout<<"File not found"<<endl;
    fread(&bfh, sizeof(BITMAPFILEHEADER),1,file);
    if(bfh.bfType != 0x4D42)
        cout<<"Not a valid bitmap"<<endl;
    fread(&bih, sizeof(BITMAPINFOHEADER),1,file);
    if(bih.biSizeImage==0)
        bih.biSizeImage=bih.biHeight*bih.biWidth*3;
    textureData = new unsigned char[bih.biSizeImage];
    fseek(file, bfh.bfOffBits, SEEK_SET);
    fread(textureData, 1, bih.biSizeImage, file);
    unsigned char temp;
    for(int i=0; i<bih.biSizeImage; i+=3)
    {
        temp = textureData[i];
        textureData[i] = textureData[i+2];
        textureData[i+2] = temp;

    }

    iWidth = bih.biWidth;
    iHeight = bih.biHeight;
    fclose(file);
}

BmpLoader::~BmpLoader()
{
    delete [] textureData;
}


void drawCube0(GLfloat r=1,GLfloat g=1,GLfloat b=1,GLboolean emission=false)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    for (GLint i = 0; i <6; i++)
    {
        glBegin(GL_QUADS);
        glVertex3fv(&v_cube0[cubeIndices0[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube0[cubeIndices0[i][1]][0]);
        glTexCoord2f(0,1);
        glVertex3fv(&v_cube0[cubeIndices0[i][2]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_cube0[cubeIndices0[i][3]][0]);
        glTexCoord2f(1,0);
        glEnd();
    }
}


void drawCubePULL(GLfloat r=1,GLfloat g=1,GLfloat b=1,GLboolean emission=false)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    for (GLint i = 0; i <6; i++)
    {
        glBegin(GL_QUADS);
        glVertex3fv(&v_cubePULL[cubeIndicesPULL[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_cubePULL[cubeIndicesPULL[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cubePULL[cubeIndicesPULL[i][2]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cubePULL[cubeIndicesPULL[i][3]][0]);
        glTexCoord2f(0,1);
        glEnd();
    }
}

static GLfloat v_pyramid[12][3] =
{
    {0.0, 1.0, 0.0}, //0
    {-1.0, 0.5, 0.0}, //1
    {-1.0, -0.5, 0.0}, //2
    {0.0, -1.0, 0.0}, //3
    {1.0, -0.5, 0.0}, //4
    {1.0, 0.5, 0.0}, //5

    {0.0, 1.0, -1}, //6
    {-1.0, 0.5, -1}, //7
    {-1.0, -0.5, -1}, //8
    {0.0, -1.0, -1}, //9
    {1.0, -0.5, -1}, //10
    {1.0, 0.5, -1} //11
};

static GLubyte poly_Indices[2][6] =
{
    {0,1,2,3,4,5},
    {6,7,8,9,10,11}
};

static GLubyte quadIndices[6][4] =
{
    {0,6,7,1},
    {0,5,11,6},
    {1,7,8,2},

    {5,4,10,11},
    {2,3,9,8},
    {4,3,9,10}
};

void drawpyramid()
{

    for (GLint i = 0; i <2; i++)
    {
        glBegin(GL_POLYGON);
        glColor3f(0,0,0);
        glVertex3fv(&v_pyramid[poly_Indices[i][0]][0]);
        glTexCoord3f(1,1,1);
        glVertex3fv(&v_pyramid[poly_Indices[i][1]][0]);
        glTexCoord3f(1,0,1);
        glVertex3fv(&v_pyramid[poly_Indices[i][2]][0]);
        glTexCoord3f(0,1,1);
        glVertex3fv(&v_pyramid[poly_Indices[i][3]][0]);
        glTexCoord3f(1,0,1);
        glVertex3fv(&v_pyramid[poly_Indices[i][4]][0]);
        glTexCoord3f(1,1,0);
        glVertex3fv(&v_pyramid[poly_Indices[i][5]][0]);
        glTexCoord3f(0,0,1);
    }
    glEnd();

    for (GLint i = 0; i <6; i++)
    {
        glBegin(GL_QUADS);
        glColor3f(0,0,0);
        glVertex3fv(&v_pyramid[quadIndices[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_pyramid[quadIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_pyramid[quadIndices[i][2]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_pyramid[quadIndices[i][3]][0]);
        glTexCoord2f(0,1);

    }
    glEnd();

}

void dumbbell(double weight = 5.0)
{
    double factor;
    if (weight==5.0) factor = 1;
    else factor = weight*0.5;

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(1,1,1);
    glScalef(0.1,0.02,0.8);
    glBindTexture(GL_TEXTURE_2D, 9);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(0,0,0);
    glScalef(0.5,0.4,0.3*factor);
    glBindTexture(GL_TEXTURE_2D, 10);
    drawpyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(0,0,0.8);
    glScalef(0.5,0.4,0.3*factor);

    glBindTexture(GL_TEXTURE_2D, 10);
    drawpyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void rack()
{

    // left leg
    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(-15,0,0);
    glScalef(0.5,12,2);
    drawCube(1,1,1);
    glPopMatrix();

    // right leg
    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(22,0,0);
    glScalef(0.5,11,2);
    drawCube(1,1,1);
    glPopMatrix();

    // bottom rack
    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(-15,0.8,0);
    glScalef(38,0.5,3);
    drawCube(0,0,0);
    glPopMatrix();

    // middle rack
    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(-15,2.8,0);
    glScalef(38,0.5,3);
    drawCube(0,0,0);
    glPopMatrix();

    // top rack
    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(-15,4.8,0);
    glScalef(38,0.5,3);
    drawCube(0,0,0);
    glPopMatrix();


    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(-15,6.8,0);
    glScalef(38,0.5,3);
    drawCube(0,0,0);
    glPopMatrix();


    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(-15,8.8,0);
    glScalef(38,0.5,3);
    drawCube(0,0,0);
    glPopMatrix();


    glPushMatrix();
    glColor3b(1,1,1);
    glTranslatef(-15,10.8,0);
    glScalef(38,0.5,3);
    drawCube(0,0,0);
    glPopMatrix();




}

void drawFan(GLfloat r=0,GLfloat g=0,GLfloat b=0,GLboolean emission=false)
{

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    glPushMatrix();

    glTranslatef(0,0,0);
    glScalef(0.1,0.1,1);
    drawCube();

    glPopMatrix();

}

void drawFanJack(GLfloat r=0,GLfloat g=0,GLfloat b=0,GLboolean emission=false)
{

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    glPushMatrix();

    glTranslatef(0,0,0);
    glScalef(0.1,0.1,2);
    drawCube();
    glutSolidSphere(2,6,6);

    glPopMatrix();

}

void ceiling_light_holder(GLfloat r,GLfloat g,GLfloat b,GLboolean emission=false)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    glPushMatrix();
    glTranslatef(3.48,8,4.48);
    //glRotatef(-90,1,0,0);
    glScalef(0.1,1,0.1);
    drawCube(1,1,1);
    glPopMatrix();
}

void ceiling_light(GLfloat r,GLfloat g,GLfloat b,GLfloat x=3.5,GLfloat y=8,GLfloat z=4.5,GLboolean emission=false)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    glPushMatrix();
    glTranslatef(x,y,z);
    glutSolidSphere(0.5,16,20);
    glPopMatrix();
}

void light(double x,double y,double z,bool lighton,GLenum Lights,bool spot,bool strip)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat strip_light_diffuse[]  = { 1.0, 1.0, 0.8, 1.0 };
    GLfloat strip_light_specular[] = { 1.0, 1.0, 0.8, 1.0 };
    GLfloat light_position[] = { x, y, z, 1.0 };

    glEnable(Lights);
    if(lighton)
    {
        if(strip)
        {
            glLightfv( Lights, GL_AMBIENT, light_ambient);
            glLightfv( Lights, GL_DIFFUSE, strip_light_diffuse);
            glLightfv( Lights, GL_SPECULAR, strip_light_specular);
            glLightfv( Lights, GL_POSITION, light_position);
        }

        else
        {
            glLightfv( Lights, GL_AMBIENT, light_ambient);
            glLightfv( Lights, GL_DIFFUSE, light_diffuse);
            glLightfv( Lights, GL_SPECULAR, light_specular);
            glLightfv( Lights, GL_POSITION, light_position);
        }

    }
    else
    {
        glLightfv( Lights, GL_AMBIENT, no_light);
        glLightfv( Lights, GL_DIFFUSE, no_light);
        glLightfv( Lights, GL_SPECULAR, no_light);
        glLightfv( Lights, GL_POSITION, no_light);
    }

    if(spot)
    {
        GLfloat spot_direction[] = { 0, -1, 0 };
        glLightfv(Lights, GL_SPOT_DIRECTION, spot_direction);
        glLightf(Lights, GL_SPOT_CUTOFF, 55.0);
    }

}

void chair(int r=0)
{
    //legs left top
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    //(1,1,1);
    glTranslatef(-1.45,0,-1.2);
    glScaled(0.2,1.5,0.2);

    glBindTexture(GL_TEXTURE_2D, 7);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //legs left bottom
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    //(1,1,1);
    glTranslatef(-1.45,0,1.3);
    glScaled(0.2,1.5,0.2);

    glBindTexture(GL_TEXTURE_2D, 7);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // right top legs
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    //(1,1,1);
    glTranslatef(1.55,0,-1.2);
    glScaled(0.2,1.5,0.2);

    glBindTexture(GL_TEXTURE_2D, 7);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // right bottom legs
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    //(1,1,1);
    glTranslatef(1.55,0,1.3);
    glScaled(0.2,1.5,0.2);

    glBindTexture(GL_TEXTURE_2D, 7);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // seat
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    glTranslatef(-1.45,1.5,-1.2);
    glScaled(3.2,0.2,2.7);

    glBindTexture(GL_TEXTURE_2D, 7);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // backrest
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    //(1,1,1);
    glTranslatef(-1.45,1.5,-1.2);
    glScaled(0.2,2.5,2.7);

    glBindTexture(GL_TEXTURE_2D, 7);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}


void LoadTexture(const char*filename)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

int i = 1;
GLuint ID_[100];

void LoadTexture1(const char*filename)
{
    glGenTextures(1, &ID_[i]);
    glBindTexture(GL_TEXTURE_2D, ID_[i]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID_[i++]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

void draw_cylinder(GLfloat radius,
                   GLfloat height,
                   GLubyte R,
                   GLubyte G,
                   GLubyte B)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glColor3ub(R-40,G-40,B-40);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while( angle < 2*PI )
    {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        glVertex3f(x, y, 0.0);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height);
    glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3ub(R,G,B);
    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*PI )
    {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y, height);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height);
    glEnd();
}



void cyl(GLfloat r=0.3,
         GLfloat g=0.4,
         GLfloat b=0.5,
         GLboolean emission = false )
{

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r, g, b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};

    GLfloat mat_em[] = {1.0,1.0,1.0,1.0};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(emission) glMaterialfv( GL_FRONT, GL_EMISSION, mat_em);
    else glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    GLUquadric* qobj;
    qobj = gluNewQuadric();
    gluCylinder(qobj,0.5,0.5,3,20,10);
}
void secondFloor();

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum(-5+lpan, 5+rpan, -5+bpan, 5+tpan, zNear, zFar);
    //gluPerspective(60,1,5,100);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyex,eyey,eyez,centerx+dx,centery+dy,centerz+dz,upx,upy,upz);
    glViewport(0, 0, windowHeight, windowWidth);

    glScalef(scale_x,scale_y,scale_z);

    glRotatef( alpha,axis_x, axis_y, 0.0 );
    glRotatef( theta, axis_x, axis_y, 0.0 );

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, 4);
    glTranslatef(-20,0,0);
    glScalef(3,1,1);
    drawWall();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, 4);
    glTranslatef(-20,30,0);
    glScalef(3,1,1);
    drawWall();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    {
        /// Floor right
        /// Floor right


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(25,0,0);
        //glRotatef(22, 0,0,1);
        glScalef(25,0.1,25);
        glBindTexture(GL_TEXTURE_2D, 3);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }

    {
        /// Wall right back

        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(25,0,0);
        //glRotatef(22, 0,0,1);
        glScalef(25,15,0.1);
        glBindTexture(GL_TEXTURE_2D, 2);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }

    {
        /// Wall right side
        // last er dun side we brown wall ta
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(55,0,0);
        //glRotatef(22, 0,0,1);
        glScalef(0.1,15,25);
        glBindTexture(GL_TEXTURE_2D, 1);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


    }

    {
        /// staircase
    auto siri=[&](){
        int st=14,nxt=0;
        while(st>0)
        {
            glEnable(GL_TEXTURE_2D);
            glPushMatrix();
            glTranslatef(21,0,nxt);
            //glRotatef(22, 0,0,1);
            glScalef(4,st,1);
            glBindTexture(GL_TEXTURE_2D, 9);
            drawCube(1,1,1);
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);

            st-=1;
            nxt++;
        }
    };
    glPushMatrix();
        glTranslatef(-120,0,0);
        glScalef(4,1,1);
        //glRotatef(22, 0,0,1);
        glScalef(1,1,1);
        glBindTexture(GL_TEXTURE_2D, 1);
        siri();
        glPopMatrix();

    }

    {
        /// TILES
        /*
        // tiles outline
        //(.8,.7,.2);
        glPushMatrix();
        glTranslatef(-14,0,0);
        //glRotatef(22, 0,0,1);
        glScalef(23, 0.002, 20);
        drawCube(.8,.7,.2);
        glPopMatrix();
        //tiles
        int start_point=-1;
        for(int k=9;k<20;k++){

        start_point = (start_point+1)%2;

        for (int i=0;i<10;i+=2){
            //(.8,.7,.2);
            glPushMatrix();

            glTranslatef(k,0,i+start_point);
            //glRotatef(22, 0,0,1);
            glScalef(1, 0.002, 1);
            drawCube(.8,.7,.2);
            glPopMatrix();
        }

        }
        */
    }

    /// DUMBBELLS========================================

    {


        glPushMatrix();
        glTranslated(-4,1.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(0,1.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(0,1.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(3,1.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(1,1.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(0,3.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(3,3.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(1,3.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-3,3.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-1,3.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-3,5.7,1);
        dumbbell();
        glPopMatrix();


        glPushMatrix();
        glTranslated(0,5.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(3,5.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(1,5.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-3,5.7,1);
        dumbbell();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-1,5.7,1);
        dumbbell();
        glPopMatrix();

    }
    /// DUMBBELLS========================================

    glPushMatrix();

    rack();
    glPopMatrix();

    /// RACK ========================================

    /// COUNTER ====================================
    {
        // cupboards
        //(1.0, 0.5, 0.0);


//        glEnable(GL_TEXTURE_2D);
//        glPushMatrix();
//        glTranslatef(3,0,11);
//        //glRotatef(22, 0,0,1);
//        glScalef(4,4,2);
//        glBindTexture(GL_TEXTURE_2D, 3);
//        drawCube(1,1,1);
//        glPopMatrix();
//        glDisable(GL_TEXTURE_2D);

        // cupboards
        //(1.0, 0.5, 0.0);
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(1.5,0,13);
        glRotatef(0, 0,0,1);
        glScalef(5,3,7);
        glBindTexture(GL_TEXTURE_2D, 6);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        /// COUNTER

        /// chair
        auto ch_3=[&]()
        {
            glBindTexture(GL_TEXTURE_2D, 1);
            glPushMatrix();
            glTranslatef(-2,0,15);
            //glRotatef(180,0,1,0);
            chair();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-2,0,10);
            //glRotatef(180,0,1,0);
            chair();
            glPopMatrix();

            glPushMatrix();
            glTranslatef(-2,0,20);
            //glRotatef(180,0,1,0);
            chair();
            glPopMatrix();
            // chair boshano lagle eta
//            glPushMatrix();
//            glTranslatef(-2,10,40);
//            //glRotatef(180,0,1,0);
//            chair();
//            glPopMatrix();


        };
         glPushMatrix();
        glTranslatef(0,0,0);
        //glRotatef(180,0,1,0);
        ch_3();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-5,0,0);
        //glRotatef(180,0,1,0);
        ch_3();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(8,0,30);
        glRotatef(180,0,1,0);
        ch_3();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(30,0,0);
        //glRotatef(180,0,1,0);
        ch_3();
        glPopMatrix();

    }
    /// COUNTER ====================================

    auto body=[&]{
    {
        // sandbag handle

        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(10, 8, 10);
        //glRotatef(22, 0,0,1);
        glScalef(0.1,7,0.1);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }

    {
        /// cyl=====================================

        glPushMatrix();

        glTranslatef(10, 8, 10);
        glRotatef(sandAngle, 1, 0, 0 );
        //glRotatef(sandAngle, 0, 0, 1 );
        glScalef(2, 2, 2);
        cyl( .5,.3,.4);
        glPopMatrix();

    }
    };
        glPushMatrix();
        glTranslatef(0, 15, 0);
        body();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-5, 15, 0);
        body();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-10, 15, 0);
        body();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(25, 15, 0);
        body();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(30, 15, 0);
        body();
        glPopMatrix();

    secondFloor();

    /// Lockers

    {
        for(int y=2; y<=7; y++)
        {
            for(int x=27; x<34; x++)
            {
                glEnable(GL_TEXTURE_2D);
                glPushMatrix();
                glTranslatef(x, y, 1);
                glScalef(1,1,1);
                glBindTexture(GL_TEXTURE_2D, 8);
                drawCube(1,1,1);
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
            }
        }
    }

    {
        for(int y=17; y<=24; y++)
        {
            for(int x=27; x<34; x++)
            {
                glEnable(GL_TEXTURE_2D);
                glPushMatrix();
                glTranslatef(x, y, 1);
                glScalef(1,1,1);
                glBindTexture(GL_TEXTURE_2D, 8);
                drawCube(1,1,1);
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
            }
        }
    }


    {
        ///LIGHT
        // ========================================

        light(26,8.83,4.5,light0,GL_LIGHT0,false,false);
        light(3.5,8,4.5,light1,GL_LIGHT1,true,false);
        light(18,12,15,light2,GL_LIGHT2,false,true);


    }

    glFlush();
    glutSwapBuffers();
}

void secondFloor()
{
    /// FLOOR
    {


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-14,15,0);
        //glRotatef(22, 0,0,1);
        glScalef(40,0.1,25);
        glBindTexture(GL_TEXTURE_2D, 3);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    /// FLOOR left
    {


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-60,15,0);
        //glRotatef(22, 0,0,1);
        glScalef(44,0.1,25);
        glBindTexture(GL_TEXTURE_2D, 3);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    /// Floor right
    {


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(25,15,0);
        //glRotatef(22, 0,0,1);
        glScalef(30,0.1,25);
        glBindTexture(GL_TEXTURE_2D, 3);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    /// Floor connector
    {


//        glEnable(GL_TEXTURE_2D);
//        glPushMatrix();
//        glTranslatef(21,15,8);
//        //glRotatef(22, 0,0,1);
//        glScalef(4,0.02,17);
//        glBindTexture(GL_TEXTURE_2D, 12);
//        drawCube(1,1,1);
//        glPopMatrix();
//        glDisable(GL_TEXTURE_2D);
    }
    /// Floor hider
    {


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-21,15.1,8);
        //glRotatef(22, 0,0,1);
        glScalef(7,0.1,17);
        glBindTexture(GL_TEXTURE_2D, 1);
        //glRotated(180,0,0,1);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    /// partition wall
    {


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(25,15,0);
        //glRotatef(22, 0,0,1);
        glScalef(.2,15,8);
        glBindTexture(GL_TEXTURE_2D, 3);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    /// walls back 2nd floor
    {

        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-45,15,0);
        //glRotatef(22, 0,0,1);
        glScalef(95,15,0.1);
        glBindTexture(GL_TEXTURE_2D, 2);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    /// walls side 2nd floor
    {

        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(55,15,0);
        //glRotatef(22, 0,0,1);
        glScalef(0.1,15,25);
        glBindTexture(GL_TEXTURE_2D, 1);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


    }

    /// walls side left 2nd floor

    {

        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-60,0,0);
        //glRotatef(22, 0,0,1);
        glScalef(0.1,30,25);
        glBindTexture(GL_TEXTURE_2D, 1);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


    }

    double base = 15.1;

    /// ALI
    {

        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-44.8,22.5,5);
        //glRotatef(22, 0,0,1);
        glScalef(.1,14,10);
        glBindTexture(GL_TEXTURE_2D, 2);
        drawCube0(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }


    {

        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-59,8,20);
        //glRotatef(-22, 30,870,11);
        glScalef(.1,15,10);
        glBindTexture(GL_TEXTURE_2D, 9);
        drawCube0(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }

























    ///Boxing Ring
    auto ring=[&]()
    {
        // base
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base,5);
        //glRotatef(22, 0,0,1);
        glScalef(14,2,10);
        glBindTexture(GL_TEXTURE_2D, 8);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        // pillars
        // left top
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2,5);
        //glRotatef(22, 0,0,1);
        glScalef(0.5,5,0.5);
        glBindTexture(GL_TEXTURE_2D, 9);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // left bottom
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2,14.5);
        //glRotatef(22, 0,0,1);
        glScalef(0.5,5,0.5);
        glBindTexture(GL_TEXTURE_2D, 9);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // right top
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(1.5,base+2,5);
        //glRotatef(22, 0,0,1);
        glScalef(0.5,5,0.5);
        glBindTexture(GL_TEXTURE_2D, 9);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // right bottom
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(1.5,base+2,14.5);
        //glRotatef(22, 0,0,1);
        glScalef(0.5,5,0.5);
        glBindTexture(GL_TEXTURE_2D, 9);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


        // ropes
        // top 1
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+1,5);
        //glRotatef(22, 0,0,1);
        glScalef(14,0.2,0.2);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // top 2
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+2.5,5);
        //glRotatef(22, 0,0,1);
        glScalef(14,0.2,0.2);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // top 3
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+4,5);
        //glRotatef(22, 0,0,1);
        glScalef(14,0.2,0.2);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        // bottom
        // bottom 1
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+1,14.5);
        //glRotatef(22, 0,0,1);
        glScalef(14,0.2,0.2);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // bottom 2
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+2.5,14.5);
        //glRotatef(22, 0,0,1);
        glScalef(14,0.2,0.2);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // bottom 3
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+4,14.5);
        //glRotatef(22, 0,0,1);
        glScalef(14,0.2,0.2);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        // left
        // left 1
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+1,5);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,0.2,10);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // left 2
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+2.5,5);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,0.2,10);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // left 3
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-12,base+2+4,5);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,0.2,10);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // right
        // right 1
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(1.6,base+2+1,5);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,0.2,10);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // right 2
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(1.6,base+2+2.5,5);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,0.2,10);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        // right 3
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(1.6,base+2+4,5);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,0.2,10);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    };
     glPushMatrix();
        glTranslatef(45, -15, 0);
        ring();
        glPopMatrix();
    /// Boxing ring end =============


    /// spectator seats=====
    {
        for(int i=0; i<4; i++)
        {
            glPushMatrix();
             glTranslated(-7,base,5+i*5);
            chair();
            glPopMatrix();
        }
    }

    /// more sandbags
    {
        // sandbag handle

        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-24, 23, 10);
        //glRotatef(22, 0,0,1);
        glScalef(0.1,7,0.1);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    {
        /// cyl=====================================

        glPushMatrix();

        glTranslatef(-24, 23, 10);
        glRotatef(sandAngle, 1, 0, 0 );
        //glRotatef(sandAngle, 0, 0, 1 );
        glScalef(2, 2, 2);
        cyl( .8,.9,.6);
        glPopMatrix();

    }

    {
        // sandbag handle
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-34, 23, 10);
        //glRotatef(22, 0,0,1);
        glScalef(0.1,7,0.1);
        glBindTexture(GL_TEXTURE_2D, 10);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }

    {
        /// cyl=====================================

        glPushMatrix();

        glTranslatef(-34, 23, 10);
        glRotatef(sandAngle, 1, 0, 0 );
        //glRotatef(sandAngle, 0, 0, 1 );
        glScalef(2, 2, 2);
        cyl( .5,.1,.8);
        glPopMatrix();

    }

    // cyl circle=====================================
    /// PULL UP BAR
    {


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-45,26,15);
        //glRotatef(22, 0,0,1);
        glScalef(8,0.2,0.2);
        glBindTexture(GL_TEXTURE_2D, 15);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-37,15,15);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,11,0.2);
        glBindTexture(GL_TEXTURE_2D, 15);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);



        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-45,26,20);
        //glRotatef(22, 0,0,1);
        glScalef(8,0.2,0.2);
        glBindTexture(GL_TEXTURE_2D, 15);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-37,15,20);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,11,0.2);
        glBindTexture(GL_TEXTURE_2D, 15);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }

   /// jetaaaa ghurree
    /// PULL UP MANequin
    {
        //right arm
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-40,24.5,20);
        //glRotatef(22, 0,0,1);
        glScalef(0.8,1.5,0.2);
        glBindTexture(GL_TEXTURE_2D, 15);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        //left arm
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-43,24.5,20);
        //glRotatef(22, 0,0,1);
        glScalef(0.2,1.5,0.2);
        glBindTexture(GL_TEXTURE_2D, 15);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


        //right shoulder2
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-40,24.5,20);
        //glRotatef(22, 0,0,1);
        glScalef(2,1.5,.2);
        glRotatef(-kappa,0,0,1);
        glBindTexture(GL_TEXTURE_2D, 10);
        //glRotatef(30,0,1,0);
        drawCubePULL(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


        //left shoulder1
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-43,24.5,20);
        //glRotatef(22, 0,1,0);
        glScalef(2,1.5,.2);
        glRotatef(kappa,0,0,1);
        glBindTexture(GL_TEXTURE_2D, 10);
        //glRotatef(-30,0,1,0);
        drawCubePULL(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }
}

double limit = 360;
double roll_value = 0.2;
//GLfloat angleYaw = 90.0, anglePitch = 270.0, angleRoll = 90.0;
//GLfloat angleYawLimit1 = 180.0, anglePitchLimit1 = 360.0, angleRollLimit1 = 270.0;
//GLfloat angleYawLimit2 = 0.0, anglePitchLimit2 = 180.0, angleRollLimit2 = -90.0;
int f=0;
double yaw=0,pitch=0,roll=0;
int bv = 0;

void b_swap()
{
    if(bv==1)
    {
        int tmp = eyey;
        eyey = eyez;
        eyez = tmp;

        tmp = upy;
        upy = upz;
        upz = tmp;

    }
}
void bird_view()
{
//     init(6);
//     f=6;
    bv = 1;
    b_swap();

}


void normal_view()
{
//     init(6);
//     f=6;
    bv = 0;
    eyex=20, eyey=10,eyez=70,centerx=12,centery=10,centerz=0, upx=0,upy=1,upz=0;

}

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    ///yaw
    case 'y':

        yaw+=1;

        if( yaw==90 || yaw ==0)
        {
            yaw=0;
            dx=0;
            break;
        }
        dx =  abs(eyez-centerz)*tan(yaw*3.1415/180);

        centery=centery;
        break;

    case 'u':

        yaw-=1;

        if(yaw==-90 || yaw==0)
        {
            yaw = 0;
            dx=0;
            break;
        }

        dx = abs(eyez-centerz)*tan(yaw*3.1415/180);

        centery=centery;

        break;

    ///pitch
    case 'r':

        pitch+=1;

        if( pitch==90 || pitch ==0)
        {
            pitch=0;
            dy=0;
            break;
        }
        dy =  abs(eyez-centerz)*tan(pitch*3.1415/180);

        centerx=centerx;
        break;

    case 't':
        pitch-=1;
        if(pitch==-90 || pitch==0)
        {
            pitch = 0;
            dy=0;
            break;
        }
        dy = abs(eyez-centerz)*tan(pitch*3.1415/180);

        centerx=centerx;
        break;


    ///    roll
    case 'i':
        roll+=0.5;
        upx = cos(roll);
        upy= sin(roll);

        break;
    case 'o':
        roll-=0.5;

        upx = cos(roll);
        upy= sin(roll);
        break;



    case '@':
        bRotate = !bRotate;
        // uRotate = false;
        axis_x=0.0;
        axis_y=1.0;

        break;


    case '#':
        uRotate = !uRotate;/**<  */
        bRotate = false;
        axis_x=1.0;
        axis_y=0.0;

        break;

    case 'z':
        //eyez-=2;
        //init(4);
        f=4;
        scale_x+=0.05;
        scale_y+=0.05;
        scale_z+=0.05;
        glutPostRedisplay();
        break;
    case 'c':
        //eyez-=2;
        //init(4);
        f=4;
        if(scale_x==0.05)break;
        scale_x-=0.05;
        scale_y-=0.05;
        scale_z-=0.05;
        glutPostRedisplay();
        break;

    //switch for light0
    case '0':
        light0 = !light0;
        glutPostRedisplay();
        break;
    case '1':
        light1 = !light1;
        glutPostRedisplay();
        break;
    case '2':
        light2 = !light2;
        glutPostRedisplay();
        break;

    case '4':
        fan4 = !fan4;
        glutPostRedisplay();
        break;

    case '5':
        sandbagMov = !sandbagMov;
        glutPostRedisplay();
        break;
    case '6':
        shoulderLeft = !shoulderLeft;
        glutPostRedisplay();
        break;

    // bird's eye view
    case '%':
        bv=1;
        bird_view();
        break;
    case '$':
        normal_view();
        break;

    ///panning
    case 'g':
        lpan-=1;
        rpan-=1;
        glutPostRedisplay();
        break;

    case 'h':
        lpan+=1;
        rpan +=1;
        glutPostRedisplay();
        break;

    case ';':
        tpan-=1;
        bpan-=1;
        glutPostRedisplay();
        break;

    case ':':
        tpan+=1;
        bpan +=1;
        glutPostRedisplay();
        break;

    // zoom 2;
    case '|':
        eyez++;

        glutPostRedisplay();
        break;
    case '\\':
        eyez--;
        glutPostRedisplay();
        break;

    case 'w': // move eye point upwards along Y axis
        eyey+=1.0;
        break;
    case 's': // move eye point downwards along Y axis
        eyey-=1.0;
        break;
    case 'a': // move eye point left along X axis
        eyex-=1.0;
        break;
    case 'd': // move eye point right along X axis
        eyex+=1.0;
        break;



    case 'j': // move ref point upwards along Y axis
        centery+=1.0;
        break;
    case 'n': // move ref point downwards along Y axis
        centery-=1.0;
        break;
    case 'b': // move ref point left along X axis
        centerx-=1.0;
        break;
    case 'm': // move eye point right along X axis
        centerx+=1.0;
        break;
    case 'k':  //move ref point away from screen/ along z axis
        centerz+=1;
        break;



    case 27:	// Escape key
        exit(1);
    }
}

void animate()
{
    if (bRotate == true)
    {
        theta += 0.1;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.2;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }

    if (fan4 == true )
    {
        gamma += 1;
        if(gamma > 360.0)
            gamma -= 360.0 * floor(gamma/360.0);
    }

    if (shoulderLeft == true )
    {
        kappa += 1;
        if(kappa > 360.0)
            kappa -= 360.0 * floor(kappa/360.0);

        cout<<" squeeze " <<squeeze<<endl;
        cout<<"release "<<release<<endl;

        if(squeeze==true)
        {
            cout<<"squeeze ++"<<endl;
            cout<<"squeeze ++"<<endl;
            kappa += .2;
        }
        else
        {
            cout<<"sadnAngle --"<<endl;
            kappa -= .2;

        }

        if(kappa > 95.0)
        {
            release=false;
            cout<<"ANgle Behind FALSE"<<endl;
        }

        if(kappa <85.0)
        {
            cout<<"Angle gehind True"<<endl;
            release=true;
        }

    }

    if ( sandbagMov == true)
    {

        //cout<<" Swing Behind " <<swingBehind<<endl;
        //cout<<"SandlAngle "<<sandAngle<<endl;

        if(swingBehind==true)
        {
            //cout<<"sandAngle ++"<<endl;
            //cout<<"sandAngle ++"<<endl;
            sandAngle += .2;
        }
        else
        {
            //cout<<"sadnAngle --"<<endl;
            sandAngle -= .2;

        }

        if(sandAngle > 95.0)
        {
            swingBehind=false;
            //cout<<"ANgle Behind FALSE"<<endl;
        }

        if(sandAngle <85.0)
        {
            //cout<<"Angle gehind True"<<endl;
            swingBehind=true;
        }

    }

    glutPostRedisplay();

}


int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("GYM FOR GRAPHICS PROJECT");
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_NORMALIZE );
    glEnable( GL_LIGHTING );



    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//4.bmp"); //1
    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//5.bmp"); //2
    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//6.bmp"); //3
    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//7.bmp"); //4
    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//8.bmp"); //5
    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//grass1.bmp"); //6
    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//9.bmp");
    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//10.bmp");
    LoadTexture1("E://4-2 SESHH//GRAPHICS//PROJECT//door.bmp"); //7 //eigula te just aponar jekono pic dia boshaia location ta thik kore dilei kaaj korbe.


    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutMainLoop();

    return 0;
}
