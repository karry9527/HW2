#include "../SharedCode/ShaderLoader.h"
#include "../SharedCode/mesh.h"

#include "FreeImage.h"
#include "view.h"
#include "scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "light.h"
#include <GL/glut.h>
#include <iostream>

using namespace std;


#define		TEX_NUM 4	  //the number of textures you use.
GLuint		texObject[TEX_NUM];	//texture object
GLhandleARB	MyShader;
mesh			*object1, *object2;
light*		light1;
view*		view1;
scene		*scene1, *scene2;
int			WinW,WinH;
int			object_num = 1;

unsigned int LoadTexture(const char* filename, int iTextureId);
//unsigned int SetTexture(int type, int iTextureId);
void LoadShaders();
void Display();
void keyboard(unsigned char key, int x, int y);
void Reshape(GLsizei , GLsizei );

int main(int argc, char** argv)
{
	object1 = new mesh("HighResolutionSphere.obj");
	object2 = new mesh("LowResolutionSphere.obj");

	view1 = new view("HW2.view");
	light1 = new light("HW2.light");
	scene1 = new scene("HW2DisplacementMap.scene");
	scene2 = new scene("HW2BumpMap.scene");
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Shader Use Texture");

	GLenum glew_error;
	if((glew_error = glewInit()) != GLEW_OK)return -1;

	FreeImage_Initialise();
	texObject[0] = LoadTexture("worldColorMap.bmp", 0);
	texObject[1] = LoadTexture("worldHeightMap.bmp", 1);
	//texObject[2] = SetTexture(1, 2);
	//texObject[3] = SetTexture(1, 3);
	FreeImage_DeInitialise();

	LoadShaders();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
    glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}

//unsigned int SetTexture(int type, int iTextureId)
//{
//	/*int iWidth = 38;
//	int iHeight = 40;*/
//	int iWidth = 10;
//	int iHeight = 10;
//	float *t = new float[object2->bitangentList.size()];
//	
//
//	glBindTexture(GL_TEXTURE_2D, iTextureId);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	if(type == 0)
//	{
//		//for(int i = 0; i < object2->tangentList.size(), i++)
//			//t[i] = object2->tangentList[i];
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight,
//			0, GL_BGRA, GL_FLOAT, object2->tangentList[0].ptr[0]);
//	}
//	else
//	{
//		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight,
//			//0, GL_BGRA, GL_FLOAT, object2->bitangentList[0]);
//	}
//	
//
//	glGenerateMipmap(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	return iTextureId;
//}

unsigned int LoadTexture(const char* pszFile, int iTextureId)
{
	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(pszFile, 0), pszFile);
	FIBITMAP *p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);

	glBindTexture(GL_TEXTURE_2D, iTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));

	FreeImage_Unload(p32BitsImage);
	FreeImage_Unload(pImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return iTextureId;
}

void LoadShaders()
{
	MyShader = glCreateProgram();
	if(MyShader != 0)
	{
		if(object_num == 1)
		{
			ShaderLoad(MyShader, "../Examples/UseTexture/displayment.vs", GL_VERTEX_SHADER);
			ShaderLoad(MyShader, "../Examples/UseTexture/displayment.fs", GL_FRAGMENT_SHADER);
		}
		else if(object_num == 2)
		{
			ShaderLoad(MyShader, "../Examples/UseTexture/UseTexture.vs", GL_VERTEX_SHADER);
			ShaderLoad(MyShader, "../Examples/UseTexture/UseTexture.fs", GL_FRAGMENT_SHADER);
		}
	}
}

void Display()
{
	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(view1->fovy, (GLfloat)WinW/(GLfloat)WinW, view1->dnear, view1->dfar);//fovy aspect near far

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(	view1->eye[0], view1->eye[1], view1->eye[2],// eye
				view1->center[0], view1->center[1], view1->center[2],  // center
				view1->up[0], view1->up[1], view1->up[2]);    // up

	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	glEnable(GL_LIGHTING);
	for(int i = 0; i < light1->now; i++)
	{
		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_POSITION, light1->lightList[i].position);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, light1->lightList[i].ambient);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, light1->lightList[i].diffuse);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, light1->lightList[i].specular);
		//cout << "i:" << i << "  " << light1->position[i] << light1->ambient[i] << light1->diffuse[i] << light1->specular[i] << endl;
	}

	glUseProgram(MyShader);

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, texObject[0]);
	

	GLint location = glGetUniformLocation(MyShader, "colorTexture");
	//if(location == -1)
	//	printf("Cant find texture name: colorTexture\n");
	//else
		glUniform1i(location, 0);

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_2D, texObject[1]);

	// bump map test

	GLint location1 = glGetUniformLocation(MyShader, "heightTexture");
	//if(location1 == -1)
	//	printf("Cant find texture name: heightTexture\n");
	///*GLint location1 = glGetUniformLocation(MyShader, "normal_texture");
	//if(location1 == -1)
	//	printf("Cant find texture name: normal_texture\n");*/
	//else
		glUniform1i(location1, 1);

	GLint location2 = glGetUniformLocation(MyShader, "control");
	//if(location2 == -1)
	//	printf("Cant find texture name : control");
	//else
		glUniform1i(location2, object_num);


	/*GLint Attriblocation = glGetAttribLocation(MyShader, "tangent");
	//if(Attriblocation == -1)
	//	printf("Cant find texture name : tangent");
	//else
		glVertexAttrib3fv(Attriblocation, object2->tangentList[i].ptr);
		//printf("tangentList ptr: %X \n", object2->tangentList[0].ptr);
		//cout << "tangentList ptr: " << object2->tangentList[0].ptr << endl;

	GLint Attriblocation1 = glGetAttribLocation(MyShader, "bitangent");
	//if(Attriblocation1 == -1)
	//	printf("Cant find texture name : bitangent");
	//else
		glVertexAttrib3fv(Attriblocation, object2->bitangentList[i].ptr);*/
	
	if(object_num == 1)
	{
		int lastMaterial = -1;
		for (size_t i=0;i < object1->fTotal;i++)
		{
			
			if(lastMaterial != object1->faceList[i].m)
			{
				lastMaterial = (int)object1->faceList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT  , object1->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE  , object1->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR , object1->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &object1->mList[lastMaterial].Ns);

				//you can obtain the texture name by object->mList[lastMaterial].map_Kd
				//load them once in the main function before mainloop
				//bind them in display function here
			}
			glBegin(GL_POLYGON);
			for (size_t j=0;j<3;j++)
			{
				glMultiTexCoord2fv(GL_TEXTURE0,	object1->tList[object1->faceList[i][j].t].ptr);
				glNormal3fv(object1->nList[object1->faceList[i][j].n].ptr);
				glVertex3fv(object1->vList[object1->faceList[i][j].v].ptr);	
			}
			glEnd();
		}
	}
	else if(object_num == 2)
	{
		int lastMaterial = -1;
		for (size_t i=0;i < object2->fTotal;i++)
		{
			//cout << "tan :" << object2->tangentList[i].ptr[0] << "  " << object2->tangentList[i].ptr[1] << "  " << object2->tangentList[i].ptr[2] << endl; 
			/*vector<float> vec(3);
			for(int i = 0; i < 3; i++)
				vec[i] = object2->tangentList[i].ptr[i];*/
			if(lastMaterial != object2->faceList[i].m)
			{
				lastMaterial = (int)object2->faceList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT  , object2->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE  , object2->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR , object2->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &object2->mList[lastMaterial].Ns);

				//you can obtain the texture name by object->mList[lastMaterial].map_Kd
				//load them once in the main function before mainloop
				//bind them in display function here
			}

			GLint Attriblocation = glGetAttribLocation(MyShader, "tangent");
				glVertexAttrib3fv(Attriblocation, object2->tangentList[i].ptr);

			GLint Attriblocation1 = glGetAttribLocation(MyShader, "bitangent");
				glVertexAttrib3fv(Attriblocation1, object2->bitangentList[i].ptr);

			glBegin(GL_POLYGON);
			for (size_t j=0;j<3;j++)
			{
				//GLint Attriblocation = glGetAttribLocation(MyShader, "tangent");
				//	glVertexAttrib3fv(Attriblocation, object2->tangentList[i*3 + j].ptr);
				//
				//GLint Attriblocation1 = glGetAttribLocation(MyShader, "bitangent");
				//	glVertexAttrib3fv(Attriblocation, object2->bitangentList[i*3 + j].ptr);

				glMultiTexCoord2fv(GL_TEXTURE0,	object2->tList[object2->faceList[i][j].t].ptr);
				glNormal3fv(object2->nList[object2->faceList[i][j].n].ptr);
				glVertex3fv(object2->vList[object2->faceList[i][j].v].ptr);	
			}
			glEnd();
		}
	}
	glutSwapBuffers();
	glutPostRedisplay();
}



void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
		case 'w':	view1->eye[2] -= 10;break;//eye往前
        case 'a':	view1->eye[0] -= 10;break;//eye往左
        case 's':	view1->eye[2] += 10;break;//eye往後
        case 'd':	view1->eye[0] += 10;break;//eye往右
		case '1':	object_num = 1;break;
		case '2':	object_num = 2;break;
    }
	glutPostRedisplay();
}

void Reshape(GLsizei w, GLsizei h)
{
	WinW = w;
	WinH = h;
	// viewport transformation
	glViewport(view1->x, view1->y, view1->w, view1->h);

}

