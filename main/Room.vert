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

void main(void)
{
  vec4 worldPos  = objectMatrix*vertex;
  vec4 viewPos   = modelViewMatrix*worldPos;

  fragmentTexCoord = texCoord;

  fragmentNormal = vec3(0, 1, 0);

  vec3 light = normalize(g_lightPos - worldPos);
  vec3 cam = normalize(g_camPos - worldPos);

  camTan = vec3(dot(cam, fragmentTexCoord), dot(cam, fragmentTexCoord), dot(cam, fragmentNormal));
  lightTan = vec3(dot(light, fragmentTexCoord), dot(light, fragmentTexCoord), dot(light, fragmentNormal));

  gl_Position    = projectionMatrix*viewPos;
}
