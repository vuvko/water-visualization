#version 330

in vec3 fragmentWorldPos;
in vec3 fragmentNormal;
in vec2 fragmentTexCoord;

in vec4 viewPos;
in vec4 worldPos;

out vec4 fragColor;

uniform vec3 g_lightPos;
uniform vec3 g_camPos;

uniform sampler2D inPosTexture;

void main(void)
{	  
  const vec3  diffColor = vec3 ( 0.5, 0.5, 1.0);
  const vec3  specColor = vec3 ( 0.5, 0.5, 0.5);
  const float specPower = 30.0;

  vec3 light = normalize(g_lightPos);
  vec3 eye = normalize(g_camPos - worldPos);
  vec3 norm = normalize(fragmentNormal);

  vec3 h = normalize(light + eye);

  vec3 diff = diffColor * max(dot(norm, light), 0.0);
  vec3 spec = specColor * pow(max(dot(norm, h), 0.0), specPower);

  fragColor = vec4(diff + spec, 0.5);
}
