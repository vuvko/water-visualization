#version 330

in vec3 fragmentWorldPos;
in vec3 fragmentNormal;
in vec2 fragmentTexCoord;

in vec4 worldPos;

in vec3 camTan;
in vec3 lightTan;

out vec4 fragColor;

// you can set these variables from the CPU code using setUniform function
//
uniform vec3 g_lightPos; 
uniform vec3 g_camPos;

uniform vec3 g_diffuseColor  = vec3(1,1,1);
uniform vec3 g_specularColor = vec3(1,1,1);

uniform sampler2D u_texture;

void main(void)
{	
  const float specPower = 30.0;
  vec3 light = normalize(g_lightPos);
  vec3 eye = normalize(g_camPos - worldPos);
  vec3 norm = normalize(fragmentNormal);

  vec3 h = normalize(light + eye);

  vec3 diff = g_diffuseColor * max(dot(norm, light), 0.0);
  vec3 spec = g_specularColor * pow(max(dot(norm, h), 0.0), specPower);

  vec3 color = diff + spec;

  float scale = 1.0;
  float bias = 0.0;

  float height = scale * (1.0 - texture(u_texture, fragmentTexCoord).a) + bias;
  vec2 tex = fragmentTexCoord - camTan.xy * height / camTan.z;

  fragColor = vec4(texture(u_texture, tex).rgb, 1.0);

  //fragColor = vec4(texture(u_texture, fragmentTexCoord).rgb * color, 1.0);
}
