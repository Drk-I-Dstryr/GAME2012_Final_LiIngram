﻿//***************************************************************************
// GAME2012_Final_LiIngram.cpp by Russell Li - 101309707 and Zackary Ingram - Student ID
//
// 01_Lights.
//
// Description:
//	Click run to see the results.
//*****************************************************************************

////http://glew.sourceforge.net/
//The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source C/C++ extension loading library. 
//GLEW provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target
//platform. OpenGL core and extension functionality is exposed in a single header file. GLEW has been tested on a 
//variety of operating systems, including Windows, Linux, Mac OS X, FreeBSD, Irix, and Solaris.
//
//http://freeglut.sourceforge.net/
//The OpenGL Utility Toolkit(GLUT) is a library of utilities for OpenGL programs, which primarily perform system - level I / O with the host operating system.
//Functions performed include window definition, window control, and monitoring of keyboardand mouse input.
//Routines for drawing a number of geometric primitives(both in solid and wireframe mode) are also provided, including cubes, spheresand the Utah teapot.
//GLUT also has some limited support for creating pop - up menus..

//OpenGL functions are in a single library named GL (or OpenGL in Windows). Function names begin with the letters glSomeFunction*();
//Shaders are written in the OpenGL Shading Language(GLSL)
//To interface with the window system and to get input from external devices into our programs, we need another library. For the XWindow System, this library is called GLX, for Windows, it is wgl,
//and for the Macintosh, it is agl. Rather than using a different library for each system,
//we use two readily available libraries, the OpenGL Extension Wrangler(GLEW) and the OpenGLUtilityToolkit(GLUT).
//GLEW removes operating system dependencies. GLUT provides the minimum functionality that should be expected in any modern window system.
//OpenGL makes heavy use of defined constants to increase code readability and avoid the use of magic numbers.Thus, strings such as GL_FILL and GL_POINTS are defined in header(#include <GL/glut.h>)

//https://glm.g-truc.net/0.9.9/index.html
////OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
///////////////////////////////////////////////////////////////////////

using namespace std;

#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include "Shape.h"
#include "Light.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))
#define FPS 60
#define MOVESPEED 0.5f
#define TURNSPEED 0.05f
#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,0.9,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)
#define SPEED 0.25f

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum keyMasks {
	KEY_FORWARD = 0b00000001,		// 0x01 or   1	or   01
	KEY_BACKWARD = 0b00000010,		// 0x02 or   2	or   02
	KEY_LEFT = 0b00000100,
	KEY_RIGHT = 0b00001000,
	KEY_UP = 0b00010000,
	KEY_DOWN = 0b00100000,
	KEY_MOUSECLICKED = 0b01000000

	// Any other keys you want to add.
};

static unsigned int
program,
vertexShaderId,
fragmentShaderId;

GLuint modelID, viewID, projID;
glm::mat4 View, Projection;

// Our bitflag variable. 1 byte for up to 8 key states.
unsigned char keys = 0; // Initialized to 0 or 0b00000000.

// Texture variables.
GLuint groundID, wallID, towerwallID, roof1ID, merlonID, towerdoorID, stairID, gateID, roof2ID, emblemID,
ohnoID, woodID, marbleID, hedgeID, brickID;
GLint width, height, bitDepth;

// Light objects. Now OOP.
AmbientLight aLight(
	glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
	0.5f);							// Diffuse strength.

DirectionalLight dLight(
	glm::vec3(1.0f, 0.0f, 0.0f),	// Origin.
	glm::vec3(1.0f, 0.0f, 0.0f),	// Diffuse colour.
	0.0f);							// Diffuse strength.

PointLight pLights[7] = { 
	{ glm::vec3(6.0f, 5.0f, -41.0f),	// Position.
	50.0f,							// Range.
	1.0f, 4.5f, 75.0f,				// Constant, Linear, Quadratic.   
	glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
	2.0f },							// Diffuse strength.

	{ glm::vec3(40.0f, 5.0f, -40.0f),	// Position.
	50.0f,							// Range.
	1.0f, 4.5f, 75.0f,				// Constant, Linear, Quadratic.   
	glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
	2.0f },							// Diffuse strength.

	{ glm::vec3(6.0f, 5.0f, -20.0f),	// Position.
	50.0f,							// Range.
	1.0f, 4.5f, 75.0f,				// Constant, Linear, Quadratic.   
	glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
	2.0f },							// Diffuse strength.

	{ glm::vec3(40.0f, 5.0f, -20.0f),	// Position.
	50.0f,							// Range.
	1.0f, 4.5f, 75.0f,				// Constant, Linear, Quadratic.   
	glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
	2.0f },							// Diffuse strength.

	{ glm::vec3(6.0f, 5.0f, -6.0f),	// Position.
	50.0f,							// Range.
	1.0f, 4.5f, 75.0f,				// Constant, Linear, Quadratic.   
	glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
	2.0f },							// Diffuse strength.

	{ glm::vec3(40.0f, 5.0f, -6.0f),	// Position.
	50.0f,							// Range.
	1.0f, 4.5f, 75.0f,				// Constant, Linear, Quadratic.   
	glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
	2.0f },							// Diffuse strength.

	{ glm::vec3(25.0f, 10.0f, -25.0f),	// Position.
	70.0f,							// Range.
	1.0f, 4.5f, 75.0f,				// Constant, Linear, Quadratic.   
	glm::vec3(1.0f, 1.0f, 1.0f),	// Diffuse colour.
	2.0f }							// Diffuse strength.
};						

SpotLight sLight(
	glm::vec3(5.0f, 3.0f, -5.0f),	// Position.
	glm::vec3(0.1f, 1.0f, 0.1f),	// Diffuse colour.
	0.0f,							// Diffuse strength.
	glm::vec3(0.0f, -1.0f, 0.0f),   // Direction. Normally opposite because it's used in dot product. See constructor.
	30.0f);							// Edge.

Material mat = { 0.5f, 8 }; // Alternate way to construct an object.

// Camera and transform variables.
float scale = 1.0f, angle = 0.0f;
glm::vec3 position, frontVec, worldUp, upVec, rightVec; // Set by function
GLfloat pitch, yaw;
int lastX, lastY;

// Geometry data.
Grid g_grid(50); // ground
Cube g_cube;
Prism g_prism(20); // Towers
Prism g_prism2(10); // Guardhouse towers
Cone g_cone2(10); // Guardhouse tower roof
Cone g_cone(20); // Tower roof

void timer(int); // Prototype.

void resetView()
{
	position = glm::vec3(10.0f, 5.0f, 25.0f); // Super pulled back because of grid size.
	frontVec = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	pitch = 0.0f;
	yaw = -90.0f;
	// View will now get set only in transformObject
}

void init(void)
{
	srand((unsigned)time(NULL));
	// Create shader program executable.
	vertexShaderId = setShader((char*)"vertex", (char*)"cube.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	modelID = glGetUniformLocation(program, "model");
	viewID = glGetUniformLocation(program, "view");
	projID = glGetUniformLocation(program, "projection");
	
	// Projection matrix : 45∞ Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 1.0f / 1.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	// Projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

	// Camera matrix
	resetView();

	// Image loading.
	stbi_set_flip_vertically_on_load(true);

	// Load first image.
	unsigned char* image = stbi_load("maze.png", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &groundID);
	glBindTexture(GL_TEXTURE_2D, groundID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End first image.

	// Load second image.
	image = stbi_load("Castlewall.png", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &wallID);
	glBindTexture(GL_TEXTURE_2D, wallID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End second image.

	// Load second image.
	image = stbi_load("Towerwall.png", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &towerwallID);
	glBindTexture(GL_TEXTURE_2D, towerwallID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End second image.

	// Load third image.
	image = stbi_load("roof.jpg", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &roof1ID);
	glBindTexture(GL_TEXTURE_2D, roof1ID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End third image.

	// Load fifth image.
	image = stbi_load("merlon.jpg", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &merlonID);
	glBindTexture(GL_TEXTURE_2D, merlonID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End fifth image.

	// Load sixth image.
	image = stbi_load("TowerDoor3.png", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &towerdoorID);
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End sixth image.

	// Load seventh image.
	image = stbi_load("Smoothstone.jpg", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &stairID);
	glBindTexture(GL_TEXTURE_2D, stairID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End seventh image.

	// Load eighth image.
	image = stbi_load("Gate3.png", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &gateID);
	glBindTexture(GL_TEXTURE_2D, gateID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End eighth image.

	// Load ninth image.
	image = stbi_load("roof.jpg", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &roof2ID);
	glBindTexture(GL_TEXTURE_2D, roof2ID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End ninth image.

	// Load ninth image.
	image = stbi_load("Kakapo2.png", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &emblemID);
	glBindTexture(GL_TEXTURE_2D, emblemID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End ninth image.

	// Load tenth image.
	image = stbi_load("Wood.jpg", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &woodID);
	glBindTexture(GL_TEXTURE_2D, woodID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End tenth image.

	// Load eleventh image.
	image = stbi_load("Marble.jpg", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &marbleID);
	glBindTexture(GL_TEXTURE_2D, marbleID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End eleventh image.

	//load twelveth image
	image = stbi_load("hedge.jpg", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &hedgeID);
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	//End twelveth image

	//load thirteenth image
	image = stbi_load("brick3.jpg", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &brickID);
	glBindTexture(GL_TEXTURE_2D, brickID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	//End thirteenth image

	//load fourteenth image
	image = stbi_load("ohno.png", &width, &height, &bitDepth, 0);
	if (!image) { cout << "Unable to load file!" << endl; }
	glGenTextures(1, &ohnoID);
	glBindTexture(GL_TEXTURE_2D, ohnoID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	//End fourteenth image

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

	// Setting material values.
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), mat.specularStrength);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), mat.shininess);

	// Setting ambient light.
	glUniform3f(glGetUniformLocation(program, "aLight.base.diffuseColour"), aLight.diffuseColour.x, aLight.diffuseColour.y, aLight.diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "aLight.base.diffuseStrength"), aLight.diffuseStrength);

	// Setting directional light.
	glUniform3f(glGetUniformLocation(program, "dLight.base.diffuseColour"), dLight.diffuseColour.x, dLight.diffuseColour.y, dLight.diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "dLight.base.diffuseStrength"), dLight.diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "dLight.origin"), dLight.origin.x, dLight.origin.y, dLight.origin.z);

	// Setting point lights.
	glUniform3f(glGetUniformLocation(program, "pLights[0].base.diffuseColour"), pLights[0].diffuseColour.x, pLights[0].diffuseColour.y, pLights[0].diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLights[0].base.diffuseStrength"), pLights[0].diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLights[0].position"), pLights[0].position.x, pLights[0].position.y, pLights[0].position.z);
	glUniform1f(glGetUniformLocation(program, "pLights[0].constant"), pLights[0].constant);
	glUniform1f(glGetUniformLocation(program, "pLights[0].linear"), pLights[0].linear);
	glUniform1f(glGetUniformLocation(program, "pLights[0].quadratic"), pLights[0].quadratic);

	glUniform3f(glGetUniformLocation(program, "pLights[1].base.diffuseColour"), pLights[1].diffuseColour.x, pLights[1].diffuseColour.y, pLights[1].diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLights[1].base.diffuseStrength"), pLights[1].diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLights[1].position"), pLights[1].position.x, pLights[1].position.y, pLights[1].position.z);
	glUniform1f(glGetUniformLocation(program, "pLights[1].constant"), pLights[1].constant);
	glUniform1f(glGetUniformLocation(program, "pLights[1].linear"), pLights[1].linear);
	glUniform1f(glGetUniformLocation(program, "pLights[1].quadratic"), pLights[1].quadratic);

	glUniform3f(glGetUniformLocation(program, "pLights[2].base.diffuseColour"), pLights[2].diffuseColour.x, pLights[2].diffuseColour.y, pLights[2].diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLights[2].base.diffuseStrength"), pLights[2].diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLights[2].position"), pLights[2].position.x, pLights[2].position.y, pLights[2].position.z);
	glUniform1f(glGetUniformLocation(program, "pLights[2].constant"), pLights[2].constant);
	glUniform1f(glGetUniformLocation(program, "pLights[2].linear"), pLights[2].linear);
	glUniform1f(glGetUniformLocation(program, "pLights[2].quadratic"), pLights[2].quadratic);

	glUniform3f(glGetUniformLocation(program, "pLights[3].base.diffuseColour"), pLights[3].diffuseColour.x, pLights[3].diffuseColour.y, pLights[3].diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLights[3].base.diffuseStrength"), pLights[3].diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLights[3].position"), pLights[3].position.x, pLights[3].position.y, pLights[3].position.z);
	glUniform1f(glGetUniformLocation(program, "pLights[3].constant"), pLights[3].constant);
	glUniform1f(glGetUniformLocation(program, "pLights[3].linear"), pLights[3].linear);
	glUniform1f(glGetUniformLocation(program, "pLights[3].quadratic"), pLights[3].quadratic);

	glUniform3f(glGetUniformLocation(program, "pLights[4].base.diffuseColour"), pLights[4].diffuseColour.x, pLights[4].diffuseColour.y, pLights[4].diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLights[4].base.diffuseStrength"), pLights[4].diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLights[4].position"), pLights[4].position.x, pLights[4].position.y, pLights[4].position.z);
	glUniform1f(glGetUniformLocation(program, "pLights[4].constant"), pLights[4].constant);
	glUniform1f(glGetUniformLocation(program, "pLights[4].linear"), pLights[4].linear);
	glUniform1f(glGetUniformLocation(program, "pLights[4].quadratic"), pLights[4].quadratic);

	glUniform3f(glGetUniformLocation(program, "pLights[5].base.diffuseColour"), pLights[5].diffuseColour.x, pLights[5].diffuseColour.y, pLights[5].diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLights[5].base.diffuseStrength"), pLights[5].diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLights[5].position"), pLights[5].position.x, pLights[5].position.y, pLights[5].position.z);
	glUniform1f(glGetUniformLocation(program, "pLights[5].constant"), pLights[5].constant);
	glUniform1f(glGetUniformLocation(program, "pLights[5].linear"), pLights[5].linear);
	glUniform1f(glGetUniformLocation(program, "pLights[5].quadratic"), pLights[5].quadratic);

	glUniform3f(glGetUniformLocation(program, "pLights[6].base.diffuseColour"), pLights[6].diffuseColour.x, pLights[6].diffuseColour.y, pLights[6].diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLights[6].base.diffuseStrength"), pLights[6].diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLights[6].position"), pLights[6].position.x, pLights[6].position.y, pLights[6].position.z);
	glUniform1f(glGetUniformLocation(program, "pLights[6].constant"), pLights[6].constant);
	glUniform1f(glGetUniformLocation(program, "pLights[6].linear"), pLights[6].linear);
	glUniform1f(glGetUniformLocation(program, "pLights[6].quadratic"), pLights[6].quadratic);

	// Setting spot light.
	glUniform3f(glGetUniformLocation(program, "sLight.base.diffuseColour"), sLight.diffuseColour.x, sLight.diffuseColour.y, sLight.diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "sLight.base.diffuseStrength"), sLight.diffuseStrength);

	glUniform3f(glGetUniformLocation(program, "sLight.position"), sLight.position.x, sLight.position.y, sLight.position.z);
	glUniform3f(glGetUniformLocation(program, "sLight.direction"), sLight.direction.x, sLight.direction.y, sLight.direction.z);
	glUniform1f(glGetUniformLocation(program, "sLight.edge"), sLight.edgeRad);

	// All VAO/VBO data now in Shape.h! But we still need to do this AFTER OpenGL is initialized.
	g_grid.BufferShape();
	g_cube.BufferShape();
	g_prism.BufferShape();
	g_prism2.BufferShape();
	g_cone.BufferShape();
	g_cone2.BufferShape();

	// Enable depth testing and face culling. 
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBlendEquation(GL_FUNC_ADD);

	// Enable smoothing.
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	timer(0); // Setup my recursive 'fixed' timestep/framerate.
}

//---------------------------------------------------------------------
//
// calculateView
//
void calculateView()
{
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec = glm::normalize(frontVec);
	rightVec = glm::normalize(glm::cross(frontVec, worldUp));
	upVec = glm::normalize(glm::cross(rightVec, frontVec));

	View = glm::lookAt(
		position, // Camera position
		position + frontVec, // Look target
		upVec); // Up vector
}

//---------------------------------------------------------------------
//
// transformModel
//
void transformObject(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, scale);
	
	// We must now update the View.
	calculateView();

	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projID, 1, GL_FALSE, &Projection[0][0]);
}

//---------------------------------------------------------------------
//
// display
//
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, groundID); // Use this texture for all shapes.

	// Grid. Note: I rendered it solid!
	transformObject(glm::vec3(1.0f, 1.0f, 1.0f), X_AXIS, -90.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	g_grid.DrawShape(GL_TRIANGLES);

	// Back wall.
	glBindTexture(GL_TEXTURE_2D, wallID);
	transformObject(glm::vec3(45.0f, 15.0f, 3.0f), X_AXIS, 0.0f, glm::vec3(2.5f, 0.0f, -3.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	// Main wall1.
	glBindTexture(GL_TEXTURE_2D, wallID);
	transformObject(glm::vec3(20.0f, 15.0f, 3.0f), X_AXIS, 0.0f, glm::vec3(2.5f, 0.0f, -48.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	// Main wall2.
	glBindTexture(GL_TEXTURE_2D, wallID);
	transformObject(glm::vec3(20.0f, 15.0f, 3.0f), X_AXIS, 0.0f, glm::vec3(27.5f, 0.0f, -48.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	// Main wall upper.
	glBindTexture(GL_TEXTURE_2D, wallID);
	transformObject(glm::vec3(5.0f, 8.0f, 3.0f), X_AXIS, 0.0f, glm::vec3(22.5f, 7.0f, -48.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	// Stairs.
	glBindTexture(GL_TEXTURE_2D, stairID);
	transformObject(glm::vec3(5.0f, 0.5f, 5.0f), X_AXIS, 0.0f, glm::vec3(22.5f, 0.0f, -48.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	transformObject(glm::vec3(5.0f, 0.5f, 4.5f), X_AXIS, 0.0f, glm::vec3(22.5f, 0.5f, -48.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	transformObject(glm::vec3(5.0f, 0.5f, 4.0f), X_AXIS, 0.0f, glm::vec3(22.5f, 1.0f, -48.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);
	
	// Railings.
	glBindTexture(GL_TEXTURE_2D, woodID);
	transformObject(glm::vec3(0.1f, 0.1f, 5.0f), X_AXIS, 45.0f, glm::vec3(22.39f, 3.0f, -45.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	transformObject(glm::vec3(0.1f, 0.1f, 5.0f), X_AXIS, 45.0f, glm::vec3(27.51f, 3.0f, -45.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	// Castle gate.
	glBindTexture(GL_TEXTURE_2D, gateID);
	transformObject(glm::vec3(5.0f, 8.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(22.5f, 1.5f, -47.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	// Emblem.
	glBindTexture(GL_TEXTURE_2D, emblemID);
	transformObject(glm::vec3(2.0f, 2.0f, 0.0001f), X_AXIS, 0.0f, glm::vec3(24.0f, 10.0f, -44.99f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left wall
	glBindTexture(GL_TEXTURE_2D, wallID);
	transformObject(glm::vec3(43.0f, 15.0f, 3.0f), Y_AXIS, 90.0f, glm::vec3(1.0f, 0.0f, -3.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right wall
	transformObject(glm::vec3(43.0f, 15.0f, 3.0f), Y_AXIS, 90.0f, glm::vec3(45.5f, 0.0f, -3.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Middle room wall1
	glBindTexture(GL_TEXTURE_2D, brickID);
	transformObject(glm::vec3(3.0f, 7.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(21.0f, 0.0f, -25.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Middle room wall2
	glBindTexture(GL_TEXTURE_2D, brickID);
	transformObject(glm::vec3(3.0f, 7.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(21.0f, 0.0f, -19.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Middle room wall3
	glBindTexture(GL_TEXTURE_2D, brickID);
	transformObject(glm::vec3(0.5f, 7.0f, 6.0f), Y_AXIS, 0.0f, glm::vec3(21.0f, 0.0f, -25.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Middle room wall4
	glBindTexture(GL_TEXTURE_2D, brickID);
	transformObject(glm::vec3(0.5f, 7.0f, 5.5f), Y_AXIS, 0.0f, glm::vec3(27.5f, 0.0f, -24.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Middle room wall5
	glBindTexture(GL_TEXTURE_2D, brickID);
	transformObject(glm::vec3(2.0f, 7.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(26.0f, 0.0f, -25.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Middle room wall6
	glBindTexture(GL_TEXTURE_2D, brickID);
	transformObject(glm::vec3(2.5f, 7.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(25.5f, 0.0f, -19.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Middle room wall7
	glBindTexture(GL_TEXTURE_2D, brickID);
	transformObject(glm::vec3(7.0f, 0.5f, 6.5f), Y_AXIS, 0.0f, glm::vec3(21.0f, 7.0f, -25.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	// Pedestal.
	glBindTexture(GL_TEXTURE_2D, marbleID);
	transformObject(glm::vec3(1.0f, 1.0f, 1.0f), X_AXIS, 0.0f, glm::vec3(24.5f, 0.0f, -23.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_prism.DrawShape(GL_TRIANGLES);

	///////////////////////////////////////////////////////////////////////////////////////////////
	
	//Back Merlon and Crenels
	//Back Merlon1
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(7.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon2
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(9.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon3
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(11.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon4
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(13.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon5
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(15.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon6
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(17.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon7
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(19.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon8
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(21.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon9
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(23.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon10
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(25.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon11
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(27.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon12
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(29.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon13
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(31.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon14
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(33.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon15
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(35.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon16
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(37.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon17
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(39.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon18
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(41.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon19
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(8.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon20
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(10.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon21
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(12.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon22
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(14.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon23
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(16.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon24
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(18.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon25
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(20.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon26
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(22.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon27
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(24.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon28
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(26.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon29
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(28.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon30
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(30.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon31
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(32.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon32
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(34.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon33
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(36.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon34
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(38.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon35
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(40.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Merlon36
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(42.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel1
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(8.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel2
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(10.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel3
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(12.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel4
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(14.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel5
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(16.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel6
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(18.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel7
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(20.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel8
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(22.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel9
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(24.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel10
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(26.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel11
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(28.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel12
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(30.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel13
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(32.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel14
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(34.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel15
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(36.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel16
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(38.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel17
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(40.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel18
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(42.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel19
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(6.0f, 15.0f, -0.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel20
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(6.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel21
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(7.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel22
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(9.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel23
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(11.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel24
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(13.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel25
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(15.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel26
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(17.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel27
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(19.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel28
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(21.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel29
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(23.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel30
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(25.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel31
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(27.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel32
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(29.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel33
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(31.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel34
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(33.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel35
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(35.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel36
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(37.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel37
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(39.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel38
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(41.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Back Crenel39
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(43.0f, 15.0f, -3.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//Front Merlon and Crenels
	//Front Merlon1
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(7.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon2
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(9.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon3
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(11.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon4
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(13.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon5
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(15.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon6
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(17.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon7
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(19.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon8
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(21.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon9
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(23.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon10
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(25.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon11
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(27.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon12
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(29.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon13
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(31.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon14
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(33.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon15
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(35.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);
	
	//Front Merlon16
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(37.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon17
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(39.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon18
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(41.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon19
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(8.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon20
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(10.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon21
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(12.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon22
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(14.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon23
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(16.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon24
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(18.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon25
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(20.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon26
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(22.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon27
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(24.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon28
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(26.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon29
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(28.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon30
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(30.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon31
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(32.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon32
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(34.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon33
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(36.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon34
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(38.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon35
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(40.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Merlon36
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(42.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel1
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(8.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel2
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(10.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel3
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(12.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel4
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(14.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel5
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(16.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel6
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(18.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel7
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(20.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel8
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(22.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel9
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(24.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel10
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(26.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel11
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(28.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel12
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(30.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel13
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(32.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel14
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(34.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel15
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(36.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel16
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(38.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel17
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(40.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel18
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(42.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel19
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(6.0f, 15.0f, -45.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel20
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(6.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel21
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(7.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel22
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(9.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel23
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(11.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel24
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(13.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel25
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(15.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel26
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(17.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel27
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(19.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel28
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(21.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel29
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(23.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel30
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(25.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel31
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(27.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel32
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(29.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel33
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(31.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel34
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(33.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel35
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(35.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel36
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(37.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel37
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(39.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel38
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(41.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Front Crenel39
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(43.0f, 15.0f, -48.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//Right Merlon and Crenels

	//Right Merlon1
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -4.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon2
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -6.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon3
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -8.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon4
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -10.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon5
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -12.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon6
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -14.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon7
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -16.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon8
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -18.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon9
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -20.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon10
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -22.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon11
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -24.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon12
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -26.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon13
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -28.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon14
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -30.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon15
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon16
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -34.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon17
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -36.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon18
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -38.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon19
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -40.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon20
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -42.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon21
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -5.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon22
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -7.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon23
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -9.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon24
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -11.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon25
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -13.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon26
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -15.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon27
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -17.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon28
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -19.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon29
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -21.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon30
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -23.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon31
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -25.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon32
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -27.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon33
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -29.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon34
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -31.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon35
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -33.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon36
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -35.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon37
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -37.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon38
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -39.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Merlon39
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -41.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel1
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -3.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel2
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -5.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel3
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -7.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel4
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -9.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel5
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -11.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel6
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -13.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel7
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -15.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel8
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -17.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel9
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -19.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel10
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -21.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel11
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -23.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel12
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -25.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel13
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -27.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel14
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -29.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel15
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -31.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel16
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -33.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel17
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -35.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel18
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -37.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel19
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -39.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel20
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -41.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel21
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(0.8f, 15.0f, -43.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel22
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -4.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel23
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -6.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel24
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -8.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel25
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -10.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel26
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -12.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel27
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -14.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel28
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -16.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel29
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -18.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel30
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -20.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel31
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -22.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel32
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -24.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel33
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -26.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel34
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -28.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel35
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -30.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel36
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel37
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -34.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel38
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -36.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel39
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -38.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel40
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -40.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Right Crenel41
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(3.8f, 15.0f, -42.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Left Merlons and Crenels

	//Left Merlon1
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -5.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon2
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -7.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon3
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -9.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon4
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -11.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon5
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -13.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon6
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -15.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon7
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -17.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon8
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -19.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon9
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -21.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon10
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -23.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon11
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -25.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon12
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -27.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon13
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -29.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon14
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -31.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon15
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -33.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon16
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -35.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon17
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -37.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon18
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -39.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon19
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -41.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon20
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -6.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon21
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -8.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon22
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -10.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon23
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -12.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon24
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -14.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon25
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -16.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon26
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -18.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon27
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -20.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon28
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -22.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon29
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -24.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon30
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -26.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon31
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -28.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon32
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -30.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon33
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon34
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -34.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon35
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -36.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon36
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -38.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon37
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -40.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon38
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -42.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Merlon39
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 2.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -4.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel1
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -4.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel2
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -6.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel3
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -8.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel4
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -10.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel5
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -12.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel6
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -14.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel7
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -16.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel8
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -18.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel9
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -20.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel10
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -22.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel11
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -24.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel12
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -26.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel13
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -28.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel14
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -30.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel15
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel16
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -34.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel17
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -36.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel18
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -38.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel19
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -40.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel20
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(45.3f, 15.0f, -42.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel21
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -5.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel22
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -7.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel23
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -9.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel24
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -11.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel25
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -13.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel26
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -15.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel27
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -17.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel28
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -19.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel29
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -21.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel30
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -23.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel31
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -25.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel32
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -27.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel33
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -29.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel34
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -31.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel35
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -33.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel36
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -35.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel37
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -37.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel38
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -39.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel39
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -41.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel40
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -43.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Left Crenel41
	glBindTexture(GL_TEXTURE_2D, merlonID);
	transformObject(glm::vec3(1.0f, 1.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(48.2f, 15.0f, -3.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	////////////////////////////////////////////////////////////////////////////////////////////////

	//Doors
	//Tower Door1
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(6.5f, 15.0f, -0.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door2
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(42.5f, 15.0f, -0.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door3
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(1.5f, 15.0f, -4.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door4
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(1.5f, 15.0f, -43.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door5
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(46.0f, 15.0f, -4.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door6
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 0.0f, glm::vec3(46.0f, 15.0f, -43.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door7
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(6.5f, 15.0f, -45.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door8
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(42.5f, 15.0f, -45.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door9
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(15.0f, 15.0f, -45.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door10
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(34.5f, 15.0f, -45.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door11
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(21.5f, 15.0f, -45.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Tower Door12
	glBindTexture(GL_TEXTURE_2D, towerdoorID);
	transformObject(glm::vec3(2.0f, 3.0f, 0.5f), Y_AXIS, 90.0f, glm::vec3(28.0f, 15.0f, -45.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Back Towers
	// Back right tower.
	glBindTexture(GL_TEXTURE_2D, towerwallID);
	transformObject(glm::vec3(7.0f, 18.0f, 7.0f), X_AXIS, 0.0f, glm::vec3(-0.1f, 0.0f, -4.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_prism.DrawShape(GL_TRIANGLES);
	
	// Back left tower.
	transformObject(glm::vec3(7.0f, 18.0f, 7.0f), X_AXIS, 0.0f, glm::vec3(42.5f, 0.0f, -4.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), mat.specularStrength);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), mat.shininess);
	g_prism.DrawShape(GL_TRIANGLES);
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Front Towers
	// Front right tower.
	transformObject(glm::vec3(7.0f, 18.0f, 7.0f), X_AXIS, 0.0f, glm::vec3(0.0f, 0.0f, -50.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), mat.specularStrength);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), mat.shininess);
	g_prism.DrawShape(GL_TRIANGLES);

	// Front middle right tower
	transformObject(glm::vec3(7.0f, 20.0f, 7.0f), X_AXIS, 0.0f, glm::vec3(15.0f, 0.0f, -50.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), mat.specularStrength);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), mat.shininess);
	g_prism2.DrawShape(GL_TRIANGLES);

	// Front middle left tower
	transformObject(glm::vec3(7.0f, 20.0f, 7.0f), X_AXIS, 0.0f, glm::vec3(28.0f, 0.0f, -50.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), mat.specularStrength);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), mat.shininess);
	g_prism2.DrawShape(GL_TRIANGLES);

	// Front left tower.
	transformObject(glm::vec3(7.0f, 18.0f, 7.0f), X_AXIS, 0.0f, glm::vec3(42.5f, 0.0f, -50.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), mat.specularStrength);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), mat.shininess);
	g_prism.DrawShape(GL_TRIANGLES);
	///////////////////////////////////////////////////////////////////////////////////////////////////

	// Back roofs
	// Back right tower roof.
	glBindTexture(GL_TEXTURE_2D, roof1ID);
	transformObject(glm::vec3(9.0f, 6.0f, 9.0f), X_AXIS, 0.0f, glm::vec3(-1.0f, 18.0f, -5.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cone.DrawShape(GL_TRIANGLES);

	// Back left tower roof.
	glBindTexture(GL_TEXTURE_2D, roof1ID);
	transformObject(glm::vec3(9.0f, 6.0f, 9.0f), X_AXIS, 0.0f, glm::vec3(41.5f, 18.0f, -5.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cone.DrawShape(GL_TRIANGLES);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Front roofs
	// Front right tower roof.
	glBindTexture(GL_TEXTURE_2D, roof1ID);
	transformObject(glm::vec3(9.0f, 6.0f, 9.0f), X_AXIS, 0.0f, glm::vec3(-1.0f, 18.0f, -51.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cone.DrawShape(GL_TRIANGLES);

	// First left tower roof.
	glBindTexture(GL_TEXTURE_2D, roof1ID);
	transformObject(glm::vec3(9.0f, 6.0f, 9.0f), X_AXIS, 0.0f, glm::vec3(41.5f, 18.0f, -51.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cone.DrawShape(GL_TRIANGLES);

	// Front middle right tower roof.
	glBindTexture(GL_TEXTURE_2D, roof2ID);
	transformObject(glm::vec3(9.0f, 6.0f, 9.0f), X_AXIS, 0.0f, glm::vec3(14.0f, 20.0f, -51.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cone2.DrawShape(GL_TRIANGLES);

	// First middle left tower roof.
	transformObject(glm::vec3(9.0f, 6.0f, 9.0f), X_AXIS, 0.0f, glm::vec3(27.0f, 20.0f, -51.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cone2.DrawShape(GL_TRIANGLES);
	////////////////////////////////////////////////////////////////////////////////////////////////

	//Hedge
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(12.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(6.0f, 0.0f, -24.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 2 (Back hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(37.5f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(5.5f, 0.0f, -8.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 3 (Left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, -30.5f), X_AXIS, 0.0f, glm::vec3(5.5f, 0.0f, -8.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 4 (Right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, -30.5f), X_AXIS, 0.0f, glm::vec3(41.5f, 0.0f, -8.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 5 (Front left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(16.5f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(6.0f, 0.0f, -39.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 6 (front right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(16.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(26.0f, 0.0f, -38.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 7 (Gate left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 4.5f), X_AXIS, 0.0f, glm::vec3(22.0f, 0.0f, -41.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 8 (Gate right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 4.5f), X_AXIS, 0.0f, glm::vec3(26.0f, 0.0f, -41.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 9 (Front left inner hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(7.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(6.0f, 0.0f, -35.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 10 (Front right inner hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(28.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(14.5f, 0.0f, -35.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 11 (Front left inner hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 4.5f), X_AXIS, 0.0f, glm::vec3(14.3f, 0.0f, -35.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 12 (Middle long inner hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(31.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(8.7f, 0.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 13 (Middle short inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 6.5f), X_AXIS, 0.0f, glm::vec3(8.8f, 0.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 14 (Middle short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 6.7f), X_AXIS, 0.0f, glm::vec3(26.0f, 0.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 15 (Middle short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 6.7f), X_AXIS, 0.0f, glm::vec3(31.5f, 0.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 16 (Middle short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 7.5f), X_AXIS, 0.0f, glm::vec3(38.5f, 0.0f, -32.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 17 (Middle short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(5.2f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(31.5f, 0.0f, -25.7f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 18 (Middle medium inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(12.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(12.0f, 0.0f, -28.8f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 19 (Middle medium inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 10.5f), X_AXIS, 0.0f, glm::vec3(12.0f, 0.0f, -28.8f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 20 (Middle medium inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 9.0f), X_AXIS, 0.0f, glm::vec3(20.5f, 0.0f, -26.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 21 (Middle medium inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 9.1f), X_AXIS, 0.0f, glm::vec3(28.2f, 0.0f, -26.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 22 (Middle medium inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(4.1f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(20.5f, 0.0f, -26.25f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedger 23 (Middle medium inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(2.5f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(26.0f, 0.0f, -26.25f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 24 (middle small inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(5.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(16.0f, 0.0f, -21.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 25 (middle small inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(2.5f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(20.5f, 0.0f, -18.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 26 (middle small inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(2.5f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(26.0f, 0.0f, -18.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 27 (middle square right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(5.2f, 4.0f, 5.5f), X_AXIS, 0.0f, glm::vec3(31.5f, 0.0f, -22.7f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 28 (Middle short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 7.5f), X_AXIS, 0.0f, glm::vec3(38.5f, 0.0f, -19.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 29 (bottom short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(5.5f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(36.0f, 0.0f, -13.3f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 30 (bottom short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(6.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(26.8f, 0.0f, -14.8f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 31 (bottom short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(6.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(31.5f, 0.0f, -10.2f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 32 (Bottom inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 6.5f), X_AXIS, 0.0f, glm::vec3(31.3f, 0.0f, -14.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 33 (bottom short inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(7.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(8.8f, 0.0f, -16.3f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 34 (bottom short inner right hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 5.6f), X_AXIS, 0.0f, glm::vec3(12.4f, 0.0f, -15.8f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 35 (Bottom right long hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 10.8f), X_AXIS, 0.0f, glm::vec3(8.8f, 0.0f, -21.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 36 (Bottom middle long hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 10.8f), X_AXIS, 0.0f, glm::vec3(17.5f, 0.0f, -21.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 37 (Top left tiny hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 2.8f), X_AXIS, 0.0f, glm::vec3(17.5f, 0.0f, -28.8f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 38 (Bottom right long hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(0.5f, 4.0f, 8.3f), X_AXIS, 0.0f, glm::vec3(22.8f, 0.0f, -18.5f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 39 (bottom short inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(4.0f, 4.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(15.0f, 0.0f, -13.3f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	//Hedge 40 (bottom short inner left hedge)
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(glm::vec3(3.0f, 7.0f, 0.5f), X_AXIS, 0.0f, glm::vec3(17.8f, 0.0f, -11.7f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	// Cursed Cube.
	glBindTexture(GL_TEXTURE_2D, ohnoID);
	transformObject(glm::vec3(0.5f, 0.5f, 0.5f), X_AXIS, 0.0f, glm::vec3(10.5f, 2.0f, -13.0f));
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), 128);
	g_cube.DrawShape(GL_TRIANGLES);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	glutSwapBuffers(); // Now for a potentially smoother render.
}

void idle() // Not even called.
{
	glutPostRedisplay();
}

void parseKeys()
{
	if (keys & KEY_FORWARD)
		position += frontVec * MOVESPEED;
	if (keys & KEY_BACKWARD)
		position -= frontVec * MOVESPEED;
	if (keys & KEY_LEFT)
		position -= rightVec * MOVESPEED;
	if (keys & KEY_RIGHT)
		position += rightVec * MOVESPEED;
	if (keys & KEY_UP)
		position += upVec * MOVESPEED;
	if (keys & KEY_DOWN)
		position -= upVec * MOVESPEED;
}

void timer(int) { // Tick of the frame.
	// Get first timestamp
	int start = glutGet(GLUT_ELAPSED_TIME);
	// Update call
	parseKeys();
	// Display call
	glutPostRedisplay();
	// Calling next tick
	int end = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc((1000 / FPS) - (end-start), timer, 0);
}

// Keyboard input processing routine.
// Keyboard input processing routine.
void keyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		if (!(keys & KEY_FORWARD))
			keys |= KEY_FORWARD; // keys = keys | KEY_FORWARD
		break;
	case 's':
		if (!(keys & KEY_BACKWARD))
			keys |= KEY_BACKWARD;
		break;
	case 'a':
		if (!(keys & KEY_LEFT))
			keys |= KEY_LEFT;
		break;
	case 'd':
		if (!(keys & KEY_RIGHT))
			keys |= KEY_RIGHT;
		break;
	case 'r':
		if (!(keys & KEY_UP))
			keys |= KEY_UP;
		break;
	case 'f':
		if (!(keys & KEY_DOWN))
			keys |= KEY_DOWN;
		break;
	default:
		break;
	}
}

void keyDownSpec(int key, int x, int y) // x and y is mouse location upon key press.
{
	switch (key)
	{
	case GLUT_KEY_UP: // Up arrow.
		if (!(keys & KEY_FORWARD))
			keys |= KEY_FORWARD; // keys = keys | KEY_FORWARD
		break;
	case GLUT_KEY_DOWN: // Down arrow.
		if (!(keys & KEY_BACKWARD))
			keys |= KEY_BACKWARD;
		break;
	default:
		break;
	}
}

void keyUp(unsigned char key, int x, int y) // x and y is mouse location upon key press.
{
	switch (key)
	{
	case 'w':
		keys &= ~KEY_FORWARD; // keys = keys & ~KEY_FORWARD. ~ is bitwise NOT.
		break;
	case 's':
		keys &= ~KEY_BACKWARD;
		break;
	case 'a':
		keys &= ~KEY_LEFT;
		break;
	case 'd':
		keys &= ~KEY_RIGHT;
		break;
	case 'r':
		keys &= ~KEY_UP;
		break;
	case 'f':
		keys &= ~KEY_DOWN;
		break;
	case ' ':
		resetView();
		break;
	default:
		break;
	}
}

void keyUpSpec(int key, int x, int y) // x and y is mouse location upon key press.
{
	switch (key)
	{
	case GLUT_KEY_UP:
		keys &= ~KEY_FORWARD; // keys = keys & ~KEY_FORWARD
		break;
	case GLUT_KEY_DOWN:
		keys &= ~KEY_BACKWARD;
		break;
	default:
		break;
	}
}

void mouseMove(int x, int y)
{
	if (keys & KEY_MOUSECLICKED)
	{
		pitch += (GLfloat)((y - lastY) * TURNSPEED);
		yaw -= (GLfloat)((x - lastX) * TURNSPEED);
		lastY = y;
		lastX = x;
	}
}

void mouseClick(int btn, int state, int x, int y)
{
	if (state == 0)
	{
		lastX = x;
		lastY = y;
		keys |= KEY_MOUSECLICKED; // Flip flag to true
		glutSetCursor(GLUT_CURSOR_NONE);
		//cout << "Mouse clicked." << endl;
	}
	else
	{
		keys &= ~KEY_MOUSECLICKED; // Reset flag to false
		glutSetCursor(GLUT_CURSOR_INHERIT);
		//cout << "Mouse released." << endl;
	}
}

//---------------------------------------------------------------------
//
// clean
//
void clean()
{
	cout << "Cleaning up!" << endl;
	glDeleteTextures(1, &groundID);
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	//Before we can open a window, theremust be interaction between the windowing systemand OpenGL.In GLUT, this interaction is initiated by the following function call :
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutSetOption(GLUT_MULTISAMPLE, 8);

	//if you comment out this line, a window is created with a default size
	glutInitWindowSize(1034, 1034);

	//the top-left corner of the display
	glutInitWindowPosition(0, 0);

	glutCreateWindow("GAME2012_Final_LiIngram");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init(); // Our own custom function.

	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutSpecialFunc(keyDownSpec);
	glutKeyboardUpFunc(keyUp);
	glutSpecialUpFunc(keyUpSpec);

	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove); // Requires click to register.

	atexit(clean); // This useful GLUT function calls specified function before exiting program. 
	glutMainLoop();

	return 0;
}