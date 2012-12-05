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
    cam_rot[0] = cam_rot[1] = cam_rot[2] = cam_rot[3] = 0.f;
    mx = my = 0;
    rdown = ldown = false;
    cam_dist = 25.0f;
  }

  int mx;
  int my;
  bool rdown;
  bool ldown;
  float cam_rot[4];
  float cam_pos[4];

  float cam_dist;

}input;

int g_width  = 0;
int g_height = 0;

float4x4 g_projectionMatrix;

float3 g_camPos(0,0,0); // z will be assigned from input.cam_dist
float3 g_lightPos(10, 10, 10);


SimpleMesh* g_pRoomMesh  = NULL;
Water* g_pWater = NULL;

static GLuint g_tileTexture;


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
    PrintGLInfo();
    SetVSync(1);

    g_renderRoomProg   = ShaderProgram("../main/Room.vert", "../main/Room.frag");
    
    g_pRoomMesh        = new SimpleMesh(g_renderRoomProg.program, 2,   SimpleMesh::PLANE,  1.0f);
    g_pWater           = new Water();

    // Texture set up.

    GLUStgaimage image;
    glusLoadTgaImage("Res/tile1.tga", &image);

    glGenTextures(1, &g_tileTexture); CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_2D, g_tileTexture); CHECK_GL_ERRORS;

    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data); CHECK_GL_ERRORS;

    // Mipmap generation is now included in OpenGL 3 and above
    glGenerateMipmap(GL_TEXTURE_2D);

    // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS;

    glBindTexture(GL_TEXTURE_2D, 0); CHECK_GL_ERRORS;

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
    input.ldown=true;		
    input.mx=x;			
    input.my=y;
  }

  if (button & 4)	// right button
  {
    input.rdown=true;
    input.mx=x;
    input.my=y;

    g_pWater->AddWave((GLUSfloat)y / g_height, (GLUSfloat)x / g_width);
  }
}

GLUSvoid mouseMove(GLUSuint button, GLUSint x, GLUSint y)
{
  if(button & 1)		// left button
  {
    int x1 = x;
    int y1 = y;

    input.cam_rot[0] += 0.25f*(y1-input.my);	// change rotation
    input.cam_rot[1] += 0.25f*(x1-input.mx);

    input.mx=x;
    input.my=y;
  }

  if(button & 4)
  {
    //g_pWater->AddWave(0.5, 0.5); // 
    g_pWater->AddWave((GLUSfloat)y / g_height, (GLUSfloat)x / g_width);
  }

}

GLUSvoid keyboard(GLUSboolean pressed, GLUSuint key)
{
  double dx = 0, dy = 0, dz = 0, shift = 0.25;
  double r_psi = input.cam_rot[1] * PIf / 180;
  double r_phi = input.cam_rot[0] * PIf / 180;
  //cerr << input.cam_rot[0] << endl;
  switch(key)
  {
  case 'w':
  case 'W':
    input.cam_dist -= shift;
    break;

  case 's':
  case 'S':
    input.cam_dist += shift;
    break;

  case 'a':
  case 'A':
    break;

  case 'd':
  case 'D':
    break;
  }

  input.cam_pos[0] += dy;
  input.cam_pos[1] += dx;
  input.cam_pos[2] += dz;

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

    g_camPos.z = input.cam_dist;
    g_camPos.x = input.cam_pos[0];
    g_camPos.y = input.cam_pos[1];


    float4x4 model;
    float4x4 modelView;
    glusLoadIdentityf(model.L()); 
    glusRotateRzRyRxf(model.L(), input.cam_rot[0], input.cam_rot[1], 0.0f);
    glusLookAtf(modelView.L(), g_camPos.x, g_camPos.y, g_camPos.z, 
                               0.0f, 0.0f, 0.0f, 
                               0.0f, 1.0f, 0.0f);                           // ... and the view matrix ...

    glusMultMatrixf(modelView.L(), modelView.L(), model.L()); 	            // ... to get the final model view matrix

  
    glViewport(0, 0, g_width, g_height);
    glClearColor(0.8f, 0.8f, 0.85f, 1.0f);
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
      glusRotateRzRyRxf(rotationMatrix.L(), 90, 0.0f, 0.0f);
      glusScalef(scaleMatrix.L(), 10, 10, 10);
      glusTranslatef(translateMatrix.L(), 0,-4,0);
      glusMultMatrixf(transformMatrix1.L(), rotationMatrix.L(), scaleMatrix.L());
      glusMultMatrixf(transformMatrix2.L(), translateMatrix.L(), transformMatrix1.L());

      // pass matrices to the shader
      //
      setUniform(g_renderRoomProg.program, "objectMatrix", transformMatrix2);
      setUniform(g_renderRoomProg.program, "g_diffuseColor",  float3(0.5, 0.65, 0.85));
      setUniform(g_renderRoomProg.program, "g_specularColor", float3(0.55, 0.55, 0.55));
      bindTexture(g_renderRoomProg.program, 0, "u_texture", g_tileTexture);
    }
    //glBindTexture(GL_TEXTURE_2D, g_tileTexture);

    g_pRoomMesh->Draw();

    //glBindTexture(GL_TEXTURE_2D, 0);

    g_pWater->SimStep(); // water simulation step


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
