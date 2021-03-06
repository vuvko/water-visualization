#version 330

in vec3 fragmentWorldPos;
in vec3 fragmentNormal;
in vec2 fragmentTexCoord;

in vec4 worldPos;

in vec3 camTan;
in vec3 lightTan;
in vec3 lightPos;
in vec3 camPos;

out vec4 fragColor;

uniform sampler2D u_texture;
uniform sampler2D u_texture_n;

uniform float scale = 0.0;

void main(void)
{
  const float specPower = 80.0;
  float numSteps = 100;
  float step   = 1.0 / numSteps;

  vec2  dtex   = camTan.xy * scale / ( numSteps * camTan.z );
  float height = 1;
  vec2  tex    = fragmentTexCoord;
  float h      = 1 - texture ( u_texture_n, tex ).a;
  if (scale < 0.0001) {
    height = h;
  }

  while ( h < height ) {
    height -= step;
    tex    += dtex;
    h     = 1 - texture ( u_texture_n, tex ).a;
  }
  
  vec2  prev   = tex - dtex;
  float hPrev  = (1 - texture ( u_texture_n, prev ).a) - (height + step);
  float hCur   = h - height;
  float weight = hCur / (hCur - hPrev );

  tex = weight * prev + (1.0 - weight) * tex;         

  vec3  color = texture ( u_texture, tex ).rgb;
  vec3  n     = normalize ( texture ( u_texture_n, tex ).rgb * 2.0 - vec3 ( 1.0 ) );
  float diff  = max ( dot ( n, normalize(lightTan) ), 0.0 );
  float spec  = pow ( max ( dot ( n, normalize(lightTan + camTan) ), 0.0 ), specPower );

  fragColor = vec4 ( color * diff + vec3 ( 0.05 * spec ), 1.0 );
}
