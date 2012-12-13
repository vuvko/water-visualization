/////////////////////////////////////////////////////////////////
// main.cpp Author: Vladimir Frolov, 2011, Graphics & Media Lab.
/////////////////////////////////////////////////////////////////

#include <GL/glew.h>

#include "GL/glus.h"
#include "../vsgl3/glHelper.h"

#include "Water.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <windows.h>

using namespace std;

static const double eps = 0.00001;

ShaderProgram g_renderRoomProg;

struct MyInput
{
  MyInput() 
  {
    cam_rot[0] = 110;
    cam_rot[1] = 330;
    scene_pos[0] = scene_pos[1] = scene_pos[2] = 0;
    mx = my = 0;
    rdown = ldown = false;
    cam_dist = 20.0f;
  }

  int mx;
  int my;
  bool rdown;
  bool ldown;
  float cam_rot[2];
  float scene_pos[3];

  float cam_dist;

}input;

int g_width  = 0;
int g_height = 0;

float4x4 g_projectionMatrix;

float3 g_camPos(0,0,0); // z will be assigned from input.cam_dist
float3 g_lightPos(1, 2, 15);

MyInput g_fixedCam[3];

bool g_simulate = true;


SimpleMesh* g_pRoomMesh  = NULL;
Water* g_pWater = NULL;

static GLuint g_tileTexture, g_tileTextureNorm, g_wallTexture, g_wallTextureNorm;

static GLuint g_verticesVBO;

static GLuint g_normalsVBO;

static GLuint g_vao;

static GLuint g_numberVertices;


void SetVSync(bool sync)
{
  typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

  PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

  if( wglSwapIntervalEXT )
    wglSwapIntervalEXT(sync);
}


void PrintGLInfo()
{
  CHECK_GL_ERRORS;
  std::cout << "GPU Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "GPU Name  : " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "GL_VER    : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL_VER  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
GLUSboolean init(GLUSvoid)
{
  try 
  {
    // First camera position
    g_fixedCam[0].cam_dist = 25;
    g_fixedCam[0].cam_rot[0] = 100;
    g_fixedCam[0].cam_rot[1] = 0;
    g_fixedCam[0].scene_pos[0] = g_fixedCam[0].scene_pos[1] = 0;
    g_fixedCam[0].scene_pos[2] = 1;

    // Second camera position
    g_fixedCam[1].cam_dist = 20;
    g_fixedCam[1].cam_rot[0] = 115;
    g_fixedCam[1].cam_rot[1] = 30;
    g_fixedCam[1].scene_pos[0] = g_fixedCam[1].scene_pos[1] = g_fixedCam[1].scene_pos[2] = 1;

    // Third camera position
    g_fixedCam[2].cam_dist = 2.5;
    g_fixedCam[2].cam_rot[0] = 85;
    g_fixedCam[2].cam_rot[1] = 330;
    g_fixedCam[2].scene_pos[0] = -3;
    g_fixedCam[2].scene_pos[1] = 2.7;
    g_fixedCam[2].scene_pos[2] = -4;

    PrintGLInfo();
    SetVSync(1);

    g_renderRoomProg   = ShaderProgram("../main/Room.vert", "../main/Room.frag");
    
    g_pRoomMesh        = new SimpleMesh(g_renderRoomProg.program, 2,   SimpleMesh::PLANE,  1.0f);
    g_pWater           = new Water();

    // Texture set up.

    GLUStgaimage image;
    glusLoadTgaImage("Res/stone.tga", &image);

    glGenTextures(1, &g_tileTexture); CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_2D, g_tileTexture); CHECK_GL_ERRORS;

    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data); CHECK_GL_ERRORS;

    glGenerateMipmap(GL_TEXTURE_2D);

    // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS;

    glusLoadTgaImage("Res/stone_n.tga", &image);

    glGenTextures(1, &g_tileTextureNorm); CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_2D, g_tileTextureNorm); CHECK_GL_ERRORS;

    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data); CHECK_GL_ERRORS;

    glGenerateMipmap(GL_TEXTURE_2D);

    // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS;

    glusLoadTgaImage("Res/wall.tga", &image);

    glGenTextures(1, &g_wallTexture); CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_2D, g_wallTexture); CHECK_GL_ERRORS;

    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data); CHECK_GL_ERRORS;

    glGenerateMipmap(GL_TEXTURE_2D);

    // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS;

    glusLoadTgaImage("Res/wall_n.tga", &image);

    glGenTextures(1, &g_wallTextureNorm); CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_2D, g_wallTextureNorm); CHECK_GL_ERRORS;

    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data); CHECK_GL_ERRORS;

    glGenerateMipmap(GL_TEXTURE_2D);

    // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS;

    glBindTexture(GL_TEXTURE_2D, 0); CHECK_GL_ERRORS;

    // loading model
    GLUSshape wavefrontObj;
    // Use a helper function to load an wavefront object file.
    cout << int(glusLoadObjFile("monkey.obj", &wavefrontObj)) << endl;

    g_numberVertices = wavefrontObj.numberVertices;

    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, wavefrontObj.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) wavefrontObj.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &g_normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, wavefrontObj.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) wavefrontObj.normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glusDestroyShapef(&wavefrontObj);

    return GLUS_TRUE;
  }
  catch(std::runtime_error e)
  {
    std::cerr << e.what() << std::endl;
    exit(-1);
  }
  catch(...)
  {
    std::cerr << "Unexpected Exception (init)!" << std::endl;
    exit(-1);
  }
}

GLUSvoid reshape(GLUSuint width, GLUSuint height)
{
	glViewport(0, 0, width, height);
  g_width  = width;
  g_height = height;

  glusPerspectivef(g_projectionMatrix.L(), 45.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);  // Calculate the projection matrix
}

GLUSvoid mouse(GLUSboolean pressed, GLUSuint button, GLUSuint x, GLUSuint y)
{
  if(!pressed)
    return;

  if (button & 1)// left button
  {
    //input.ldown=true;		
    input.mx=x;			
    input.my=y;
  }

  if (button & 4)	// right button
  {
    //input.rdown=true;
    input.mx=x;
    input.my=y;

    g_pWater->AddWave((GLUSfloat)x / g_width, (GLUSfloat)y / g_height);
  }
}

GLUSvoid mouseMove(GLUSuint button, GLUSint x, GLUSint y)
{
  if(button & 1)		// left button
  {
    input.cam_rot[0] += (input.my - y) / 5.0;	// change rotation
    input.cam_rot[1] += (input.mx - x) / 5.0;
    
    if (input.cam_rot[0] > 360) {
      input.cam_rot[0] = 0;
    } else if (input.cam_rot[0] < 0) {
      input.cam_rot[0] = 360;
    }

    if (input.cam_rot[1] > 360) {
      input.cam_rot[1] = 0;
    } else if (input.cam_rot[1] < 0) {
      input.cam_rot[1] = 360;
    }
    
    input.mx=x;
    input.my=y;
  }

  if(button & 4)
  {
    //g_pWater->AddWave(0.5, 0.5); // 
    g_pWater->AddWave((GLUSfloat)x / g_width, (GLUSfloat)y / g_height);
  }

}

GLUSvoid keyboard(GLUSboolean pressed, GLUSuint key)
{
  double dx = 0, dy = 0, dz = 0, shift = 0.25;
  double r_psi = input.cam_rot[0] * PIf / 180;
  double r_phi = input.cam_rot[1] * PIf / 180;

  switch(key)
  {
  case 'W':
    input.cam_dist -= shift;
    if (input.cam_dist < eps) { // fixes the problem of inverting camera and controls
      input.cam_dist = 5 * shift; // in order to minimize the number of operations
      dx = input.cam_dist * sin(r_psi) * cos(r_phi);
      dy = input.cam_dist * sin(r_psi) * sin(r_phi);
      dz = input.cam_dist * cos(r_psi);
    }
    break;

  case 'S':
    input.cam_dist += shift;
    break;

  case 'A':
    dx = -shift * cos(r_phi - PIf / 2);
    dy = -shift * sin(r_phi - PIf / 2);
    break;

  case 'D':
    dx = shift * cos(r_phi - PIf / 2);
    dy = shift * sin(r_phi - PIf / 2);
    break;

  case VK_PRIOR: // PAGE UP key
    dz += shift;
    break;

  case VK_NEXT: // PAGE DOWN key
    dz -= shift;
    break;

  case VK_F1: // F1 - first camera
    input.cam_dist = g_fixedCam[0].cam_dist;
    input.cam_rot[0] = g_fixedCam[0].cam_rot[0];
    input.cam_rot[1] = g_fixedCam[0].cam_rot[1];
    input.scene_pos[0] = g_fixedCam[0].scene_pos[0];
    input.scene_pos[1] = g_fixedCam[0].scene_pos[1];
    input.scene_pos[2] = g_fixedCam[0].scene_pos[2];
    break;

  case VK_F2: // F2 - second camera
    input.cam_dist = g_fixedCam[1].cam_dist;
    input.cam_rot[0] = g_fixedCam[1].cam_rot[0];
    input.cam_rot[1] = g_fixedCam[1].cam_rot[1];
    input.scene_pos[0] = g_fixedCam[1].scene_pos[0];
    input.scene_pos[1] = g_fixedCam[1].scene_pos[1];
    input.scene_pos[2] = g_fixedCam[1].scene_pos[2];
    break;

  case VK_F3: // F3 - third camera
    input.cam_dist = g_fixedCam[2].cam_dist;
    input.cam_rot[0] = g_fixedCam[2].cam_rot[0];
    input.cam_rot[1] = g_fixedCam[2].cam_rot[1];
    input.scene_pos[0] = g_fixedCam[2].scene_pos[0];
    input.scene_pos[1] = g_fixedCam[2].scene_pos[1];
    input.scene_pos[2] = g_fixedCam[2].scene_pos[2];
    break;


  case VK_F5: // F5 - Pause
    g_simulate = false;
    break;

  case VK_F6: // F6 - Resume
    g_simulate = true;
    break;
  }

  input.scene_pos[0] += dx;
  input.scene_pos[1] += dy;
  input.scene_pos[2] += dz;

}

GLUSboolean update(GLUSfloat time)
{
  try 
  {
    static float elaspedTimeFromStart = 0;
    static int frames = 0;
    elaspedTimeFromStart += 10 * time;

    ++frames;
    if (elaspedTimeFromStart > 10 - eps)
    {
      cerr << frames << endl;
      elaspedTimeFromStart = 0;
      frames = 0;
      g_pWater->AddWave(0.5, 0.5);
    }

    float3 camPos;
    double r_psi = input.cam_rot[0] * PIf / 180;
    double r_phi = input.cam_rot[1] * PIf / 180;
    camPos.x = input.scene_pos[0] - input.cam_dist * sin(r_psi) * cos(r_phi);
    camPos.y = input.scene_pos[1] - input.cam_dist * sin(r_psi) * sin(r_phi);
    camPos.z = input.scene_pos[2] - input.cam_dist * cos(r_psi);
    g_camPos.x = camPos.x;
    g_camPos.y = camPos.y;
    g_camPos.z = camPos.z;

    /*
    cout << "-----" << endl;
    cout << input.scene_pos[0] << ' ' << input.scene_pos[1] << ' ' << input.scene_pos[2] << endl;
    cout << input.cam_rot[0] << ' ' << input.cam_rot[1] << ' ' << input.cam_dist << endl;
    cout << camPos.x << ' ' << camPos.y << ' ' << camPos.z << endl;
    cout << sin(r_psi - PIf / 2) * cos(r_phi) << ' ' <<
                               cos(r_psi - PIf / 2) << ' ' <<
                               sin(r_psi - PIf / 2) * sin(r_phi) << endl;
    */
    //cout << g_camPos.x << ' ' << g_camPos.y << ' ' << g_camPos.z << endl;
    

    float4x4 modelView;
    glusLookAtf(modelView.L(), camPos.x, camPos.y, camPos.z, 
                               input.scene_pos[0], input.scene_pos[1], input.scene_pos[2],
                               sin(r_psi - PIf / 2) * cos(r_phi),
                               sin(r_psi - PIf / 2) * sin(r_phi),
                               cos(r_psi - PIf / 2));                           // ... and the view matrix ...
  
    glViewport(0, 0, g_width, g_height);
    glClearColor(0.65f, 0.6f, 0.75f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // draw room
    //
    glUseProgram(g_renderRoomProg.program);

    setUniform(g_renderRoomProg.program, "g_camPos", g_camPos);
    setUniform(g_renderRoomProg.program, "g_lightPos", g_lightPos);
    setUniform(g_renderRoomProg.program, "modelViewMatrix", modelView);
    setUniform(g_renderRoomProg.program, "projectionMatrix", g_projectionMatrix);  // set matrix we have calculated in "reshape" funtion

    // draw floor
    //
    {
      float4x4 rotationMatrix, scaleMatrix, translateMatrix;
      float4x4 transformMatrix1, transformMatrix2;

      // calc matrices
      //
      glusRotateRzRyRxf(rotationMatrix.L(), 0, 0, 0);
      glusScalef(scaleMatrix.L(), 10, 10, 10);
      glusTranslatef(translateMatrix.L(), 0,0,-8);
      glusMultMatrixf(transformMatrix1.L(), rotationMatrix.L(), scaleMatrix.L());
      glusMultMatrixf(transformMatrix2.L(), translateMatrix.L(), transformMatrix1.L());

      // pass matrices to the shader
      //
      setUniform(g_renderRoomProg.program, "objectMatrix", transformMatrix2);
      setUniform(g_renderRoomProg.program, "scale", 0.1f);
      setUniform(g_renderRoomProg.program, "type", 0.1f);
      setUniform(g_renderRoomProg.program, "scale_val", float2(2.0f, 2.0f));
      bindTexture(g_renderRoomProg.program, 1, "u_texture", g_tileTexture);
      bindTexture(g_renderRoomProg.program, 2, "u_texture_n", g_tileTextureNorm);
    }

    g_pRoomMesh->Draw();

    // draw borders
    //
    
    {
      float4x4 rotationMatrix, scaleMatrix, translateMatrix;
      float4x4 transformMatrix1, transformMatrix2;

      // calc matrices
      //
      glusRotateRzRyRxf(rotationMatrix.L(), 90, 0, 0);
      glusScalef(scaleMatrix.L(), 10, 5, 1);
      glusTranslatef(translateMatrix.L(), 0,-10,-3);
      glusMultMatrixf(transformMatrix1.L(), rotationMatrix.L(), scaleMatrix.L());
      glusMultMatrixf(transformMatrix2.L(), translateMatrix.L(), transformMatrix1.L());

      // pass matrices to the shader
      //
      setUniform(g_renderRoomProg.program, "objectMatrix", transformMatrix2);
      setUniform(g_renderRoomProg.program, "scale", 0.0f);
      setUniform(g_renderRoomProg.program, "type", 2.0f);
      setUniform(g_renderRoomProg.program, "scale_val", float2(1.0f, 0.5f));
      bindTexture(g_renderRoomProg.program, 1, "u_texture", g_wallTexture);
      bindTexture(g_renderRoomProg.program, 2, "u_texture_n", g_wallTextureNorm);
    }

    g_pRoomMesh->Draw();

    {
      float4x4 rotationMatrix, scaleMatrix, translateMatrix;
      float4x4 transformMatrix1, transformMatrix2;

      // calc matrices
      //
      glusRotateRzRyRxf(rotationMatrix.L(), 90, 0, 90);
      glusScalef(scaleMatrix.L(), 10, 5, 1);
      glusTranslatef(translateMatrix.L(), 10,0,-3);
      glusMultMatrixf(transformMatrix1.L(), rotationMatrix.L(), scaleMatrix.L());
      glusMultMatrixf(transformMatrix2.L(), translateMatrix.L(), transformMatrix1.L());

      // pass matrices to the shader
      //
      setUniform(g_renderRoomProg.program, "objectMatrix", transformMatrix2);
      setUniform(g_renderRoomProg.program, "scale", 0.0f);
      setUniform(g_renderRoomProg.program, "type", 1.0f);
      setUniform(g_renderRoomProg.program, "scale_val", float2(1.0f, 0.5f));
      bindTexture(g_renderRoomProg.program, 1, "u_texture", g_wallTexture);
      bindTexture(g_renderRoomProg.program, 2, "u_texture_n", g_wallTextureNorm);
    }

    g_pRoomMesh->Draw();
    
    if (g_simulate) { // everything that moves
      g_pWater->SimStep(); // water simulation step
    }


    // draw water
    //
    glUseProgram(g_pWater->GetRenderProgram());

    setUniform(g_pWater->GetRenderProgram(), "g_camPos", g_camPos);
    setUniform(g_pWater->GetRenderProgram(), "g_lightPos", g_lightPos);
    setUniform(g_pWater->GetRenderProgram(), "modelViewMatrix", modelView);
    setUniform(g_pWater->GetRenderProgram(), "projectionMatrix", g_projectionMatrix);  

    // set water plane transforms
    //
    {
      float4x4 rotationMatrix, scaleMatrix, translateMatrix;
      float4x4 transformMatrix1, transformMatrix2;

      glusScalef(scaleMatrix.L(), 10, 10, 10);
      glusTranslatef(translateMatrix.L(), 0,0,0);
      glusMultMatrixf(transformMatrix1.L(), rotationMatrix.L(), scaleMatrix.L());
      glusMultMatrixf(transformMatrix2.L(), translateMatrix.L(), transformMatrix1.L());

      setUniform(g_pWater->GetRenderProgram(), "objectMatrix", transformMatrix2);
    }
    
    g_pWater->Draw();

    return GLUS_TRUE;
  }
  catch(std::runtime_error e)
  {
    std::cerr << e.what() << std::endl;
    exit(-1);
  }
  catch(...)
  {
    std::cerr << "Unexpected Exception(render)!" << std::endl;
    exit(-1);
  }
}

/**
 * Function to clean up things.
 */
GLUSvoid shutdown(GLUSvoid)
{
  delete g_pWater; g_pWater = NULL;
  delete g_pRoomMesh; g_pRoomMesh = NULL;
}

/**
 * Main entry point.
 */
int main(int argc, char* argv[])
{
	glusInitFunc(init);
	glusReshapeFunc(reshape);
	glusUpdateFunc(update);
	glusTerminateFunc(shutdown);
  glusMouseFunc(mouse);
  glusMouseMoveFunc(mouseMove);
  glusKeyFunc(keyboard);

	glusPrepareContext(3, 0, GLUS_FORWARD_COMPATIBLE_BIT);

	if (!glusCreateWindow("Shallow Water Template Sample", 640, 480, GLUS_FALSE))
	{
		printf("Could not create window!");
		return -1;
	}

	// Init GLEW
	glewExperimental = GL_TRUE;
  GLenum err=glewInit();
  if(err!=GLEW_OK)
  {
    sprintf("glewInitError", "Error: %s\n", glewGetErrorString(err));
    return -1;
  }
  glGetError(); // flush error state variable, caused by glew errors
  
	if (!glewIsSupported("GL_VERSION_3_0"))
	{
		printf("OpenGL 3.0 not supported.");

		glusDestroyWindow();
		return -1;
	}

	glusRun();

	return 0;
}
