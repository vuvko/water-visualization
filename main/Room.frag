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

uniform vec3 g_diffuseColor;
uniform vec3 g_specularColor;

uniform sampler2D u_texture;

void main(void)
{
  const float specPower = 30.0;
  vec3 light = normalize(lightPos);
  vec3 eye = normalize(camPos - worldPos);
  vec3 norm = normalize(fragmentNormal);

  vec3 h = normalize(light + eye);

  vec3 diff = g_diffuseColor * max(dot(norm, eye), 0.0);
  vec3 spec = g_specularColor * pow(max(dot(norm, h), 0.0), specPower);

  fragColor = vec4(diff + spec, 1.0);
  /*
  const vec4  diffColor = vec4 ( 0.5, 0.5, 1.0, 0.5);
  const vec4  specColor = vec4 ( 0.5, 0.5, 0.5, 0.3);
  const float specPower = 30.0;
  //vec3 lPos = vec3(0, 0, 10);
  //vec3 gPos = vec3(10, 10, 10);

  vec3 light = normalize(lightPos - worldPos);
  vec3 eye = normalize(camPos - worldPos);
  vec3 norm = normalize(fragmentNormal);

  vec3 h = normalize(light + eye);

  vec4 diff = diffColor * max(dot(norm, light), 0.0);
  vec4 spec = specColor * pow(max(dot(norm, h), 0.0), specPower);

  fragColor = diff + spec;
  *//*
  float scale = 10;
  float numSteps = 10;
  float step   = 1.0 / numSteps;

  vec2  dtex   = camTan.xy * scale / ( numSteps * camTan.z ); 
    float height = 1;                                
    vec2  tex    = fragmentTexCoord;                 
    float he      = texture ( u_texture, tex ).a;     
	/*
    while ( he < height )                             
    {
        height -= step;
        tex    += dtex;
        he       = texture ( u_texture, tex ).a;
    }
    *//*     
	vec2  prev   = tex - dtex;                                                
    float hPrev  = texture ( u_texture, prev ).a - (height + step);
    float hCur   = he - height;
    float weight = hCur / (hCur - hPrev );

    tex = weight * prev + (1.0 - weight) * tex;         
	*//*
  vec3  color = texture ( u_texture, tex ).rgb;
  fragColor = vec4(color, 1.0);
  *//*
  vec3  n     = normalize ( texture ( u_texture, tex ).rgb * 2.0 - vec3 ( 1.0 ) );
  float diff  = max ( dot ( n, normalize ( lightTan ) ), 0.0 );
  float spec  = pow ( max ( dot ( n, normalize ( lightTan ) ), 0.0 ), specPower );

  fragColor = vec4 ( color * diff + vec3 ( 0.7 * spec ), 1.0 );
  */
  //fragColor = vec4(texture(u_texture, fragmentTexCoord).rgb * color, 1.0);
  /*
  float scale = 1.0;
  float bias = 0.0;
  float h = scale * texture(u_texture, fragmentTexCoord).a + bias;
  vec2 tex = fragmentTexCoord + camTan.xy * h / camTan.z;
  fragColor = vec4(texture(u_texture, tex).rgb, 1.0);
  */
}
