#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 objectMatrix;

uniform vec3 g_lightPos; 
uniform vec3 g_camPos;

in vec4 vertex;
in vec4 normal;
in vec2 texCoord;

out vec2 fragmentTexCoord;
out vec4 worldPos;
out vec4 viewPos;
out vec3 fragmentNormal;
out vec3 lightTan;
out vec3 camTan;
out vec3 lightPos;
out vec3 camPos;

void main(void)
{
  worldPos  = objectMatrix*vertex;
  viewPos   = modelViewMatrix*worldPos;

  fragmentTexCoord = texCoord;

  fragmentNormal = normalize(objectMatrix*normal);
  //fragmentNormal = vec3(0, 0, 1);
  
  vec3 light = normalize(vec4(g_lightPos, 1) - worldPos);
  vec3 cam = normalize(vec4(g_camPos, 1) - worldPos);

  //camTan = cross(fragmentNormal, (cross(cam, fragmentNormal)));

  camTan = vec3(cam.x, cam.y, cam.z);
  lightTan = vec3(light.x, light.y, light.z);
  
  camPos = g_camPos;
  lightPos = g_lightPos;

  gl_Position    = projectionMatrix*viewPos;
}
