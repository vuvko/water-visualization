#version 330

in vec3 fragmentWorldPos;
in vec3 fragmentNormal;
in vec2 fragmentTexCoord;

in vec4 worldPos;

in vec3 lightPos;
in vec3 camPos;

out vec4 fragColor;

uniform sampler2D u_texture;

void main(void)
{
    const vec3  diffColor = vec3 ( 0.5, 0.5, 0.5);
    const vec3  specColor = vec3 ( 0.3, 0.3, 0.3);
    const float specPower = 40.0;

    vec3 light = normalize(lightPos - worldPos);
    vec3 eye = normalize(camPos - worldPos);
    vec3 norm = -normalize(fragmentNormal);

    vec3 hv = normalize(light + eye);

    vec3 diff = diffColor * max(dot(norm, light), 0.0);
    vec3 spec = specColor * pow(max(dot(norm, hv), 0.0), specPower);

    fragColor = vec4((diff + spec) * texture(u_texture, fragmentTexCoord).rgb, 1);
}
