#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 objectMatrix;

uniform vec3 g_lightPos; 
uniform vec3 g_camPos;

uniform vec2 scale_val = vec2(1, 1);

in vec4 vertex;
in vec4 normal;
in vec2 texCoord;

out vec2 fragmentTexCoord;
out vec4 worldPos;
out vec4 viewPos;
out vec3 fragmentNormal;
out vec3 lightPos;
out vec3 camPos;

void main(void)
{
  worldPos  = objectMatrix*vertex;
  viewPos   = modelViewMatrix*worldPos;

  fragmentTexCoord = vec2(texCoord.x * scale_val.x, texCoord.y * scale_val.y);

  fragmentNormal = normalize(objectMatrix*normal);
  
  camPos = g_camPos;
  lightPos = g_lightPos;

  gl_Position    = projectionMatrix*viewPos;
}
