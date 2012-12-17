#version 330

in  vec2  fragmentTexCoord;
out float outHeight;

uniform sampler2D inPositions;
uniform sampler2D inPositionsPrev;

uniform int gridSizeX;
uniform int gridSizeY;

void main(void)
{ 
  float x = fragmentTexCoord.x*float(gridSizeX);
  float y = fragmentTexCoord.y*float(gridSizeY);
  
  vec2 invSize = vec2(1.0/float(gridSizeX), 1.0/float(gridSizeY));
 
  // this is the way you may read from prev (inPositionsPrev) and current (inPositions) textures
  //
  float Uprev = textureLod(inPositionsPrev, vec2(x,y)*invSize, 0).x; 
  float Ucurr_r = textureLod(inPositions, vec2(x, y + 1)*invSize, 0).x; 
  float Ucurr_l = textureLod(inPositions, vec2(x, y - 1)*invSize, 0).x; 
  float Ucurr_u = textureLod(inPositions, vec2(x + 1, y)*invSize, 0).x; 
  float Ucurr_d = textureLod(inPositions, vec2(x - 1, y)*invSize, 0).x; 
  
  //outHeight = Uprev; // and here you need to place formula
  float w = 1.985;
  outHeight = (1 - w) * Uprev + w * (Ucurr_r + Ucurr_l + Ucurr_u + Ucurr_d) / 4;
}

