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
out vec3 fragmentNormal;
out vec3 lightTan;
out vec3 camTan;
out vec3 lightPos;
out vec3 camPos;

void main(void)
{
  vec4 worldPos  = objectMatrix*vertex;
  vec4 viewPos   = modelViewMatrix*worldPos;

  fragmentTexCoord = texCoord;

  fragmentNormal = normalize(objectMatrix*normal);
  //fragmentNormal = vec3(0, 0, 1);
  
  vec3 light = objectMatrix*vec4(g_lightPos, 1) - viewPos;
  vec3 cam = g_camPos;
  //cam = modelViewMatrix * vec4(cam, 1) - viewPos;
  cam = normalize(cam - vec3(viewPos));

  //camTan = cross(fragmentNormal, (cross(cam, fragmentNormal)));

  camTan = vec3(cam.x, cam.y, cam.z);
  lightTan = vec3(light.x, light.z, light.y);
  
  camPos = g_camPos;
  lightPos = g_lightPos;

  gl_Position    = projectionMatrix*viewPos;
}
