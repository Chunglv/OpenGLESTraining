#include "Application.h"
#include "FileSystem.h"
#include "Shaders.h"
#include "Vertex.h"
#include "Input.h"
#include "Color.h"
#include "TGA.h"
#include "TexCoor.h"

//#define INTERPOLATION
#define TEXTURE
Shaders myShaders;

float vertexData[] = { 0.5f, 0.5f, 0.0f , 1.0f, 0.0f,
0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
-0.5f, 0.5f, 0.0f, 0.0f, 0.0f };
Color  colorData[3];
TexCoor texCoor[4];
unsigned int indices[] = {0, 1, 3, 1, 2 , 3};

GLuint vbo, ebo, tbo;
unsigned int texture1;
// create the application here.
Application::Application()
{	
}

// Initialize current game.
int Application::Init(const char *resPath)
{		
	FileSystem::GetInstance()->Init(resPath);			
	//creation of shaders and program 

	myShaders.Init("Shaders/texture.vs", "Shaders/texture.fs");


	InitVBO();
	InitTexture();

	return true;
}

// Update current state
void Application::Update(float deltatime)
{
	Input * input = Input::GetInstance();	
	
	if ( input->HasPointer(POINTER_DRAGGED) )
	{
		LOGI("\nPOINTER_DRAGGED");
	}
	if ( input->HasPointer(POINTER_DOWN) )
	{
		LOGI("\nPOINTER_DOWN");
	}
	if ( input->HasPointer(POINTER_UP) )
	{
		LOGI("\nPOINTER_UP");
	}
	
	if ( input->KeyPressed() )
	{
		LOGI("\nKey Pressed");
	}
}

void Application::InitVBO()
{
	
	glGenBuffers(1, &vbo);	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}

void Application::InitTexture()
{
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, bpp;
	// load image, create texture and generate mipmaps
	char* imageData = LoadTGA("..\\Resource\\Textures\\betty_color.tga", &width, &height, &bpp);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

}
// Render application

void Application::Render()
{	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(myShaders.program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(myShaders.program, "u_Texture"), 0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if (myShaders.a_position != -1)
	{
		glEnableVertexAttribArray(myShaders.a_position);
		glVertexAttribPointer(myShaders.a_position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	}
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindBuffer(GL_ARRAY_BUFFER, tbo);
	int iTextLoc = glGetAttribLocation(myShaders.program, "a_TexCoord");
	if (iTextLoc != -1)
	{
		//glBufferData(GL_ARRAY_BUFFER, sizeof(texCoor), texCoor, GL_STATIC_DRAW);
		glEnableVertexAttribArray(iTextLoc);
		glVertexAttribPointer(iTextLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// destroy the application here.
void Application::Destroy()
{	
	Application::FreeInstance();
};

Application::~Application()
{	
	FileSystem::FreeInstance();
	Input::FreeInstance();		
}

