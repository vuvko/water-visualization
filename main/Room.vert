#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 objectMatrix;

uniform vec3 g_lightPos; 
uniform vec3 g_camPos;

uniform vec2 scale_val = vec2(1, 1);

uniform float type;

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

  fragmentTexCoord = vec2(texCoord.x * scale_val.x, texCoord.y * scale_val.y);

  fragmentNormal = normalize(objectMatrix*normal);
  
  vec3 light = normalize(g_lightPos - vec3(worldPos));
  vec3 cam = normalize(g_camPos - vec3(worldPos));

  //camTan = cross(fragmentNormal, (cross(cam, fragmentNormal)));

  if (type < 0.5) {
    camTan = vec3(cam.x, cam.y, cam.z);
    lightTan = vec3(light.x, light.y, light.z);
  } else if (type < 1.5) {
    camTan = vec3(cam.y, -cam.z, -cam.x);
    lightTan = vec3(light.y, -light.z, -light.x);
  } else {
    camTan = vec3(-cam.x, -cam.z, cam.y);
    lightTan = vec3(-light.x, -light.z, light.y);
  }
  
  camPos = g_camPos;
  lightPos = g_lightPos;

  gl_Position    = projectionMatrix*viewPos;
}
