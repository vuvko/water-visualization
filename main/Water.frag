#version 330

in vec3 fragmentWorldPos;
in vec3 fragmentNormal;
in vec2 fragmentTexCoord;

in vec4 viewPos;
in vec4 worldPos;
in vec3 lightPos;
in vec3 camPos;

out vec4 fragColor;

uniform sampler2D inPosTexture;

void main(void)
{	  
  const vec4  diffColor = vec4 ( 0.5, 0.5, 1.0, 0.7);
  const vec4  specColor = vec4 ( 0.5, 0.5, 0.5, 0.5);
  const float specPower = 30.0;

  vec3 light = normalize(lightPos - worldPos);
  vec3 eye = normalize(camPos - worldPos);
  vec3 norm = normalize(fragmentNormal);

  vec3 h = normalize(light + eye);

  vec4 diff = diffColor * max(dot(norm, light), 0.0);
  vec4 spec = specColor * pow(max(dot(norm, h), 0.0), specPower);

  fragColor = diff + spec;
}
