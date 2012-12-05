#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 objectMatrix;

uniform int gridSizeX;
uniform int gridSizeY;

uniform sampler2D inPositions;

in vec4 vertex;
in vec2 texCoord;

out vec3 fragmentWorldPos;
out vec3 fragmentNormal;
out vec2 fragmentTexCoord;

out vec4 worldPos;
out vec4 viewPos;

uniform vec3 g_lightPos;
uniform vec3 g_camPos;

void main(void)
{
  float x = texCoord.x*float(gridSizeX);
  float y = texCoord.y*float(gridSizeY);
  
  vec2 invSize = vec2(1.0/float(gridSizeX), 1.0/float(gridSizeY));

  float c = textureLod(inPositions, vec2(x,y)*invSize, 0).x;

  vec4 vpos = vertex + vec4(0,c,0,0);  // displace vertex position 

  float l = textureLod(inPositions, vec2(x-1, y)*invSize, 0).x;
  float r = textureLod(inPositions, vec2(x+1, y)*invSize, 0).x;
  float u = textureLod(inPositions, vec2(x, y-1)*invSize, 0).x;
  float d = textureLod(inPositions, vec2(x, y+1)*invSize, 0).x;

  float nx = l-r;
  float ny = u-d;
  vec3 normal = normalize(vec3(nx,ny,2.0/float(gridSizeX)));
  
  worldPos = objectMatrix*vpos;
  viewPos  = modelViewMatrix*worldPos;
  
  gl_Position    = projectionMatrix*viewPos;
  fragmentNormal = vec3(objectMatrix*normalize(vec4(normal, 1.0)));
}
